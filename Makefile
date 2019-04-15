.PHONY: run install

test: octrha
	./octrha test

octrha: *.h *.c
	gcc -o octrha main.c

install: octrha
	cp octrha ~/.local/bin/octrha