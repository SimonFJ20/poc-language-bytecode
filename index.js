const { readFile } = require('fs/promises');
const nearley = require('nearley');
const grammar = require('./grammar');

const main = async () => {
    if (process.argv.length < 3) throw new Error('not enough args')
    const filename = process.argv[2];
    const text = (await readFile(filename)).toString();
    const parser = new nearley.Parser(nearley.Grammar.fromCompiled(grammar));
    parser.feed(text);
    console.log(JSON.stringify(parser.results[0], null, 4));
}

main();
