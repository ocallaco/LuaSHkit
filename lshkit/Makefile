CC=g++
INCLUDE=-Ilshkit-0.2.1/include/
CFLAGS=-Wall -fPIC

all: luash.so

temp:  
	mkdir temp

lshkit:
	$(MAKE) -C lshkit-0.2.1/build
	cp lshkit-0.2.1/build/src/liblshkit.so .

temp/luash.o: temp
	$(CC) -c luash.cc -o $@ ${CFLAGS} ${INCLUDE}

luash.so: temp/luash.o lshkit
	$(CC) -shared -o $@ temp/luash.o 

clean:
	rm -rf temp
	rm luash.so
