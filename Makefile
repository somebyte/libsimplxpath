CPP=g++
VPATH=src/lib:src/example
LIBNAME=libsimplxpath

all: lib xmltest

%.o: %.cpp 
	${CPP} -c $< -Iinclude -O2 -fPIC -o $@

lib: simplxpath.o simplxpath_impl.o
	${CPP} $^ -shared -o ${LIBNAME}.so

xmltest: xmltest.o
	${CPP} $^ -o $@ -L./ -lsimplxpath -lxml2

clean:
	rm -v *.o

distclean: 
	rm -v *.o ${LIBNAME}.so xmltest default.xml xmltest.xml


.PHONY: lib xmltest clean distclean
