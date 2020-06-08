# legendary-dashboard - Developer Notes

## BME280 Environment Sensor

Things to keep in mind:

1. Despite several wiring diagrams on the web, the ESP32-Wire-Library for TTGO-T1 board uses pins 21 (SDA) and 23 (SCL) by default.

Sample logic-trace with 100kHz I²C-frequency:

![I2C-Trace](docs/I2C-BME280-logictrace.png)

## Waveshare e-Paper Display and SIM800L T-Call ESP32 

The actual pinning for the SPI connection depends on

1. The available pins on the board. Refer to the [schematic](docs/TTGO_T-Call_SIM800_v1.3_schematic.pdf)
2. Hard-coded pins (e.g. MOSI, SCK) in [GxEPD E-Paper display library](https://github.com/ZinggJM/GxEPD)
3. Pins that are suitable as Input/Output. See [ESP32 Pinout Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)

e-Paper Module | ESP32 | Color | Function | Note
-|-|-|-|-
`VCC`  | `3V3` | red |  3.3V | 
`GND`  | `GND` | black |  GND |
`DIN`  | `GPIO 15` | blue |  SPI MOSI | hard-coded in GxEPD-Lib
`CLK`  | `GPIO 14` | yellow |  SPI SCK | hard-coded in GxEPD-Lib
`CS`  | `GPIO 19` | orange |  SPI chip select (Low active) |
`DC`  | `GPIO 18` | green |  Data/Command control pin | High for data, and low for command
`RST`  | `GPIO 0` | white |  External reset pin | Low for reset
`BUSY`  | `GPIO 2` | purple |  Busy state output pin | Low for busy

## Streaming display-content via webcam 

Remote developers using [PIO Remote](https://docs.platformio.org/en/latest/plus/pio-remote.html) can be supported by using the Rasperry Pi camera to stream a video of the e-Paper display and the board itself (to check on LEDs) in their browser:

![camerasteam](https://i.ibb.co/G90PcZh/camerasteam.gif)

Sourcecode from [Video Streaming with Raspberry Pi Camera](https://randomnerdtutorials.com/video-streaming-with-raspberry-pi-camera/):

```Python
# Web streaming example
# Source code from the official PiCamera package
# http://picamera.readthedocs.io/en/latest/recipes2.html#web-streaming

import io
import picamera
import logging
import socketserver
from threading import Condition
from http import server

PAGE="""\
<html>
<head>
<title>Raspberry Pi - Remote Developer Camera</title>
</head>
<body>
<center><h1>Raspberry Pi - Remote Developer Camera</h1></center>
<center><img src="stream.mjpg" width="640" height="480"></center>
</body>
</html>
"""

class StreamingOutput(object):
    def __init__(self):
        self.frame = None
        self.buffer = io.BytesIO()
        self.condition = Condition()

    def write(self, buf):
        if buf.startswith(b'\xff\xd8'):
            # New frame, copy the existing buffer's content and notify all
            # clients it's available
            self.buffer.truncate()
            with self.condition:
                self.frame = self.buffer.getvalue()
                self.condition.notify_all()
            self.buffer.seek(0)
        return self.buffer.write(buf)

class StreamingHandler(server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(301)
            self.send_header('Location', '/index.html')
            self.end_headers()
        elif self.path == '/index.html':
            content = PAGE.encode('utf-8')
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.send_header('Content-Length', len(content))
            self.end_headers()
            self.wfile.write(content)
        elif self.path == '/stream.mjpg':
            self.send_response(200)
            self.send_header('Age', 0)
            self.send_header('Cache-Control', 'no-cache, private')
            self.send_header('Pragma', 'no-cache')
            self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
            self.end_headers()
            try:
                while True:
                    with output.condition:
                        output.condition.wait()
                        frame = output.frame
                    self.wfile.write(b'--FRAME\r\n')
                    self.send_header('Content-Type', 'image/jpeg')
                    self.send_header('Content-Length', len(frame))
                    self.end_headers()
                    self.wfile.write(frame)
                    self.wfile.write(b'\r\n')
            except Exception as e:
                logging.warning(
                    'Removed streaming client %s: %s',
                    self.client_address, str(e))
        else:
            self.send_error(404)
            self.end_headers()

class StreamingServer(socketserver.ThreadingMixIn, server.HTTPServer):
    allow_reuse_address = True
    daemon_threads = True

with picamera.PiCamera(resolution='640x480', framerate=24) as camera:
    output = StreamingOutput()
    #Uncomment the next line to change your Pi's Camera rotation (in degrees)
    #camera.rotation = 90
    camera.start_recording(output, format='mjpeg')
    try:
        address = ('', 8000)
        server = StreamingServer(address, StreamingHandler)
        server.serve_forever()
    finally:
        camera.stop_recording()
```
Use the following systemd-config to enable background 
```INI
[Unit]
Description=camerastream
After=network.target

[Service]
User=henry
Type=simple
WorkingDirectory=/home/henry
ExecStart=python3 stream.py
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

```Bash
sudo cp camerastream.service /etc/systemd/system
sudo systemctl start camerastream.service
```
