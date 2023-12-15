all:
	clang++ *.cpp -lpthread -O3 -o domino
gdb:
	clang++ *.cpp -g
gprof:
	clang++ *.cpp -O3 -pg -o domino
clean:
	rm -f *.bin
	rm -f domino
	rm -f domino.exe
	rm -f gmon.out
