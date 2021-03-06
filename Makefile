.POSIX:
.SUFFIXES:

CXXFLAGS=--std=c++11 -W -O
GOOGLE_TEST_LIB = gtest

LDLIBS_MAIN=-lm
LDLIBS_TESTS=-lm -l$(GOOGLE_TEST_LIB) -lpthread

INCLUDES=include

all: interactive #tests
interactive: obj/main.o
	mkdir -p bin
	$(CXX) $(LDFLAGS) $(LDLIBS_MAIN) -o build/avl_tree $^

#tests: avl_tree_tests
#win32: tests
#	ren tests\all test\all.exe

#%_tests: obj/%_tests.o
#	if not exist build/tests mkdir build/tests
#	$(CXX) $(LDFLAGS) $(LDLIBS_TESTS) -o build/tests/$* $^

obj/%_tests.o: tests/%_tests.cpp include/%.hpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) $(LDLIBS) -c $< -o $@

obj/main.o: main.cpp include/avl_tree.hpp
	mkdir -p build
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) $(LDLIBS) -c $< -o $@

clean:
	$(RM) -r build
	$(RM) -r obj

.PHONY: all interactive clean #tests