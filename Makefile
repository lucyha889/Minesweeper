all: compile link

compile:
	g++ -Isrc/include -c ./*.cpp

link:
	g++ *.o -o sfmlMsGame -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm *.o
	rm *.exe

run:
	./sfmlMsGame