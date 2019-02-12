.PHONY: restaurant clean

restaurant: main.cc doorman.cc
	g++ -o restaurant main.cc -pthread -lrt
	g++ -o doorman doorman.cc -pthread -lrt
	g++ -o customer customer.cc -pthread -lrt
	g++ -o waiter waiter.cc -pthread -lrt
 
clean:
	rm restaurant
	rm doorman
	rm customer
	rm waiter
