# protocol for the arduino pi communication

## the commands prefixes

The protocol will use http like commands.  

3 - command  
2 - status  
1 - data  
7 - get information  
5 - get quick peek  
6 - send big data

After the command prefix will come the id of the command. For example  
201 - get status from the first sensor


## the protocol base

The arduino will update the pi when it want,  
they pi will receive the data and log it.

It is possible for the pi to ask the arduino  
for data.

### the steps of the protocol

1. The arduino will send in one of the regular check's that it wants to send data to the raspberry pi

2. The raspberry pi will acknowledge the request

3. The raspberry pi will ask the arduino to start sending the data

4. The arduino will send the data in small chunks with a check sum value in the end of the string

5. The raspberry pi will calculate the checksum

6. The raspberry pi will compare both of the checksums to see if nothing got lost in the middle

7. If the check sums are equal the raspberry pi will ask for the next one

8. Steps 4-7 will be repeated until one of the messages will contain
