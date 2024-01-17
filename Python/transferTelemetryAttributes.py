import requests
import json
import time
import datetime
import pytz
import tkinter as tk
from tkinter import simpledialog
from tkinter import messagebox

# SERVER ADDRESS URL
CLOUD_ADDRESS = 'your_api_endpoint'
SH_ADDRESS = 'your_api_endpoint'

# ENDPOINT FOR JWT AUTHENTICATION TOKEN
TOKEN_URL = CLOUD_ADDRESS + '/api/auth/login'

# ENDPOINT FOR JWT AUTHENTICATION TOKEN
SELF_HOST_TOKEN_URL = SH_ADDRESS + '/api/auth/login'

# ENDPOINT FOR TELEMETRY RESOURCE (SWAGGER_UI FOR REST APIs)
TIMESERIES_URL = CLOUD_ADDRESS + '/api/plugins/telemetry/DEVICE/

# ENDPOINT FOR TELEMETRY RESOURCE (SWAGGER_UI FOR REST APIs)
POST_TIMESERIES_URL = SH_ADDRESS + '/api/plugins/telemetry/{}/{}/timeseries/'

# ENDPOINT FOR TELEMETRY RESOURCE (SWAGGER_UI FOR REST APIs)
ATTRIBUTES_URL = CLOUD_ADDRESS + '/api/plugins/telemetry/DEVICE/{}/values/attributes/'

# ENDPOINT FOR TELEMETRY RESOURCE (SWAGGER_UI FOR REST APIs)
POST_ATTRIBUTES_URL = SH_ADDRESS + '/api/plugins/telemetry/DEVICE/{}/attributes/'

# Function to get JWT Authentication Token
def getJWT(URL):
    print("Getting new JWT token...")
    auth_data = {'username': USERNAME, 'password': PASSWORD}
    response = requests.post(URL, json=auth_data)

    if response.status_code == 200:
        print('JWT Token obtained!')
        jwt_token = json.loads(response.content)['token']
        return jwt_token
    else:
        print('Failed to obtain JWT token: ', response.content)

# Function to get JWT Authentication Token
def getSH_JWT(URL):
    print("Getting new JWT token for SH...")
    auth_data = {'username': USERNAME_SH, 'password': PASSWORD}
    response = requests.post(URL, json=auth_data)

    if response.status_code == 200:
        print('JWT Token obtained!')
        jwt_token = json.loads(response.content)['token']
        return jwt_token
    else:
        print('Failed to obtain JWT token: ', response.content)

# Function to get a list of data keys from a resource
def getDeviceData(startTs, endTs):
    print("Getting resource data, this may take some time...")

    # Keys
    keys = ['keys'] # Replace with valid database keys

    # ARGUMENTS FOR API HEADER
    data = {
        'keys': keys,
        'startTs': startTs,
        'endTs': endTs,
        "agg": "AVG",
    }

    response = requests.get(TIMESERIES_URL.format(DEVICE_ID), headers=CLOUD_HEADERS, params=data)
    
    if response.status_code == 200:
        response = response.json()
        print('Successfully retrieved data from resource!')
        postDeviceData(response)
    else:
        print('Failed to retrieve data from resource: ', response.content)


# Function to post a list of data keys from a resource    
def postDeviceData(data):
    print("Posting existing data to new resource...")
    post_url = POST_TIMESERIES_URL.format('DEVICE', NEW_DEVICE_ID, 'telemetry', '100000000')
    
    # parse original JSON response
    response_dict = data

    # create list of transformed data
    transformed_data = []
    if response_dict:
        for ts in response_dict[next(iter(response_dict))]:
            data_dict = {"ts": ts["ts"], "values": {}}
            for key in response_dict:
                for datapoint in response_dict[key]:
                    if datapoint["ts"] == ts["ts"]:
                        data_dict["values"][key] = round(float(datapoint["value"]), 2)
                        break
            transformed_data.append(data_dict)
            DEMO_JSON.append(transformed_data)
    response = requests.post(post_url, headers=SH_HEADERS, data=json.dumps(transformed_data))

    if response.status_code == 200:
        print('Successfully posted data to new resource!')
    else:
        print('Failed to post data to new resource: ', response.content)

