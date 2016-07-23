all:
	g++ client.cpp -o client
run:
	./client &
kill:
	killall client
e:
	nano -c client.cpp
