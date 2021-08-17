sbin_PROGRAMS = udev_read_ahead_helper
udev_read_ahead_helper_SOURCES = udev-read-ahead-helper.c
udev_read_ahead_helper_CFLAGS = -I ../../include -I ../../libparser -I ../../libdevice -lmount
udev_read_ahead_helper_LDADD = ../../libparser/libparser.a ../../libdevice/libdevice.a ../../src/log.a
udev_read_ahead_helper_DEPENDENCIES = ../../libparser/libparser.a ../../libdevice/libdevice.a
EXTRA_udev_read_ahead_helper_DATA = 99-bdi.rules
EXTRA_udev_read_ahead_helperdir = /etc/udev/rules.d

99-bdi.rules: 99-bdi.rules.in Makefile
	sed "s@_helper_install_path_@$(sbindir)/udev_read_ahead_helper@" \
		$< > $@

clean-local:
	rm -fr 99-bdi.rules
