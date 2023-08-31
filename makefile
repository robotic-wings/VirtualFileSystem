CC = gcc
CFLAGS = -o

all: vfs
vfs: utils.c interface.c filesystem.c vfs.c
	$(CC) $(CFLAGS) vfs utils.c filesystem.c interface.c vfs.c
clean:
	rm vfs