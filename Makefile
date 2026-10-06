CC = gcc
CFLAGS = -Wall -Wextra -g

build:
	$(CC) $(CFLAGS) main.c dispatch_system.c -o dispatch_sim

clean:
	rm -f dispatch_sim dispatch.out