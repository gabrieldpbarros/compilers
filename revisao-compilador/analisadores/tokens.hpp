#ifndef TOKENS
#define TOKENS
#include <string>

typedef enum {
    ID,       // identificador     [A-Za-z_]([A-Za-z0-9_]*)
    SIM,      // simbolo           (\+|\-|\*|\/|;|\(|\)|\[\|\]|\{|\}|\=|\=\=)
    NUM,      // numero            [0-9]+(\.[0-9]+)?
    SUM,      // soma              \+
    SUB,      // subtracao         \-
    MULT,     // multiplicacao     \*
    DIV,      // divisao           \/
    END_LINE, // fim de linha      \;
    O_PAR,    // abre parenteses   \(
    C_PAR,    // fecha parenteses  \)
    O_COL,    // abre colchetes    \[
    C_COL,    // fecha colchetes   \]
    O_CHV,    // abre chaves       \{
    C_CHV,    // fecha chaves      \}
    ATT,      // atribuicao        \=
    COMP,     // comparacao        \=\=
    STR,      // string            "([caracter^(\\")]|\"|\n")*" | '([caracter^(\\')]|\'|\n')*'
    END,      // fim da cadeia
    // PALAVRAS RESERVADAS
    IF,
    ELSE,
    VOID,
    BOOL,
    UNSIGNED,
    INT,
    FLOAT,
    DOUBLE,
    CHAR,
    RETURN,
    PRINTF,
} NomeToken;

struct Lexema {
    int string_index{};
    NomeToken temp_token_name{};
    std::string token_content{};
};

struct Token {
    NomeToken name{};
    std::string content{};
};

#endif