all:
	g++ *.cpp -O3 -o domino -pg
clean:
	rm -f *.bin
	rm -f domino
	rm -f domino.exe
	rm -f gmon.out
