cc = gcc
CFLAGS = -Wall -g

%.o: %.c
	@$(CC) -c -o $@ $<

main: main.o logger/logger.o
	@$(CC) $(CFLAGS) -o $@.exe $^
	@./$@.exe

clean:
	@find . -type f -name "*.o" -delete
	@find . -type f -name "*.exe" -exec rm -f {} +