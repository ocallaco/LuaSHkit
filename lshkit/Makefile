CC=g++
INCLUDE=-Ilshkit-0.2.1/include/ -lgsl -lgslcblas
CFLAGS=-Wall -fPIC

all: luash.so

temp:  
	mkdir temp

lshkit:
	$(MAKE) -C lshkit-0.2.1/build
	cp lshkit-0.2.1/build/src/liblshkit.so .

temp/mplsh-fit-tune.o: temp
	$(CC) -c mplsh-fit-tune.cpp -o $@ ${CFLAGS} ${INCLUDE}

temp/luash.o: temp/mplsh-fit-tune.o temp
	$(CC) -c luash.cpp -o $@ ${CFLAGS} ${INCLUDE}

luash.so: temp/luash.o lshkit
	$(CC) -shared -o $@ temp/*.o ${INCLUDE}

clean:
	rm -rf temp
	rm luash.so
