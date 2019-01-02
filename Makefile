CXX = g++
LDFLAGS = -std=gnu++17 -O3
PREFIX = /usr/local

.PHONY: install
install:
	$(CXX) ./src/Toast.cpp $(LDFLAGS) -o toast
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp toast $(DESTDIR)$(PREFIX)/bin/toast

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/toast