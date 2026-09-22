# Makefile for simple test harness

TESTAPP_OBJS = miby.o testapp.o
PARSER_TEST_OBJS = miby_parser_tests.o parser_tests.o

CFLAGS ?= -g
CPPFLAGS = -I../src -I.
WARNFLAGS = -Wall -Wextra -Werror

.PHONY: all test clean

all: testapp

testapp: $(TESTAPP_OBJS)
	$(CC) $(TESTAPP_OBJS) -o $@
	
miby.o: ../src/miby.c
	$(CC) $(CPPFLAGS) -DMIBY_USER_CONFIG=\"testapp.h\" $(CFLAGS) -c $< -o $@ 
	
testapp.o: testapp.c
	$(CC) $(CPPFLAGS) -DMIBY_USER_CONFIG=\"testapp.h\" $(CFLAGS) -c $< -o $@

miby_parser_tests.o: ../src/miby.c parser_tests.h
	$(CC) $(CPPFLAGS) -DMIBY_USER_CONFIG=\"parser_tests.h\" $(CFLAGS) $(WARNFLAGS) -c $< -o $@

parser_tests.o: parser_tests.c parser_tests.h
	$(CC) $(CPPFLAGS) -DMIBY_USER_CONFIG=\"parser_tests.h\" $(CFLAGS) $(WARNFLAGS) -c $< -o $@

parser_tests: $(PARSER_TEST_OBJS)
	$(CC) $(PARSER_TEST_OBJS) -o $@

test: parser_tests
	./parser_tests

clean:
	rm -f $(TESTAPP_OBJS) $(PARSER_TEST_OBJS) testapp parser_tests
