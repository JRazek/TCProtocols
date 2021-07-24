TCP Server 
to run server simply create the TCPServer object.<br/>
Before the run you must specify the listeners for the server. <br/>
to add listener use the .addListener() method which will init the listener for run<br/>
when the server is ready - simply call .run() method.<br/>
From this time server will start listening for any new incoming connections. <br/>
After the successful handshake. Listener creates new thread with socket responsible for that connection.

Data is passed in following format - <br/>
first 8 bytes specifies the number of packets that are going to be sent.<br/>
bytes are encoded in decreasing order - first byte is most significant.<br/>

after this socket is will be expecting n specified packets of data.<br/>
Each packet starts with 8 bytes including the length(bytes) of the packet sent.<br/>
Same encoding as with the packet count.<br/>

For simplicity the TransferObject class has been created with all the necessary encoding methods.<br/>