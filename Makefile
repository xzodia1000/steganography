# Options to pass to the C compiler
CFLAGS = -std=c99 -O2 -Wall -pedantic -g

# List of programs to build
TARGETS = steg

# Default rule, built if you just say "make"
all: $(TARGETS)

# Build a program
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Remove built programs and temporary files
clean:
	rm -f $(TARGETS) *~
