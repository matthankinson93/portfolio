import { Component, Inject, OnInit, AfterViewInit, ElementRef, ViewChild } from '@angular/core';
import { MAT_DIALOG_DATA, MatDialogRef } from '@angular/material/dialog';
import { Device } from '../models/device.model';
import { DeviceTelemetryService } from '../services/device-telemetry.service';
import { DeviceTelemetry } from '../models/device-telemetry';
import { Chart } from 'chart.js';

@Component({
  selector: 'app-device-detail-dialog',
  templateUrl: './device-detail-dialog.component.html',
  styleUrls: ['./device-detail-dialog.component.css']
})

export class DeviceDetailDialogComponent implements AfterViewInit {
  deviceTelemetry: DeviceTelemetry[] = [];
  displayedColumns: string[] = ['telemetry_key', 'telemetry_value'];
  multi: any[] = [];
  
  @ViewChild('lineChartCanvas') private chartRef!: ElementRef<HTMLCanvasElement>;
  private chart!: Chart;

  constructor(
    public dialogRef: MatDialogRef<DeviceDetailDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: { device: Device },
    private deviceTelemetryService: DeviceTelemetryService
  ) {
    this.loadTelemetryData();
    this.loadTelemetryRange();
  }

  ngAfterViewInit(): void {
    this.chart = new Chart(this.chartRef.nativeElement, {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          label: 'Dataset 1',
          data: [],
          backgroundColor: 'rgba(0, 123, 255, 0.5)',
          borderColor: 'rgba(0, 123, 255, 1)',
          borderWidth: 1
        }]
      },
      options: {
        scales: {
          y: {
            beginAtZero: true,
            ticks: {
              font: {
                size: 8
              }
            }
          },
          x: {
            ticks: {
              font: {
                size: 8
              }
            }
          }
        }
      }
    });
  }

  loadTelemetryData(): void {
    const deviceId = this.data.device.device_id;
    if (deviceId) {
      this.deviceTelemetryService.getLatestTelemetry(deviceId).subscribe({
        next: (telemetry) => {
          telemetry.forEach(item => {
            item.telemetry_value = parseFloat(item.telemetry_value).toFixed(2)
          });
          this.deviceTelemetry = telemetry;
        },
        error: (err) => {
          console.error('Error fetching telemetry data:', err);
        }
      });
    }
  }

  loadTelemetryRange(): void {
    const deviceId = this.data.device.device_id;
    const endDate = new Date();
    const startDate = new Date();
    startDate.setDate(endDate.getDate() - 14);

    const startTS = startDate.toISOString();
    const endTS = endDate.toISOString();

    if (deviceId) {
      this.deviceTelemetryService.getTelemetryDataInRange(deviceId, startTS, endTS).subscribe({
        next: (telemetry) => {
          this.processTelemetryDataForChart(telemetry);
        },
        error: (err) => {
          console.error('Error fetching telemetry data for the last 14 days:', err);
        }
      });
    }
  }

  processTelemetryDataForChart(telemetryData: DeviceTelemetry[]): void {
    if (!this.chart) {
      console.error('Chart has not been initialized.');
      return;
    }
  
    const datasetsMap = new Map<string, { timestamps: Date[], values: number[] }>();
  
    telemetryData.forEach((dataPoint) => {
      if (!dataPoint.telemetry_key || !dataPoint.telemetry_timestamp || isNaN(parseFloat(dataPoint.telemetry_value))) {
        console.error('Invalid data point:', dataPoint);
        return;
      }
  
      let dataset = datasetsMap.get(dataPoint.telemetry_key);
      if (!dataset) {
        dataset = { timestamps: [], values: [] };
        datasetsMap.set(dataPoint.telemetry_key, dataset);
      }
      dataset.timestamps.push(new Date(dataPoint.telemetry_timestamp));
      dataset.values.push(parseFloat(dataPoint.telemetry_value));
    });
  
    this.chart.data.labels = [];
    this.chart.data.datasets = [];
  
    datasetsMap.forEach((dataset, key) => {
      let sortedTimestampsValues = dataset.timestamps
      .map((timestamp, index) => ({ timestamp, value: dataset.values[index] }))
      .sort((a, b) => a.timestamp.getTime() - b.timestamp.getTime());

      let sortedTimestamps = sortedTimestampsValues.map(tv => tv.timestamp);
      let sortedValues = sortedTimestampsValues.map(tv => tv.value);


      this.chart.data.labels = sortedTimestamps.map(timestamp => timestamp.toLocaleString());
      
      this.chart.data.datasets.push({
        label: key,
        data: dataset.values,
        fill: false,
        borderColor: this.getRandomColor(),
        tension: 0.1,
        pointRadius: 0,
      });
    });
    this.chart.update();
  }
  
  private getRandomColor(): string {
    const r = Math.floor(Math.random() * 255);
    const g = Math.floor(Math.random() * 255);
    const b = Math.floor(Math.random() * 255);
    return `rgba(${r},${g},${b},0.5)`;
  }
}