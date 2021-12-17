const fs = require('fs');

const compile_name = (node, ctx) => {
    return `_${node.value}`;
}

const compile_array = (node) => {
    const values = node.values.map(value => compile_value(value)).join(', ');
    return `array_value((Value* []) {${values}, NULL})`;
}

const compile_value = (node, ctx) => {
    switch (node.type) {
        case 'int':
            return `int_value(${node.value})`;
        case 'float':
            return `float_value(${node.value})`;
        case 'string':
            return `string_value("${node.value}")`;
        case 'call':
            return compile_call(node, ctx);
        case 'name':
            return compile_name(node, ctx);
        case 'array':
            return compile_array(node);
    }
}

const compile_call = (node, ctx) => {
    const func = ctx.funcs.find(func => func.name === node.name.value);
    if (!func) throw new Error(`function '${node.name.value}' not defined`);
    const name = `_${node.name.value}`;
    const args = node.args.map(value => compile_value(value, ctx));
    while (args.length < func.argc) args.push('NULL')
    return `${name}(${args.join(', ')})`;
}

const compile_block = (nodes, ctx) => {
    const calls = nodes.map(call => compile_call(call, ctx) + ';');
    if (calls.length > 0) calls[calls.length - 1] = `return ${calls[calls.length - 1]}`;
    else calls.push('return none_value();');
    return calls.join('\n\t');
}

const compile_def = (node, ctx) => {
    ctx.funcs.push({name: node.name.value, argc: node.args.length});
    const name = `_${node.name.value}`;
    const args = node.args.map(arg => `Value* _${arg.value}`).join(', ');
    const body = compile_block(node.body, ctx);
    return `Value* ${name}(${args})\n{\n\t${body}\n}`;
}

const compileToC = (ast) => {
    const ctx = {
        funcs: [...standardFuncs()],
    };
    const files = readSourceFilesSync();
    const before = sourceBefore(files);
    const program = ast.map(def => compile_def(def, ctx)).join('\n\n').replace(/\t/g, '    ');
    const after = sourceAfter(files);
    return cleanUp(`${before}\n${program}\n\n${after}\n`);
}

const standardFuncs = () => [
    {name: 'null',      argc: 0},
    {name: 'false',     argc: 0},
    {name: 'true',      argc: 0},
    {name: 'add',       argc: 2},
    {name: 'sub',       argc: 2},
    {name: 'mul',       argc: 2},
    {name: 'div',       argc: 2},
    {name: 'mod',       argc: 2},
    {name: 'pow',       argc: 2},
    {name: 'sqrt',      argc: 1},
    {name: 'string',    argc: 1},
    {name: 'at',        argc: 2},
    {name: 'length',    argc: 1},
    {name: 'join',      argc: 2},
    {name: 'split',     argc: 2},
    {name: 'if',        argc: 3},
    {name: 'print',     argc: 1},
    {name: 'input',     argc: 1},
]

const readSourceFilesSync = () => ({
    utilsC:     fs.readFileSync('./source/utils.c').toString(),
    utilsH:     fs.readFileSync('./source/utils.h').toString(),
    valueC:     fs.readFileSync('./source/value.c').toString(),
    valueH:     fs.readFileSync('./source/value.h').toString(),
    builtinsC:  fs.readFileSync('./source/builtins.c').toString(),
    entryC:     fs.readFileSync('./source/entry.c').toString(),
});

const sourceBefore = ({utilsC, valueC, builtinsC, utilsH, valueH}) => {
    return `\n${utilsH}\n${valueH}\n` + [utilsC, valueC, builtinsC].join('\n');
}

const sourceAfter = ({entryC}) => {
    return entryC;
}

const cleanUp = (text) => {
    return text
        .replaceAll('#include "utils.h"', '')
        .replaceAll('#include "value.h"', '');
}

module.exports = { compileToC };

