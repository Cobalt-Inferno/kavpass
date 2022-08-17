CFLAGS = -march=native -O2 -Wall -Wextra
PREFIX = /usr/local
INSTALLDIR = $(PREFIX)/bin
TARGET = xArtix
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra 
RM = rm -rf
SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCDIR = include
IV := -I $(INCDIR)
LINKER = gcc
LFLAGS = -Wall $(IV) -lm
SRC := $(wildcard $(SRCDIR)/*.c)
INC := $(wildcard $(INCDIR)/*.h)
OBJS := $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)



$(BINDIR)/$(TARGET) : $(OBJS)
	mkdir -p $(INCDIR) $(BINDIR) $(OBJDIR)
	$(LINKER) $(OBJS) $(LFLAGS) -o $@
	@echo "Linking complete."
$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(INCDIR) $(BINDIR) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiliation complete."
.PHONY: install
install:
	mkdir -p ${INSTALLDIR}
	cp -r $(BINDIR)/$(TARGET) ${INSTALLDIR}
	@echo "Installation complete."
.PHONY: clean
clean:
	$(RM) $(OBJS)
	@echo "Clean complete."
.PHONY: uninstall
uninstall:
	$(RM) ${INSTALLDIR}/$(TARGET)
	@echo "Uninstallation complete."
