CC=g++
INCLUDE=-Ilshkit-0.2.1/include/ -lgsl -lgslcblas -L. -llshkit
CFLAGS=-Wall -fPIC

all: luash.so

temp:  
	mkdir temp

lshkit:
	$(MAKE) -C lshkit-0.2.1/build
	cp lshkit-0.2.1/build/src/liblshkit.so .

temp/mplsh-fit-tune.o: lshkit temp
	$(CC) -c mplsh-fit-tune.cpp -o $@ ${CFLAGS} ${INCLUDE}

temp/luash.o: lshkit temp/mplsh-fit-tune.o temp
	$(CC) -c luash.cpp -o $@ ${CFLAGS} ${INCLUDE}

luash.so: lshkit temp/luash.o 
	$(CC) -shared -o $@ temp/*.o ${INCLUDE}

clean:
	rm -rf temp
	rm luash.so
