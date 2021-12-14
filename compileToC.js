const fs = require('fs');

const compile_array = (node) => {
    const values = node.values.map(value => compile_value(value)).join(', ');
    return `array_value((Value* []) {${values}, NULL})`;
}

const compile_value = (node) => {
    switch (node.type) {
        case 'int':
            return `int_value(${node.value})`;
        case 'string':
            return `string_value("${node.value}")`;
        case 'call':
            return compile_call(node);
        case 'name':
            return node.value;
        case 'array':
            return compile_array(node);
    }
}

const compile_call = (node) => {
    const name = `_${node.name.value}`;
    const args = node.args.map(value => compile_value(value)).join(', ');
    return `${name}(${args})`;
}

const compile_block = (nodes) => {
    return nodes.map(call => compile_call(call) + ';').join('\n\t');
}

const compile_def = (node) => {
    const name = `_${node.name.value}`;
    const args = node.args.map(arg => `Value* ${arg.value}`).join(', ');
    const body = compile_block(node.body);
    return `Value* ${name}(${args})\n{\n\t${body}\n\treturn NULL;\n}`;
}

const compileToC = (ast) => {
    const before = fs.readFileSync('./source/head.c').toString();
    const builtins = fs.readFileSync('./source/builtins.c').toString();
    const program = ast.map(def => compile_def(def)).join('\n\n').replace(/\t/g, '    ');
    const after = fs.readFileSync('./source/entry.c').toString();
    return `${before}\n${builtins}\n${program}\n${after}\n`;
}

module.exports = { compileToC };

