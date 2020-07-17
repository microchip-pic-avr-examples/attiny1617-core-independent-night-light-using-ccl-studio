==========
AC driver
==========

The Analog Comparator (AC) compares the voltage levels on two inputs and gives a digital output based
on this comparison. The AC can be configured to generate interrupt requests and/or Events upon several
different combinations of input change.
The dynamic behavior of the AC can be adjusted by a hysteresis feature. The hysteresis can be
customized to achieve optimal operation for each application.
The input selection includes analog port pins, internal references and DAC output. The analog
comparator output state can also be output on a pin for use by external devices.

Features
--------
* Initialization
* Callback on comparison interrupt
* Multiple comparators per hardware instance supported

Applications
------------
* Monitor if sensor/battery voltage exceeds the reference voltage.
* Monitor two sensor voltage, compare which is higher.

Dependencies
------------
* CLKCTRL/CLK for clock
* CPUINT/PMIC for Interrupt
* JTAG/PDI/UPDI for debug
* PORT for I/O Lines and Connections
* EVSYS for Event

Note
----
* ISR will be generated only when Global Interrupt checkbox and driver_isr Harness checkbox are enabled

Concurrency
-----------
N/A

Limitations
-----------
N/A

Knows issues and workarounds
----------------------------
N/A

