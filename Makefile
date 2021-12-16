
program: generatedo
	gcc -o $@ generated.o

generatedo: generatedc
	gcc -c -o generated.o generated.c

generatedc: examples/test.poclang grammar.js
	node index.js $< > generated.c

grammar.js: grammar.ne
	yarn build

clean:
	$(RM) generated*
	$(RM) grammar.js
	$(RM) program

