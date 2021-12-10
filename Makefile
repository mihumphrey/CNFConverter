SRCFILES := $(wildcard src/*.c)
HFILES := $(wildcard src/*.c)

all: $(SRCFILES) $(HFILES)
	gcc -o cnf -Ofast $(SRCFILES)

debug: $(SRCFILES) $(HFILES)
	gcc -o cnf -g -fsanitize=address $(SRCFILES)

zip:
	make clean >/dev/null
	zip -r cnf.zip src Makefile README.md
 
clean:
	rm -f cnf && clear

.PHONY: all zip clean
