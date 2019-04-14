.PHONY: run

run: octrha
	./octrha

octrha: main.c
	gcc -o octrha main.c

