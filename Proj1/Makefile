.PHONY: stucs clean

stucs: main.o list.o l.o table.o
	g++ -o stucs main.o l.o list.o table.o
	rm *.o
main.o: main.cc header.h
	g++ -c main.cc
list.o: list.cc header.h
	g++ -c list.cc
l.o: l.cc header.h
	g++ -c l.cc
table.o: table.cc header.h
	g++ -c table.cc
clean:
	rm stucs
