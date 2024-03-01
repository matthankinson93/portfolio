/*
 * Monitors vehicle data (coolant temp, oil pressure, boost pressure, EGT) via CAN J1939 PGN Codes.
 * LED & buzzer triggered based on configurable thresholds.
 * UART for configuration upload.
 * MQTT for data publishing (anonymous)
 * NVS for configuration storage across reboots.
 * Uses FreeRTOS for task management and ESP-IDF libraries for peripheral control.
 * 
 * Pinout:
 * - UART: 
 *   - TX: Default UART0 TX (GPIO1)
 *   - RX: Default UART0 RX (GPIO3)
 * - CAN:
 *   - TX: GPIO 18 (CAN_TX_GPIO)
 *   - RX: GPIO 23 (CAN_RX_GPIO)
 * - GPIO:
 *   - LED: GPIO 2 (LED_GPIO)
 *   - Buzzer: GPIO 4 (BUZZER_GPIO)
 * 
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/can.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "driver/uart.h"
#include "mqtt_client.h"

// UART Bridge
#define EX_UART_NUM UART_NUM_0
#define BUF_SIZE (1024)

 // GPIO
#define LED_GPIO 2
#define BUZZER_GPIO 4
#define CAN_TX_GPIO GPIO_NUM_18 
#define CAN_RX_GPIO GPIO_NUM_23

// J1939 PGN
#define COOLANT_TEMP_CAN_ID 0x18FEEE00
#define OIL_PRESSURE_CAN_ID 0x18FEFB00
#define BOOST_PRESSURE_CAN_ID 0x18FEF200
#define EGT_TEMP_CAN_ID 0x18FEDF00

// Default Threshold
#define DEFAULT_T_THRES 80.0
#define DEFAULT_O_THRES 20.0
#define DEFAULT_B_THRES 8.0
#define DEFAULT_E_THRES 70.0

static bool ALERT = false;
static SemaphoreHandle_t MUTEX;

// Values
float C_TEMP;
float O_PRES;
float B_PRES;
float E_TEMP;

// Thresholds
float T_THRES;
float O_THRES;
float B_THRES;
float E_THRES;

// MQTT
static const char *MQTT_BROKER_URI;
static char mqtt_payload[256];
esp_mqtt_client_handle_t MQTT_CLIENT; 

void uart_init() {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    ESP_ERROR_CHECK(uart_param_config(EX_UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

void can_init() {
  can_general_config_t g_config  = {
    .mode = CAN_MODE_NORMAL,
    .tx_io = CAN_TX_GPIO,
    .rx_io = CAN_RX_GPIO,
    .clkout_io = CAN_CLKOUT_GPIO_UNUSED,
    .bus_off_io = CAN_BUS_OFF_GPIO_UNUSED,  
    .tx_queue_len = 5,
    .rx_queue_len = 10,
    .alerts_enabled = CAN_ALERT_ALL,
    .clkout_divider = 0
  };

  can_timing_config_t t_config = {
    .baudrate = 500000,
    .sjw = CAN_SJW_1TQ,              
    .tseg_1 = CAN_TSEG1_12TQ,
    .tseg_2 = CAN_TSEG2_3TQ,
    .triple_sampling = CAN_TRIPLE_SAMPLING_DISABLE
  };

  can_filter_config_t f_config = {
    .acceptance_code = 0,
    .acceptance_mask = 0,      
    .single_filter = true                
  }; 

  ESP_ERROR_CHECK(can_driver_install(&g_config, &t_config, &f_config));
  ESP_ERROR_CHECK(can_start());
}

void gpio_init() {
    gpio_pad_select_gpio(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(BUZZER_GPIO);
    gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);
}

void mqtt_init(void) {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    size_t required_size = 0;
    err = nvs_get_str(nvs_handle, "MQTT_URI", NULL, &required_size);
    char* MQTT_BROKER_URI = malloc(required_size);
    err = nvs_get_str(nvs_handle, "MQTT_URI", MQTT_BROKER_URI, &required_size);

    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = MQTT_BROKER_URI,
    };
    MQTT_CLIENT = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(MQTT_CLIENT, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(MQTT_CLIENT);

    free(MQTT_BROKER_URI); 
    nvs_close(nvs_handle);
}

static esp_err_t nvs_init() {
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
    return ret;
}

void save_t(float T_THRES, float O_THRES, float B_THRES, float E_THRES) {
    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open("storage", NVS_READWRITE, &my_handle));
    ESP_ERROR_CHECK(nvs_set_blob(my_handle, "T_THRES", &T_THRES, sizeof(float)));
    ESP_ERROR_CHECK(nvs_set_blob(my_handle, "O_THRES", &O_THRES, sizeof(float)));
    ESP_ERROR_CHECK(nvs_set_blob(my_handle, "B_THRES", &B_THRES, sizeof(float)));
    ESP_ERROR_CHECK(nvs_set_blob(my_handle, "E_THRES", &E_THRES, sizeof(float)));
    ESP_ERROR_CHECK(nvs_commit(my_handle));
    nvs_close(my_handle);
}

void load_t(float *T_THRES, float *O_THRES, float *B_THRES, float *E_THRES) {
    nvs_handle_t t;
    ESP_ERROR_CHECK(nvs_open("storage", NVS_READONLY, &t));
    size_t size;

    if (nvs_get_blob(t, "T_THRES", T_THRES, &size) != ESP_OK) {
        *T_THRES = DEFAULT_T_THRES;
    }
    size = sizeof(float);

    if (nvs_get_blob(t, "O_THRES", O_THRES, &size) != ESP_OK) {
        *O_THRES = DEFAULT_O_THRES;
    }
    size = sizeof(float);

    if (nvs_get_blob(t, "B_THRES", B_THRES, &size) != ESP_OK) {
        *B_THRES = DEFAULT_B_THRES;
    }
    size = sizeof(float);

    if (nvs_get_blob(t, "E_THRES", E_THRES, &size) != ESP_OK) {
        *E_THRES = DEFAULT_E_THRES;
    }

    nvs_close(t);
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event) {
  esp_mqtt_client_handle_t client = event->client;
  return ESP_OK;
}

static void mqtt_event_handler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data) {
    mqtt_event_handler_cb(event_data);
}

void uart_mon(void *pvParameters) {
    const int max_cmd_len = 1024;
    uint8_t* data = (uint8_t*) malloc(BUF_SIZE);
    char cmd_buf[max_cmd_len];
    int cmd_len = 0;

    while (1) {
    int len = uart_read_bytes(EX_UART_NUM, data, BUF_SIZE - 1, 20 / portTICK_RATE_MS);
    if (len > 0) {
       data[len] = '\0';
      for (int i = 0; i < len; ++i) {

        if (cmd_len < max_cmd_len - 1) {
          cmd_buf[cmd_len++] = (char)data[i];
          cmd_buf[cmd_len] = '\0';
        }
        
        if (data[i] == '\n') {
          float newThres;
          char thresType[10];
          
          if (sscanf(cmd_buf, "SET %s %f\n", thresType, &newThres) == 2) {
            if (strcmp(thresType, "T_THRES") == 0) T_THRES = newThres;
            else if (strcmp(thresType, "O_THRES") == 0) O_THRES = newThres;
            else if (strcmp(thresType, "B_THRES") == 0) B_THRES = newThres;
            else if (strcmp(thresType, "E_THRES") == 0) E_THRES = newThres;
            save_t(T_THRES, O_THRES, B_THRES, E_THRES);
          } else if (sscanf(cmd_buf, "SET MQTT_URI %s\n", MQTT_BROKER_URI) == 1) {
            esp_err_t err = esp_flash_data_set("MQTT_URI", MQTT_BROKER_URI, strlen(MQTT_BROKER_URI));
          }
          cmd_len = 0;
        }
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  free(data);
}

void oil_pres(void *pvParameters) {
  can_message_t message;
  while (1) {
    if (can_receive(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
      if (message.identifier == OIL_PRESSURE_CAN_ID) {
        uint32_t r = (message.data[3] << 24) |
                      (message.data[2] << 16) |
                      (message.data[1] << 8) |
                       message.data[0];
                       
        int sign = (r >> 31) & 1;
        int exp = ((r >> 23) & 0xFF) - 127;
        int mant = (r & 0x7FFFFF) | 0x800000;
        O_PRES = pow(-1, sign) * (mant / pow(2, 23)) * pow(2, exp); 
        if (O_PRES < O_THRES) {
          xSemaphoreTake(MUTEX, portMAX_DELAY);
          ALERT = true;
          xSemaphoreGive(MUTEX);
        }
      }
    }
  vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void c_temp(void *pvParameters) {
  can_message_t message;
  while (1) {
    if (can_receive(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
      if (message.identifier == COOLANT_TEMP_CAN_ID) {
        uint32_t r = (message.data[3] << 24) |
                      (message.data[2] << 16) |
                      (message.data[1] << 8) |
                      message.data[0];
 
        int sign = (r >> 31) & 1;
        int exp = ((r >> 23) & 0xFF) - 127;
        int mnt = (r & 0x7FFFFF) | 0x800000;
        C_TEMP = pow(-1, sign) * (mnt / pow(2, 23)) * pow(2, exp);
        if (C_TEMP > T_THRES) {
          xSemaphoreTake(MUTEX, portMAX_DELAY);
          ALERT = true;
          xSemaphoreGive(MUTEX);
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void b_pres(void *pvParameters) {
  can_message_t message;
  while (1) {
    if (can_receive(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
      if (message.identifier == BOOST_PRESSURE_CAN_ID) {
        uint32_t r = (message.data[3] << 24) |
                      (message.data[2] << 16) |
                      (message.data[1] << 8) |
                       message.data[0];
                       
        int sign = (r >> 31) & 1;
        int exp = ((r >> 23) & 0xFF) - 127;
        int mant = (r & 0x7FFFFF) | 0x800000;
        B_PRES = pow(-1, sign) * (mant / pow(2, 23)) * pow(2, exp);
        if (B_PRES < B_THRES) {
          xSemaphoreTake(MUTEX, portMAX_DELAY);
          ALERT = true;
          xSemaphoreGive(MUTEX);
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void e_temp(void *pvParameters) {
  can_message_t message;
  while (1) {
    if (can_receive(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
      if (message.identifier == EGT_TEMP_CAN_ID) {
        uint32_t r = (message.data[3] << 24) |
                      (message.data[2] << 16) |
                      (message.data[1] << 8) |
                       message.data[0];
                       
        int sign = (r >> 31) & 1;
        int exp = ((r >> 23) & 0xFF) - 127;
        int mant = (r & 0x7FFFFF) | 0x800000;
        E_TEMP = pow(-1, sign) * (mant / pow(2, 23)) * pow(2, exp);
        if (E_TEMP < E_THRES) {
          xSemaphoreTake(MUTEX, portMAX_DELAY);
          ALERT = true;
          xSemaphoreGive(MUTEX);
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void reset(void *pvParameters) {
    bool a = true;

    if ((C_TEMP > T_THRES) &&
        (O_PRES < O_THRES) &&
        (B_PRES < B_THRES) &&
        (E_TEMP > E_THRES)) a = false;

    if (!a) {
        xSemaphoreTake(MUTEX, portMAX_DELAY);
        ALERT = false;
        xSemaphoreGive(MUTEX);
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
}

void check_and_alert() {
  if (ALERT) {
    gpio_set_level(LED_GPIO, 1);
    gpio_set_level(BUZZER_GPIO, 1);
  } else {
    gpio_set_level(LED_GPIO, 0);
    gpio_set_level(BUZZER_GPIO, 0);
  }
}

void alert(void *pvParameters) {
  while (1) {
    check_and_alert();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void mqtt_pub(void *pvParameters) {
  while (1) {
    snprintf(mqtt_payload, sizeof(mqtt_payload), "{\"CoolantTemp\":%.2f, \"OilPressure\":%.2f, \"BoostPressure\":%.2f, \"EGTTemp\":%.2f}", C_TEMP, O_PRES, B_PRES, E_TEMP);
    if (mqtt_client != NULL) {
      int msg_id = esp_mqtt_client_publish(mqtt_client, "devices/telemetry", mqtt_payload, 0, 1, 0);
    }
    vTaskDelay(300000 / portTICK_PERIOD_MS);
  }
}

void app_main() {

  uart_init();
  can_init();
  gpio_init(); 
  mqtt_init();
  esp_err_t err = nvs_init();
  load_t(&T_THRES, &O_THRES, &B_PRES, &E_THRES);

  MUTEX = xSemaphoreCreateMutex();

  xTaskCreate(oil_pres, "oil_pres", 2048, NULL, 10, NULL);
  xTaskCreate(c_temp, "c_temp", 2048, NULL, 10, NULL);
  xTaskCreate(b_pres, "b_pres", 2048, NULL, 10, NULL);
  xTaskCreate(e_temp, "e_temp", 2048, NULL, 10, NULL);
  xTaskCreate(alert, "alert", 2048, NULL, 9, NULL);
  xTaskCreate(reset, "reset", 2048, NULL, 8, NULL);
  xTaskCreate(uart_mon, "uart_mon", 4096, NULL, 8, NULL);
  xTaskCreate(mqtt_pub, "mqtt_pub", 4096, NULL, 7, NULL);
}