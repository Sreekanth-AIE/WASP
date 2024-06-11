import requests
from smtp_util import SendMail
from Adafruit_IO import Client, Data


# Adafruit IO credentials
ADAFRUIT_IO_USERNAME = 'your_username'
ADAFRUIT_IO_KEY = 'your_key'

aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
feed = "wifi_name"

def get_position_from_server(api_endpoint):
    try:
        wifi_name = aio.receive(feed)
        if wifi_name:
            print(f"position from server: {wifi_name}")
            SendMail(wifi_name)
        else:
            print("position not found in the server response.")
    except Exception as err:
        print(f"An error occurred: {err}")

if __name__ == "__main__":
    get_position_from_server()
