## IoT Application

### Starting the development environment:

#### Postgres:
macOS: 'brew services start postgresql'

Linux/Unix: 'pg_ctl -D /path/to/your/postgresql/data/directory start' - where '/path/to/your/postgresql' is the location or docker of your postgres database.

#### Front end application:
run 'ng serve -o' from the root directory. 
Navigate to http://localhost:4200/. The application will automatically reload if you change any of the source files.

#### Back end / express server
run 'npm start' from the root directory.
run 'ps aux | grep node' to verify

## Starting a production environment:

#### For Linux/Unix based cloud server:
ensure a '.service' file exists for a unix based server.
a setup script exists at './setup-services.sh'
ensure permissions are set first by running 'chmod +x setup-services.sh'

## API ROUTES:

#### Login service
'baseURL/api/login'

#### User service
'baseURL/api/users'

#### Device service
'baseURL/api/devices'

#### Telemtry handler
'baseURL'/api/telemetry'

### Posting telemetry examples
In order to post telemetry, a device must be created, and the auth token saved for the API endpoint.

#### CURL:
curl -X POST http://baseURL/api/telemetry/saveTelemetry \
     -H "Authorization: Bearer auth_token" \
     -H "Content-Type: application/json" \
     -d '{"key": "temperature", "value": "25", "ts": "1705402416000"}'

#### Python:
import requests

url = "http://baseURL/api/telemetry/saveTelemetry"
headers = {
    "Authorization": "Bearer auth_token",
    "Content-Type": "application/json"
}
data = {
    "key": "temperature",
    "value": "25",
    "ts": "1705402416000"
}

response = requests.post(url, json=data, headers=headers)
print(response.json())


## Demonstrations

#### Login Screen for authenticated users:

![image](https://github.com/matthankinson93/portfolio/assets/102736018/cc40af11-c67c-48f9-9f7a-688c2a7c5e89)

#### Entities overview:

![image](https://github.com/matthankinson93/portfolio/assets/102736018/76ad8a7f-54db-44f6-8298-3296dfd70a8b)

#### Creating a new device:

![image](https://github.com/matthankinson93/portfolio/assets/102736018/1405761e-1fea-4248-bd56-d00272ac3813)

#### Viewing a device's telemetry, latest values, and authentication information:

![image](https://github.com/matthankinson93/portfolio/assets/102736018/b5fe2628-3ce7-45f2-8349-459103d5845e)



