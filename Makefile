all: client server

client:
	g++ Client.cpp -o Client.app

server:
	g++ Server.cpp -o Server.app

clean:
	rm -rf Client.app Server.app
