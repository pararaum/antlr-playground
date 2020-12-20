// -*- mode: antlr -*-

grammar Expr;

// Main rule, explicitly catches the EOF, see https://github.com/antlr/antlr4/blob/4.9/doc/parser-rules.md
main: expr EOF;

expr: expr ('*'|'/') expr
    | expr ('+'|'-') expr
    | INT
    | '(' expr ')'
    ;
INT     : [0-9]+ ;
WHITESPACE  : [ \t\r\n]+ -> skip; // Skip white spaces... https://github.com/antlr/antlr4/blob/4.9/doc/getting-started.md
