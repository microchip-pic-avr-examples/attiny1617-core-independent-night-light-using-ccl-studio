======================
SPI driver
======================
Serial Peripheral Interface(SPI) driver Full duplex, Three-wire Synchronous Data Trasnfer in Master or Slave mode of operation.
Peripheral is clocked by the peripheral clock and supports seven programmable bit rated. Optional Clock double option is also available in Master mode

Features
--------
* Initialization

Applications
------------
* Fast communication between an AVR device and peripheral devices or several microcontrollers.

Dependencies
------------
* CLKCTRL/CLK for clock
* CPUINT/PMIC for Interrupt
* PORT for I/O Lines and Connections
* UPDI/PDI/JTAG for debug

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
