# traceroute

Program which traces the route from your computer to a given address by sending
ICMP packets with increasing TTLs.

## How it works

The program uses raw sockets to send ICMP packets to the given address. 
There are three packets per each time-to-live going from 1 to 30. When packet's
TTL is 0, the receiving router will send you a time exceeded packet. This
way it is possible to track what routers were on the route to the final address.

## Requirements

- Linux
- make

## How to run

- First compile the program with `make`.
- Run `./traceroute «address»` **as a root**. The address must be an IPv4 address.
