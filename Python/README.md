## Python Examples

#### getDeviceData:

Script used for retrieving a list of defined database keys via the RESTful API architecture, with the use of JSON Web Token Authentication.
The Script will generate a base64 endoded csv file for FTP, and a decoded csv file in the main directory.

#### transferTelemetryAttributes:

Script used for merging a list of database keys from one API endpoint to another - useful for data migration across resources that use the RESTful API architecture.
The script will gather, transform, and post all data for a speficied resource (based on it's UUID) from one cloud server to another.
The script uses 'tkinkter' GUI library.
