CXX = g++
LDFLAGS = -std=c++2a -O3 -Wno-c++98-compat -Wno-c++98-compat-pedantic
PREFIX = /usr/local

.PHONY: install
install: toast
	$(CXX) toast.cpp $(LDFLAGS) -o toast
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp toast $(DESTDIR)$(PREFIX)/bin/toast

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/toast