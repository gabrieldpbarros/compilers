#ifndef TOKENS
#define TOKENS

typedef enum {
    ID,    // identificador     [A-Za-z_]([A-Za-z0-9_]*)
    SIM,   // simbolo           (\+|\-|\*|\/|;)
    NUM,   // numero            [0-9]+(\.[0-9]+)?
    O_PAR, // abre parenteses   \(
    C_PAR, // fecha parenteses  \)
    O_COL, // abre colchetes    \[
    C_COL, // fecha colchetes   \]
    O_CHV, // abre chaves       \{
    C_CHV, // fecha chaves      \}
    ATT,   // atribuicao        =
    COMP,  // comparacao        ==
    STR    // string            "([caracter^(\\")]|\"|\n")*" | '([caracter^(\\')]|\'|\n')*'
} Tokens;

#endif