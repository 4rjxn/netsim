CC     = gcc
CFLAGS = -Wall -Wextra -g
SRCS   = main.c graph.c ui.c
OBJS   = $(SRCS:.c=.o)

netsim: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o netsim

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

run: netsim
	./netsim

clean:
	rm -f $(OBJS) netsim

.PHONY: run clean
