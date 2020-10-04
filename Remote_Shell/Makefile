SRC = $(wildcard *.c)               # list of source files
OBJS = $(patsubst %.c, %.o, $(SRC)) # list of object files
objs : $(OBJS)                      # target

clean :
	-rm *.out $(OBJS)