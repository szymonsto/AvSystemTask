# Av System recrutation task
## Features

 - application shares information about
 	- current system time
 	- total disc space and free disc space
 	- IP address used to Internet communication
 	- current CPU temperature with units
 	- client's device errors
 	- UTC offset
 	- Timezone
 	- supported binding and modes
 - server can track clients' device system time and CPU temperaure by using notifications
 - reboots client's device from server
 - clears errors from server
 - specyfyies endpoint name and server's URL address
 - changes timezone and UTC offset from server
 - secure conncetion by using Pre-Shared key
 
 ## Building
 
 	cmake . && make && sudo ./anjay-task <endpoint name> <server URL address> <password>
 	
 
 ### Example
 
 	cmake . && make && sudo ./anjay-task urn:dev:os:anjay_task try-anjay.avsystem.com:5684 password
 	
 	
	### Author: Szymon Stolarski

