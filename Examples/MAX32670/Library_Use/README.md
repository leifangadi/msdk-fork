## Description

Simple program that demonstrates how to link static libraries to an application.

This demo application toggles an LED using GPIO functions found in the static library myLib.a. Refer
to project.mk to see how to include ".a" static library files into your project.

## Software

### Project Usage

Universal instructions on building, flashing, and debugging this project can be found in the **[MSDK User Guide](https://analog-devices-msdk.github.io/msdk/USERGUIDE/)**.

### Project-Specific Build Notes

(None - this project builds as a standard example)

## Required Connections

-   Connect a USB cable between the PC and the CN1 (USB/PWR) connector.
-   Select RX0 and TX0 on Headers JP1 and JP3 (UART 0).
-   Close jumper JP1 (LED1 EN).
-   Close jumper JP2 (LED2 EN).
-   Open a terminal application on the PC and connect to the EV kit's console UART at 115200, 8-N-1.

## Expected Output

The Console UART of the device will output these messages:

```
********************** Static Library Example **********************

This example calls static library functions to toggle an LED.

...
```
