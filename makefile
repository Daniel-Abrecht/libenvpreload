CFLAGS  += -fPIC -shared
LDFLAGS += -fPIC -shared
CFLAGS  += -std=c99 -Wall -Wextra -pedantic -Werror
CFLAGS  += -Os -ffunction-sections -fdata-sections -Wl,--gc-sections
CFLAGS  += -D_DEFAULT_SOURCE

all: libenvpreload.so

libenvpreload.so: libenvpreload.o
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

clean:
	rm -f libenvpreload.so libenvpreload.o

install:
	mkdir -p $(DESTDIR)$(PREFIX)/usr/lib/xorg/modules/
	cp libenvpreload.so $(DESTDIR)$(PREFIX)/usr/lib/xorg/modules/
	mkdir -p $(DESTDIR)$(PREFIX)/etc/X11/xorg.conf.d/
	cp 10-libenvpreload.conf $(DESTDIR)$(PREFIX)/etc/X11/xorg.conf.d/

uninstall:
	rm -f $(DESTDIR)/usr/lib/xorg/modules/libenvpreload.so
	rm -f $(DESTDIR)/etc/X11/xorg.conf.d/10-libenvpreload.conf
