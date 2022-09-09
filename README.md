# ovpn-tracker

This is a simple tracker for the Open VPN server to collect usage statistics for further analysis. 
Currently it's under extensive development and many things are hard-coded rather than configured properly

The main idea is to connect to Open VPN server via Management interface (plain text, telnet-like) and retrieve CURRENT usage statistics. 
Doing so periodically and storing the data in the RDBMS (MySQL) allows to see a bigger picture on usage statistics beyond the current status.
The same information is available on status.log, and the [rpgram can take it from there too - but it's limited to only one server.

With the data collected it's possible to see activity from a spcific CN, IP address, specific times, how much traffic has been consumed both ways, 
and much much more. This program is set only to COLLECT the data (based on current status) and store it in a RDBMS. Further analysis 
including visualization is beyond the scope at this moment. It can easily be some with MySQL Workbench, SQL queries and other tools. 

For example, plain data may look like this:

id,cn,bytes_in,bytes_out,session_since,updated,ip4,network_name
'19544','rodios-bk-ru_p4pn-virginia1','2790','2741','2022-09-08 04:07:51','2022-09-08 04:07:52','37.112.24.191','pastushkov.com'
'19431','aleshaivanov1993-gmail-com_p4pn-virginia1','203655','286887','2022-09-08 04:02:42','2022-09-08 04:07:52','46.146.163.15','pastushkov.com'
'19540','vvlada-boldyreva-mail-ru_p4pn-virginia1','2903','2995','2022-09-08 04:07:43','2022-09-08 04:07:52','94.233.236.121','pastushkov.com'
'19539','UNDEF','54','282','2022-09-08 04:07:08','2022-09-08 04:07:52','213.87.126.182','pastushkov.com'
'19529','favika-bk-ru_p4pn-virginia1','10482','20947','2022-09-08 04:06:18','2022-09-08 04:07:52','94.25.164.51','pastushkov.com'
'18453','arkochetov-yandex-ru_p4pn-virginia1','83664616','2054812454','2022-09-08 03:28:31','2022-09-08 04:07:52','94.29.9.153','pastushkov.com'
'19575','tumenamx24-mail-ru_p4pn-virginia1','425446','11283405','2022-09-08 04:08:03','2022-09-08 04:08:52','89.250.149.53','pastushkov.com'

