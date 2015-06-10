XBee Setup

Entering `+++` at the default baud rate of 9600 should result in an `OK` prompt. Commands can then be sent to the XBee.
All commands are prefixed with `AT`.

_(Note that the `AT` prefix is not shown below for clarity.)_

Enter command mode

	+++

Reset to defaults

	RE

PAN ID (Network ID, should be the same for all XBee nodes on each personal network)
	
	ID 0510


Enable API mode (use packets instead of just passing through data transparently)

	AP 1


Set 16-bit node address (this should be unique for each node on a given network)
I'm using addresses that start with "32" and then a two-digit serial number (e.g. 3201).

	MY <0000-FFFF>


Write settings to NV memory

	WR
