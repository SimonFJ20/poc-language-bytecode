const { readFile } = require('fs/promises');
const nearley = require('nearley');
const { compileToC } = require('./compileToC');
const grammar = require('./grammar');

const main = async () => {
    if (process.argv.length < 3) throw new Error('not enough args')
    const filename = process.argv[2];
    const text = (await readFile(filename)).toString();
    const parser = new nearley.Parser(nearley.Grammar.fromCompiled(grammar));
    parser.feed(text);
    const ast = parser.results[0];
    // console.log(JSON.stringify(ast, null, 4));
    const c = compileToC(ast);
    console.log(c);
}

main();
