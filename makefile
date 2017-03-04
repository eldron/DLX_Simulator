normalizer:
	gcc -c instruction.c mem.c register.c simulator.c
	gcc instruction.o mem.o register.o simulator.o -o simulator

clean:
	rm -f normalizer
	rm -f *.o