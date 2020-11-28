# protocol for the arduino pi communication

TODO add the checksum

## the protocol base

the protocol will use http like commands.  
the http protocol is used to transfer data between 2 computers.

## the commands prifixes

3 - command  
2 - status  
1 - data  
4 - get information  
5 - get quick peek
6 - send big data

after the command prifix will come the id of the command. for example  
201 - get status from the first sensor

## the big data protocol

if the arduino wants to dump an array of data that it gettered  to the raasbery pi it will use this protocol.  
the arduino will send after one of the regular checkups that  it wants to send the data.  

### the steps of the protocol

1. the arduino will send in one of the regulag checkus that it wants to send data to the rassbery pi

2. the rassbery pi will acnolege the request

3. the rassbery pi will ask the arduino to start sending the data

4. the arduino will send the data in small chunks with a check sum value in the end of the string

5. the rassbery pi will calculate the checksum

6. the rassbery pi will compare both of the checksums to see if nothing got lost in the middle

7. if the check sums are equal the rassbery pi will ask for the next one

8. steps 4-7 will be repeted until one of the messages will contain
