# Top level EAVR Makefile
# $Id$

.PHONY: all
all:
	cd lcd && ${MAKE} && cd ..

.PHONY: clean
clean:
	cd lcd && ${MAKE} clean && cd ..
