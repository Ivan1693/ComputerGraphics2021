TARGET=objppmx
SRCDIR=src
OBJDIR=obj
INCDIR=inc

OBJS= $(subst src,obj,$(subst .c,.o,$(wildcard $(SRCDIR)/*.c)))

CC=gcc
CFLAGS=-g -Wall -I./$(INCDIR)
LDFLAGS=-lm -lpthread

all: $(TARGET)

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) -c $< $(CFLAGS) -o $@

$(TARGET): $(OBJS)
	@echo $(OBJS)
	$(CC) $^ -o $(TARGET) $(LDFLAGS)

.PHONY: clean pclean

clean:
	rm -fv obj/*.o $(TARGET)
pclean:
	rm ppm/*.ppm
