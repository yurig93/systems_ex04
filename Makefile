IDIR =./
CC=gcc
CFLAGS=-Wall -I .

ODIR=.

LIBS=-lm

TARGET_FREQUENCY = frequency

# Generic target for objects
$(ODIR)/%.o: %.c $(DEPS_LIB_BANK)
	$(CC) -fpic -c -o $@ $< $(CFLAGS)

$(TARGET_FREQUENCY): $(TARGET_FREQUENCY).o
	$(CC) -o $@ $^ $(CFLAGS)

all: $(TARGET_FREQUENCY)

.PHONY: clean

clean:
	rm -f $(ODIR)/frequency $(ODIR)/*.o $(ODIR)/*.so $(ODIR)/*.a *~ $(INCDIR)/*~