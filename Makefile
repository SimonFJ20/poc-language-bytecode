
CFLAGS=-Wall
LFLAGS=-lm

program: generatedo
	gcc -o $@ generated.o $(LFLAGS) $(CFLAGS)

generatedo: generatedc
	gcc -c -o generated.o generated.c $(CFLAGS)

generatedc: examples/reduceRight.poclang grammar.js
	node index.js $< > generated.c

grammar.js: grammar.ne
	yarn build

clean:
	$(RM) generated*
	$(RM) grammar.js
	$(RM) program

