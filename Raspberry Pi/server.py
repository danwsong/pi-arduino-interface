from flask import Flask
from flask import request
import serial
import struct

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

app = Flask(__name__)

@app.route("/api")
def drive():
    print("API request received.")
    if "val" in request.args:
        val = int(request.args["val"])
        ser.write(struct.pack('>B', val))
    return ""

if __name__ == "__main__":
    if setupComplete:
        app.run(host="0.0.0.0", port=80)
