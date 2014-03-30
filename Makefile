SRCDIR := src
BUILDDIR := build
INCDIR := include
TARGET := bin/feldgenerator

SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

CFLAGS := -std=c99 -Wall -Wextra -Werror $(shell pkg-config --cflags IL)
# CFLAGS := $(CFLAGS) -g # DEBUG
CFLAGS := $(CFLAGS) -O3 # RELEASE
LIB := -lm $(shell pkg-config --libs IL)
INC := -I $(INCDIR)

$(TARGET): $(OBJECTS)
	@echo "Linking…"
	$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning…";
	$(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean
