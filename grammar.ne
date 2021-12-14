
@{%

const moo = require('moo');
const lexer = moo.compile({
    ws:     {match: /[\s;]/, lineBreaks: true},
    int:    /0|(?:[1-9][0-9]*)/,
    string: {match: /"(?:\\["\\]|[^\n"\\])*"/, value: s => s.slice(1, -1)},
    keywords: ['let', 'func', 'return', 'end'],
    name:   /[a-zA-Z][a-zA-Z0-9]*/,
    lparen: '(',
    rparen: ')',
    comma:  ',',
});

%}

@lexer lexer

main    ->  (_ def (_ def):*):? _
    {% ([v]) => v ? [v[1], ...v[2].map(v => v[1])] : null %}

def     ->  "func" __ %name _ %lparen (_ %name (%comma _ %name):*):? _ %rparen _ block _ "end"
    {% ([,,name,,,args,,,,body]) => ({
        type: 'def',
        name,
        args: args ? [args[1], ...args[2].map(arg => arg[2])] : null,
        body,
    }) %}

block   ->  (call (__ call):*):?
    {% ([calls]) => calls ? [calls[0], ...calls[1].map(call => call[1])] : null %}

call    ->  %name _ %lparen (_ value (%comma _ value):*):? _ %rparen 
    {% ([name,,,args]) => ({type: 'call', name, args: args ? [args[1][0], ...args[2].map(v => v[2][0])] : null}) %}

value   ->  %int
        |   %string
        |   call
        |   %name

__      ->  %ws:+
_       ->  %ws:*