# Function to get a list of data keys attributes from a resource  
def getAttributes():
    print('Retrieving device attributes...')
    
    # Attribute keys
    keys = ['keys'] 
    params = {'keys': keys}

    response = requests.get(ATTRIBUTES_URL.format(DEVICE_ID, ''), headers=CLOUD_HEADERS, params=params)

    if response.status_code == 200:
        response = response.json()
        postJSON = convertJSON(response)
        postAttributes(postJSON)
    else:
        print('Failed to retrieve resource attributes:', response.content)
    
# Function to post a list of data keys attributes to a resource  
def postAttributes(attributes):
    print('Posting attributes to new resource...')
    post_url = POST_ATTRIBUTES_URL.format(NEW_DEVICE_ID)
    response = requests.post(post_url, headers=SH_HEADERS, data=attributes)

    if response.status_code == 200:
        print('Successfully posted attributes to new resource!')
    else:
        print('Failed to post attributes data to new resource: ', response.content)

# HELPER FUNCITON TO CONVERT ATTRIBUTES INTO VALID JSON
def convertJSON(attributes):
    data = {}

    # Iterate over the 'attributes' list
    for attribute in attributes:
        key = attribute['key']
        value = attribute['value']
    
        # Check if the value is a dictionary
        if isinstance(value, dict):
            transformed_value = {}
        
            # Iterate over the key-value pairs in the dictionary
            for k, v in value.items():
                # Check if the inner value is a list
                if isinstance(v, list):
                    transformed_value[k] = v
                else:
                    transformed_value[k] = [v]
        
            data[key] = transformed_value
        else:
            data[key] = value

    # Convert the 'data' dictionary to JSON
    result = json.dumps(data)
    return result


# !! MAIN DRIVER CODE !! #
root = tk.Tk()
root.withdraw()
USERNAME = simpledialog.askstring(title="Input credentials", prompt="Enter your account email address:")
USERNAME_SH = simpledialog.askstring(title="Input credentials", prompt="Enter your account email address:")
PASSWORD = simpledialog.askstring(title="Input credentials", prompt="Enter your account password:", show="•")
DEVICE_ID = simpledialog.askstring(title="Input device ID", prompt="Enter the existing resource UUID:")
NEW_DEVICE_ID = simpledialog.askstring(title="Input device ID", prompt="Enter the new resource UUID:")
TIMEZONE = timezone = pytz.timezone('Australia/Brisbane')
DEMO_JSON = []

# Get authenticated token
jwt_token = getJWT(TOKEN_URL)
jwt_sh_token = getSH_JWT(SELF_HOST_TOKEN_URL)

# Build API header
CLOUD_HEADERS = {'Content-Type': 'application/json', 'X-Authorization': 'Bearer ' + jwt_token}
SH_HEADERS = {'Content-Type': 'application/json', 'X-Authorization': 'Bearer ' + jwt_sh_token}

# Get and post resource attributes
getAttributes()

timezone = pytz.timezone('Australia/Brisbane')
now = int(datetime.datetime.now(timezone).timestamp() * 1000)

# Set a valid timewindow
start_ts = int((now - 31556926000))  # 1 year ago from BNE timezone
end_ts = start_ts + 86400000  # 1 day after start time

# Loop each day of data
while end_ts <= now:
    start_dt = datetime.datetime.fromtimestamp(start_ts/1000, tz=timezone)
    end_dt = datetime.datetime.fromtimestamp(end_ts/1000, tz=timezone)
    start_str = start_dt.strftime('%d-%m-%Y %H:%M')
    end_str = end_dt.strftime('%d-%m-%Y %H:%M')
    print(f'Data timeframe: {start_str} to {end_str} (Brisbane time)')
    getDeviceData(start_ts, end_ts)
    start_ts += 86400000  # increment by 24 hours in ms
    end_ts += 86400000  # increment by 24 hours in

# Get any data missed from today
getDeviceData((now - 86400000), now)

print('Resource migration successful!')
# end 