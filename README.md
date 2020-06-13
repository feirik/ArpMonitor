### Background
The purpose of the program is to automatically monitor the ARP cache on Windows 10. This can be useful for learning how the ARP cache works or is affected by ARP attacks such as MAC changers and ARP poisoning.

![Example output](https://github.com/feirik/ArpMonitor/blob/master/Images/ArpMonitor_readme.png)

### Command line options

#### -i --interface <INTERFACE IP ADDRESS> (optional)
Selects an interface in the arp -a output for the program to monitor

#### -d --delay <NUMBER> (optional)
Sets the delay in seconds between monitoring cycles for the program. Default delay is 5 seconds if not specified.

#### -o --output <FILEPATH> (optional)
Filepath of the output log file. Default filepath is in the directory of the .exe.

#### -l --logonly (optional flag)
Disables event output to console.

#### -p --passive (optional flag)
Disables any network traffic by the program. The network traffic is used to retrieve the domain name of an ARP entry through nslookup.

### ARP events
The program works by comparing a old snapshot of the ARP cache with the present one. The ARP events are divided into the following:
- Regular ARP entry, new IP address broadcasting on new MAC address
- Multi-IP ARP entry, multiple IP addresses broadcasting on one MAC address
- New IP address broadcasting on old MAC address
- Old IP address broadcasting on new MAC address

Additionally, the ARP events are classified based on when they appear:
- ARP entry at start-up
- New ARP entry while program is running
- Elapsed ARP entry while program is running

### Data sources
The "arp -a" command is used as a source for gathering the MAC address and IP address for entries. An Organizationally unique identifier (OUI) list is used for showing the manufacturer connected to a MAC address. A modified nslookup script is used for retrieving the domain name of an entry.








