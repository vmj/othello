
CC:=gcc

SRC = $(wildcard *.c)
HDR = $(filter-out $(SRC:.c=.h),$(wildcard *.h))
OBJ = $(SRC:.c=.o)
LDFLAGS = -lglut

all: othello

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

othello: $(HDR) $(OBJ)
	$(CC) $(LDFLAGS) -o othello $(OBJ)

clean:
	-@rm *~ *.o 2>/dev/null || true

distclean: clean
	-@rm othello 2>/dev/null || true
