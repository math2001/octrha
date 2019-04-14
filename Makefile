.PHONY: run

run: octrha
	./octrha

octrha: *.h *.c
	gcc -o octrha main.c

