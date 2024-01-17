import requests
import json
import csv 
import base64
import io
from datetime import datetime, timedelta

# SERVER ADDRESS URL
SH_ADDRESS = 'your_api_endpoint'

# ENDPOINT FOR JWT AUTHENTICATION TOKEN
SELF_HOST_TOKEN_URL = SH_ADDRESS + '/api/auth/login'

# ENDPOINT FOR TELEMETRY RESOURCE (SWAGGER_UI FOR REST APIs)
TELEMETRY_URL = SH_ADDRESS + '/api/plugins/telemetry/'

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

# Function to get a list of devices
def getDevices(URL):
    print("Getting devices...")
    try:
        # Make the GET request with the provided parameters and headers
        response = requests.get(TELEMETRY_URL, headers=SH_HEADERS)
        # Check if the request was successful (status code 200)
        if response.status_code == 200:
            # Return the JSON data from the response
            return response.json()
            print(response.json)
        else:
            # Print an error message if the request was not successful
            print(f"Error: {response.status_code} - {response.text}")
            return None
    except requests.exceptions.RequestException as e:
        # Handle any exception that might occur during the request
        print(f"An error occurred: {e}")
        return None
    
# Replace with your credentials
USERNAME_SH = 'username@account.com.au'
PASSWORD = 'pa55word'

# Get authenticated token
jwt_sh_token = getSH_JWT(SELF_HOST_TOKEN_URL)

# Build API header
SH_HEADERS = {'Content-Type': 'application/json', 'X-Authorization': 'Bearer ' + jwt_sh_token}

# Get devices
devices_data = getDevices(TELEMETRY_URL)

# Sanitise JSON values
nested_objects = devices_data[0]['value']

# Define CSV Headers
csv_headers = ["Site Name", "Site Code", "Serial No", "Name", "Name", "Level", "Last Reading"]

csv_bytes_io = io.BytesIO()
header_row = ','.join(csv_headers) + '\n'
csv_bytes_io.write(header_row.encode())

# Loop through the JSON for devices, and build a CSV
for site, values in nested_objects.items():
    data_row = ','.join(str(value) for value in values.values()) + '\n'
    csv_bytes_io.write(data_row.encode())

# Convert to base64 encoding for file transfer
csv_bytes_io.seek(0)
csv_bytes = csv_bytes_io.read()
base64_encoded = base64.b64encode(csv_bytes).decode()

# Save as base64 file
with open("csvReport", mode="w", encoding="utf-8") as outfile:
    outfile.write(base64_encoded)

# Open back up base64 file
with open("csvReport", mode="r", encoding="utf-8") as infile:
    base64_encoded = infile.read()

# Decode the csv and save locally to server in current directory
csv_bytes = base64.b64decode(base64_encoded)
csv_headers = ["Site Name", "Site Code", "Serial No", "Name", "Name", "Level", "Last Reading"]csv_rows = csv_bytes.decode("utf-8").splitlines()

with open("csvReport_decoded.csv", mode="w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(csv_headers)

    for row in csv_rows:
        values = row.split(',')
        writer.writerow(values)

print("CSV data has been decoded and saved to 'svReport_decoded.csv'.")