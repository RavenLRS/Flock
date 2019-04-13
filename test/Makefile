CFLAGS += -g
GTEST_DIR = googletest/googletest
LIBGTEST = libgtest.a
INCLUDES = -isystem $(GTEST_DIR)/include -I$(GTEST_DIR)
ALL_TESTS = $(patsubst %.test.cpp,%,$(wildcard *.test.cpp))

LIBFLOCK_DIR = ../src/libflock
LIBFLOCK_SOURCES = $(wildcard $(LIBFLOCK_DIR)/*.c)
LIBFLOCK_CFLAGS = -I$(LIBFLOCK_DIR)

all: $(ALL_TESTS)

$(LIBGTEST):
	$(CXX) -std=c++11 $(INCLUDES) -pthread \
	    $(CFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc
	$(AR) -rv $@ gtest-all.o

%.test: %.test.cpp $(LIBFLOCK_SOURCES) $(LIBGTEST)
	$(CXX) -std=c++11 $(INCLUDES) $(CFLAGS) $(LIBFLOCK_CFLAGS) \
	    $(shell grep "^//CFLAGS:" $< |cut -d: -f 2) \
	    -o $@ $< $(LIBFLOCK_SOURCES) \
	    gtest-main.cpp $(LIBGTEST)

%: %.test
ifneq ($(TEST_FILTER),)
	./$< --gtest_filter=$(TEST_FILTER)
else
	./$<
endif


clean:
	$(RM) -r *.dSYM *.a *.o *.test
