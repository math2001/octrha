.PHONY: run

test: octrha
	./octrha test

octrha: *.h *.c
	gcc -o octrha main.c

