CC = gcc
LD = gcc

CFLAGS=-std=gnu99


all: oled_output

oled_output: oled_output.o oled.o
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf oled_output oled*.o
