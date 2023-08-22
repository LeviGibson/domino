all:
	clang++ *.cpp -O3 -o domino
gdb:
	clang++ *.cpp -g
clean:
	rm -f *.bin
	rm -f domino
	rm -f domino.exe
	rm -f gmon.out
