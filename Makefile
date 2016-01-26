#
# Makefile:

DEBUG	= -g -O0
#DEBUG	= -O3
CC	= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

SRC	= ltc2309.c
OBJ	= $(SRC:.c=.o)
BINS = $(SRC:.c=)

all: $(BINS)

ltc2309: ltc2309.o
	@echo [link]
	@$(CC) -o $@ ltc2309.o $(LDFLAGS) $(LDLIBS)

.c.o:
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@echo "[Clean]"
	@rm -f $(OBJ) *~ core tags $(BINS)

tags:	$(SRC)
	@echo [ctags]
	@ctags $(SRC)

depend:
	makedepend -Y $(SRC)

install: ltc2309
	@echo [install]
	@sudo cp ltc2309 /usr/local/bin

