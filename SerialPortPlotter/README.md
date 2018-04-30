# SerialPortPlotter
A server that visualizes data from the microcontroller.
The application reads every two bytes from serial port and draws values on the plot.

#### How to build?
1. Clone the repository
2. Enter the _SerialPortPlotter_ folder
3. Run `make` in the terminal. _Golang_ must be installed.

#### How to use?
1. Power ON device.
2. Connect usb-uart converter to the computer.
3. Run a server as the sudo-user. Check port of usb-uart converter.
`./plotter -s USB_UART_PORT -h localhost -p 3000`
```
Usage of ./plotter:
  -h string
        host to serve on (default "localhost")
  -p string
        port to serve on (default "3000")
  -s string
        serial port (default "/dev/ttyUSB0")
```
4. Enter the url from the application in your web browser. Default is http://localhost:3000.