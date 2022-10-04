
CFLAGS = -march=native -O3 -Wall -Wextra -std=c99 -Wimplicit-function-declaration -Wunused-parameter -Wshadow -Wdouble-promotion -Wundef -fno-common -fstack-usage -Wconversion -ffunction-sections -Wpadded -fshort-enums -ffast-math
PREFIX = /usr/local
INSTALLDIR = $(PREFIX)/bin
TARGET = kavpass
CC = cc
RM = rm -rf
SRCDIR = src
OBJDIR = obj
INCDIR = include
IV := -I $(INCDIR)
LINKER = cc
LFLAGS = -Wall $(IV) -lcrypto -Wimplicit-function-declaration -D_DEFAULT_SOURCE -Wunused-parameter -lm
FL := -lssl -lcrypto -lcurl 
SRC := $(wildcard $(SRCDIR)/*.c)
INC := $(wildcard $(INCDIR)/*.h)
OBJS := $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)






kavpass: ${OBJS}
	$(LINKER) $(FL) $(OBJS) -o $@
	@echo "Linking complete."
$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(INCDIR) $(OBJDIR)
	$(CC) $(CFLAGS) $(LFLAGS) -c $< -o $@
	@echo "Compiliation complete."
.PHONY: install
install:
	mkdir -p ${DESTDIR}/${PREFIX}/bin
	cp -f kavpass ${DESTDIR}${PREFIX}/bin
	@echo "Installation complete."
.PHONY: clean
clean:
	$(RM) $(OBJS)
	@echo "Clean complete."
.PHONY: fullclean
fc:
	$(RM) $(OBJDIR)/*
	$(RM) $(TARGET)
	@echo "Clean complete."
.PHONY: uninstall
uninstall:
	$(RM) ${DESTDIR}${PREFIX}/bin/$(TARGET)
	@echo "Uninstallation complete."
