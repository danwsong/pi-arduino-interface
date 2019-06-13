# imports flask server library
from flask import Flask
from flask import request
# import serial library for usb communication with the arduino
import serial
# import struct library to pack data to send over serial
import struct


# check if arduino is connected by checking each of the 2 usb ports
# if not connected, send an error message and exits code
setupComplete = False
try:
    ser = serial.Serial("/dev/ttyUSB0")
    setupComplete = True
except:
    try:
        ser = serial.Serial("/dev/ttyUSB1")
        setupComplete = True
    except:
        print("ERROR: Arduino is not connected.")

# create flask server
app = Flask(__name__)
# route that will run the sendData function when "/api" is accessed
@app.route("/api")
def sendData():
    # pre:  /api is accessed
    # post: sends requested data to arduino

    print("API request received.")
    # error checking that makes sure to send data if there is a value requested
    if "val" in request.args:
        # if there is a value, store it as an integer and send it to the arduino
        val = int(request.args["val"])
        ser.write(struct.pack('>B', val))
    return ""

# if the program is running, start up the server on this computer (ip address of 0.0.0.0 means the computer the program is running on)
if __name__ == "__main__":
    if setupComplete:
        app.run(host="0.0.0.0", port=80)
