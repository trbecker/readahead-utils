all: udev-read-ahead-helper

udev-read-ahead-helper: udev-read-ahead-helper.c
	$(CC) -g -o $@ $< -lmount

/bin/udev-read-ahead-helper: udev-read-ahead-helper
	install --owner=root --group=root --mode=0755 $< $@

/etc/udev/rules.d/99-bdi.rules: 99-bdi.rules
	install --owner=root --group=root --mode=0644 $< $@

/etc/readahead.conf: readahead.conf
	install --owner=root --group=root --mode=644 $< $@

install: /etc/udev/rules.d/99-bdi.rules /bin/udev-read-ahead-helper /etc/readahead.conf
