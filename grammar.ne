
@{%

const moo = require('moo');
const lexer = moo.compile({
    ws:         {match: /[\s;]/, lineBreaks: true},
    comment:    /\/\/.*?$/,
    float:      /\-?(?:(?:0|(?:[1-9][0-9]*))\.[0-9]+)/,
    int:        /\-?(?:0|(?:[1-9][0-9]*))/,
    string:     {match: /"(?:\\["\\n]|[^\n"\\])*"/, value: s => s.slice(1, -1)},
    keywords:   ['func', 'end'],
    name:       /[a-zA-Z][a-zA-Z0-9_]*/,
    lparen:     '(',
    rparen:     ')',
    lbracket:   '[',
    rbracket:   ']',
    comma:      ',',
    qmark:      '?',
    colon:      ':',
});

%}

@lexer lexer

main    ->  (_ def (_ def):*):? _
    {% ([v]) => v ? [v[1], ...v[2].map(v => v[1])] : null %}

def     ->  "func" __ %name _ %lparen (_ %name (%comma _ %name):*):? _ %rparen _ block _ "end"
    {% ([,,name,,,args,,,,body]) => ({
        type: 'def',
        name,
        args: args ? [args[1], ...args[2].map(arg => arg[2])] : [],
        body,
    }) %}

block   ->  (call (__ call):*):?
    {% ([calls]) => calls ? [calls[0], ...calls[1].map(call => call[1])] : [] %}

call    ->  %name _ %lparen (_ value (%comma _ value):*):? _ %rparen 
    {% ([name,,,args]) => ({type: 'call', name, args: args ? [args[1][0], ...args[2].map(v => v[2][0])] : []}) %}

array   ->  %lbracket (_ value (_ %comma _ value):*):? _ %rbracket
    {% ([,v]) => ({type: 'array', values: v ? [v[1][0], ...v[2].map(v => v[3][0])] : []}) %}

branch  ->  value __ %qmark __ value __ %colon __ value
    {% (v) => ({type: 'branch', condition: v[0][0], truthy: v[4][0], falsy: v[8][0]}) %}

value   ->  %int
        |   %float
        |   %string
        |   call
        |   %name
        |   array
        |   branch

__      ->  (%ws|%comment):+
_       ->  (%ws|%comment):*
