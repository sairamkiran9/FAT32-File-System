BINS = shell
C = gcc
CFLAGS = -std=gnu99 -Wall -pedantic -g

all: $(BINS)

#files included for shell execution
shell: main.c utils.c execute.c execute_ls.c execute_cd.c \
		execute_size.c execute_creat.c execute_mkdir.c execute_rm.c \
		execute_rmdir.c execute_read.c execute_open.c execute_close.c execute_rename.c
	$(C) $(CFLAGS) -o shell main.c utils.c execute.c execute_ls.c \
		execute_cd.c execute_size.c execute_creat.c execute_mkdir.c \
		execute_rm.c execute_rmdir.c execute_read.c execute_open.c execute_close.c \
		execute_rename.c

#removes (cleans) executables
clean:
	rm $(BINS)

