TARGET=main
SRC=$(TARGET).c
INCDIR=../objppmx/inc
CC=gcc
CFLAGS=-g -Wall -I./$(INCDIR)
LDFLAGS=-lm -lpthread

all: $(TARGET)

build:
	@echo "Building at: ../objppmx"
	if [ ! -n "$(ls -A ../objppmx/obj/*.o 2>/dev/null)" ]; then \
	cd ../objppmx/ ; \
	make ; \
	fi
$(TARGET): build
	$(CC) ../objppmx/obj/*.o $(SRC)  -o $(TARGET) $(CFLAGS) $(LDFLAGS)

.PHONY: clean ppmclean

ppmclean:
	rm *.ppm
clean:
	rm $(TARGET)
