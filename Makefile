OBJS = TolA_10_P4_main.o TolA_10_P4_class.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

p4: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o p4

TolA_10_P4_main.o: TolA_10_P4_main.cpp TolA_10_P4_class.h
	$(CC) $(CFLAGS) TolA_10_P4_main.cpp

TolA_10_P4_class.o: TolA_10_P4_class.cpp TolA_10_P4_class.h
	$(CC) $(CFLAGS) TolA_10_P4_class.cpp

clean:
	\rm *.o p4

run: p4
	./p4