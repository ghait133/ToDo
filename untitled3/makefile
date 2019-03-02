cc = g++

all: clean
	$(cc) -o bin/program.bin main.cpp -lcurses -lmenu -lform -g

program: clean obj/drivers.o
	$(cc) -o bin/program.bin obj/drivers.o main.cpp -lcurses -lmenu -lform

obj/drivers.o:
	$(cc) -c Drivers.cpp -o obj/drivers.o -lcurses -lmenu -lform

clean:
	rm -rf ./bin
	rm -rf ./obj
	mkdir -p obj
	mkdir -p bin
