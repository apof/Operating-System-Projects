SOURCE	= mydiz.cc block.cc archive.cc List.cc dir_funcs.cc error.cc
HEADER  = block.h archive.h List.h dir_funcs.h error.h
OBJS 	= mydiz.o block.o archive.o List.o dir_funcs.o error.o
OUT  	= mydiz
CC		= g++
FLAGS   = -g -c
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)
	rm -f $(OBJS)

mydiz.o: mydiz.cc
	$(CC) $(FLAGS) mydiz.cc

block.o: block.cc block.h
	$(CC) $(FLAGS) block.cc

archive.o: archive.cc archive.h
	$(CC) $(FLAGS) archive.cc

List.o: List.cc List.h
	$(CC) $(FLAGS) List.cc

dir_funcs.o: dir_funcs.cc dir_funcs.h
	$(CC) $(FLAGS) dir_funcs.cc

error.o: error.cc error.h
	$(CC) $(FLAGS) error.cc

clean:
	rm -f $(OUT)

count:
	wc $(SOURCE) $(HEADER)