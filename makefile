client:
	gcc client.c functions.c -o client	
   
server:
	gcc server.c functions.c -o server
   
clean:
	rm -f client server
