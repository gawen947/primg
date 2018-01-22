TARGET = primg

SRC  = $(wildcard *.c)
OBJS = $(SRC:.c=.o)
DEPS = $(SRC:.c=.d)

CFLAGS := -O2 -I/usr/local/include -fomit-frame-pointer -std=c99 \
	-pedantic -Wall -Wextra -MMD -pipe -ggdb
LDFLAGS := -L/usr/local/lib -lgmp

ifdef VERBOSE
	Q :=
else
	Q := @
endif

.PHONY: all clean

%.o: %.c
	@echo "===> CC $<"
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET): $(OBJS)
	@echo "===> LD $@"
	$(Q)$(CC) $(OBJS) $(LDFLAGS) -o $@

clean:
	@echo "===> CLEAN"
	$(Q)rm -f *.o
	$(Q)rm -f *.d
	$(Q)rm -f $(TARGET)

install:
	@echo "===> Installing $(TARGET)"
	$(Q)install -s $(TARGET) /usr/local/bin

-include $(DEPS)
