.POSIX:
.SUFFIXES:

CXXFLAGS=--std=c++11 -W -O
GOOGLE_TEST_LIB = gtest
LDLIBS=-lm -l$(GOOGLE_TEST_LIB) -lpthread
INCLUDES=include

all: tests
tests: avl_tree_tests
win32: tests
	ren tests\all test\all.exe

%_tests: obj/%_tests.o
	mkdir -p build/tests
	$(CXX) $(LDFLAGS) $(LDLIBS) -o build/tests/$* $^

obj/%_tests.o: tests/%_tests.cpp include/%.hpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) $(LDLIBS) -c $< -o $@

clean:
	$(RM) -r build
	$(RM) -r obj

.PHONY: all tests clean