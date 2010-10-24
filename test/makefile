
OBJS = miby.o testapp.o

CFLAGS = -DMIBY_USER_CONFIG=\"testapp.h\" -I../src -I.

testapp: $(OBJS)
	$(CC) $(OBJS) -o $@
	
miby.o: ../src/miby.c
	$(CC) $(CFLAGS) -c $< -o $@ 
	
testapp.o: testapp.c
	$(CC) $(CFLAGS) -c $< -o $@
