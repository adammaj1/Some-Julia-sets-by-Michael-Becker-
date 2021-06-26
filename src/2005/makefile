kCC=/usr/bin/gcc
OBJ = complex.o imagearea.o zero.o cpoly.o julia2.o
BIN = julia2
SRC = julia2.c
IDIRS = -I /usr/include/glib-1.2 -I /usr/include/gtk-1.2 -I /usr/lib/glib/include
#CPRE_MACROS = -DZERO_DEBUG

CFLAGS = -Wall -g $(IDIRS) $(CPRE_MACROS)
LDFLAGS = `gtk-config --cflags` `gtk-config --libs` -lgthread
all: $(OBJ) $(BIN)

julia2: $(OBJ) 
	$(CC) $(CFLAGS) -o julia2 $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean::
	rm -rf $(OBJ) $(BIN)

