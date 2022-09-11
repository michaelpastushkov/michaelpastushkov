# ovpn-tracker

This is a simple tracker for the Open VPN server to collect usage statistics for further analysis. 
Currently it's under extensive development and many things are hard-coded rather than configured properly

The main idea is to connect to Open VPN server via Management interface (plain text, telnet-like) and retrieve CURRENT usage statistics. 
Doing so periodically and storing the data in the RDBMS (MySQL) allows to see a bigger picture on usage statistics beyond the current status.
The same information is available on status.log, and the [rpgram can take it from there too - but it's limited to only one server.

With the data collected it's possible to see activity from a spcific CN, IP address, geo, specific times, how much traffic has been consumed both ways, 
and much much more. This program is set only to COLLECT the data (based on current status) and store it in a RDBMS. Further analysis 
including visualization is beyond the scope at this moment. It can easily be some with MySQL Workbench, SQL queries and other tools. 
