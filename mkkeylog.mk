keylog: keylog.o
	gcc keylog.o -o keylog
keylog.o: keylog.c
	gcc -c keylog.c

clean:
	rm *.o keylog