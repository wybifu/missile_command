DEBUG=yes
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
EXEC=../Linux_version

CLIB=-Wall -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm

ifeq ($(DEBUG),yes)
	CFLAGS=-Wall  -c -g 
else
	CFLAGS=-Wall  -c -O3
endif

all:$(EXEC) 
ifeq ($(DEBUG),yes)
	@echo "Debug mode"
else 
	@echo "Release mode"
endif

$(EXEC): $(OBJ)
	gcc -o $@ $^ $(CLIB)

%.o: %.c
	gcc $(CFLAGS) $*.c

clean:
	rm -f $(OBJ)

