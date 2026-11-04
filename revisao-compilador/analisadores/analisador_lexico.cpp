#include "analisador_lexico.hpp"
#include "tokens.hpp"
#include <iostream>
#include <stack>
#include <string_view>

namespace analisador_lexico {
    bool eh_sinalizador_string(char caractere) {
        if(caractere == '\'' || caractere == '\"')
            return true;
        return false;
    }

    bool eh_caractere_string(char caractere) {
        if((caractere >= '!' && caractere <= '~') ||
            caractere == ' ')
            return true;
        return false;
    }

    bool eh_alfabetico(char caractere) {
      if ((caractere >= 'a' && caractere <= 'z') ||
          (caractere >= 'A' && caractere <= 'Z'))
          return true;
      return false;
    }

    bool eh_numero(char caractere) {
        if (caractere >= '0' && caractere <= '9') return true;
        return false;
    }

    bool eh_simbolo(char caractere) {
        if (caractere == '+' ||
            caractere == '-' ||
            caractere == '*' ||
            caractere == '/' ||
            caractere == ';' ||
            caractere == '=' ||
            caractere == '(' ||
            caractere == ')' ||
            caractere == '[' ||
            caractere == ']' ||
            caractere == '{' ||
            caractere == '}') 
            return true;
        return false;
    }

    bool eh_ponto_final(char caractere) {
        if (caractere == '.') return true;
        return false;
    }

    bool eh_underscore(char caractere) {
        if (caractere == '_') return true;
        return false;
    }

    bool eh_inutil(char caractere) {
        if (caractere == ' ' ||
            caractere == '\n' ||
            caractere == '\t' ||
            caractere == '\r')
           return true;
        return false;
    }

    bool eh_fim(int tamanho_cadeia, int &indice_atual) {
        if (indice_atual >= tamanho_cadeia)
            return true;
        return false;
    }

    std::string_view token_to_string(Token token) {
        switch (token.name) {
            case NomeToken::ID: return "ID";
            case NomeToken::SIM: return "SIM";
            case NomeToken::NUM: return "NUM";
            case NomeToken::SUM: return "SUM";
            case NomeToken::SUB: return "SUB";
            case NomeToken::MULT: return "MULT";
            case NomeToken::DIV: return "DIV";
            case NomeToken::END_LINE: return "END_LINE";
            case NomeToken::O_PAR: return "O_PAR";
            case NomeToken::C_PAR: return "C_PAR";
            case NomeToken::O_COL: return "O_COL";
            case NomeToken::C_COL: return "C_COL";
            case NomeToken::O_CHV: return "O_CHV";
            case NomeToken::C_CHV: return "C_CHV";
            case NomeToken::ATT: return "ATT";
            case NomeToken::COMP: return "COMP";
            case NomeToken::STR: return "STR";
            case NomeToken::END: return "END";
            case NomeToken::IF: return "IF";
            case NomeToken::ELSE: return "ELSE";
            case NomeToken::WHILE: return "WHILE";
            case NomeToken::VOID: return "VOID";
            case NomeToken::BOOL: return "BOOL";
            case NomeToken::UNSIGNED: return "UNSIGNED";
            case NomeToken::INT: return "INT";
            case NomeToken::FLOAT: return "FLOAT";
            case NomeToken::DOUBLE: return "DOUBLE";
            case NomeToken::CHAR: return "CHAR";
            case NomeToken::RETURN: return "RETURN";
            case NomeToken::PRINTF: return "PRINTF";
        }
        return "DESCONHECIDO";
    }

    void _print_token(Token token) {
        std::cout << "<" << token_to_string(token);
        if (token.content.length() > 0) 
            std::cout << ", " << token.content;
        std::cout << "> ";
    }

    void check_reserved_words(Lexema* lexema) {
        if (lexema->token_content.empty()) return;

        NomeToken token = NomeToken::ID;
        switch (lexema->token_content.front()) {
            case 'i':
                if (lexema->token_content == "if") token = NomeToken::IF;
                else if (lexema->token_content == "int") token = NomeToken::INT;
                break;
            case 'e':
                if (lexema->token_content == "else") token = NomeToken::ELSE;
                break;
            case 'w':
                if (lexema->token_content == "while") token = NomeToken::WHILE;
                break;
            case 'v':
                if (lexema->token_content == "void") token = NomeToken::VOID;
                break;
            case 'b':
                if (lexema->token_content == "bool") token = NomeToken::BOOL;
                break;
            case 'u':
                if (lexema->token_content == "unsigned") token = NomeToken::UNSIGNED;
                break;
            case 'f':
                if (lexema->token_content == "float") token = NomeToken::FLOAT;
                break;
            case 'd':
                if (lexema->token_content == "double") token = NomeToken::DOUBLE;
                break;
            case 'c':
                if (lexema->token_content == "char") token = NomeToken::CHAR;
                break;
            case 'r':
                if (lexema->token_content == "return") token = NomeToken::RETURN;
                break;
            case 'p':
                if (lexema->token_content == "printf") token = NomeToken::PRINTF;
                break;
        }

        if (token != NomeToken::ID) {
            lexema->temp_token_name = token;
            lexema->token_content.clear();
        }
    }

    void check_simbols(Lexema* lexema) {
        if (lexema->token_content.empty()) return;

        NomeToken token = NomeToken::SIM;
        switch (lexema->token_content.front()) {
            case '+':
                token = NomeToken::SUM;
                break;
            case '-':
                token = NomeToken::SUB;
                break;
            case '/':
                token = NomeToken::DIV;
                break;
            case '*':
                token = NomeToken::MULT;
                break;
            case ';':
                token = NomeToken::END_LINE;
                break;
            case '(':
                token = NomeToken::O_PAR;
                break;
            case ')':
                token = NomeToken::C_PAR;
                break;
            case '[':
                token = NomeToken::O_COL;
                break;
            case ']':
                token = NomeToken::C_COL;
                break;
            case '{':
                token = NomeToken::O_CHV;
                break;
            case '}':
                token = NomeToken::C_CHV;
                break;
            case '=':
                if (lexema->token_content == "=") token = NomeToken::ATT;
                else if (lexema->token_content == "==") token = NomeToken::COMP;
                break;
        }

        if (token != NomeToken::ID) {
            lexema->temp_token_name = token;
            lexema->token_content.clear();
        }
    }

    Token get_next_token(std::string_view cadeia, int &indice_atual, Lexema* lexema) {
        // zeramos o lexema
        lexema->string_index    = {};
        lexema->temp_token_name = {};
        lexema->token_content   = {};

        int tamanho_cadeia = cadeia.size();

        // consumo de caracteres inuteis
        while (!eh_fim(tamanho_cadeia, indice_atual) &&
                eh_inutil(cadeia.at(indice_atual)))
            indice_atual++;
    
        int indice_inicio_analise = indice_atual;

        // fim do arquivo
        if (eh_fim(tamanho_cadeia, indice_atual)) {
            lexema->string_index    = {indice_atual};
            lexema->temp_token_name = {NomeToken::END};
        }

        // inicio de token
        // possivel ID: [A-Za-z_]
        else if (eh_alfabetico(cadeia.at(indice_atual)) ||
            eh_underscore(cadeia.at(indice_atual))) {
            lexema->string_index    = {indice_atual};
            lexema->temp_token_name = {NomeToken::ID};
            lexema->token_content.push_back(cadeia.at(indice_atual++));

            // ([A-Za-z0-9_]*)
            for (/*nao precisamos de nada aqui*/;
                !eh_fim(tamanho_cadeia, indice_atual) &&
                (eh_alfabetico(cadeia.at(indice_atual)) ||
                eh_numero(cadeia.at(indice_atual))     ||
                eh_underscore(cadeia.at(indice_atual)));
                indice_atual++
            )
                lexema->token_content.push_back(cadeia.at(indice_atual));
        }

        // possivel numero: [0-9]+(\.[0-9]+)?
        else if (eh_numero(cadeia.at(indice_atual))) {
            lexema->string_index    = {indice_atual};
            lexema->temp_token_name = {NomeToken::NUM};
            lexema->token_content.push_back(cadeia.at(indice_atual++));

            // [0-9]+
            for (;
                !eh_fim(tamanho_cadeia, indice_atual) &&
                eh_numero(cadeia.at(indice_atual)); 
                indice_atual++) 
                lexema->token_content.push_back(cadeia.at(indice_atual));
            
            // (\.[0-9]+)?
            if (!eh_fim(tamanho_cadeia, indice_atual) && 
                eh_ponto_final(cadeia.at(indice_atual))) {
                lexema->token_content.push_back(cadeia.at(indice_atual++));

                for (; 
                    !eh_fim(tamanho_cadeia, indice_atual) && 
                    eh_numero(cadeia.at(indice_atual)); 
                    indice_atual++)
                    lexema->token_content.push_back(cadeia.at(indice_atual));
            }
        }

        // possivel simbolo: (\+|\-|\*|\/|;|\(|\)|\[\|\]|\{|\}|\=|\=\=)
        else if (eh_simbolo(cadeia.at(indice_atual))) {
            lexema->string_index    = {indice_atual};
            lexema->temp_token_name = {NomeToken::SIM};
            lexema->token_content.push_back(cadeia.at(indice_atual++));

            // caso de atribuicao (==)
            if (!eh_fim(tamanho_cadeia, indice_atual) && 
                cadeia.at(indice_atual) == '=')
                lexema->token_content.push_back(cadeia.at(indice_atual++));
        }

        // possivel string: "([caracter^("\)]|\[caracter])*" | '([caracter^('\)]|\[caracter])*'
        else if (eh_sinalizador_string(cadeia.at(indice_atual))) {
            lexema->string_index    = {indice_atual};
            lexema->temp_token_name = {NomeToken::STR};
            lexema->token_content.push_back(cadeia.at(indice_atual)); // nao deve incrementar

            // monta uma pilha que espera outra aspa
            std::stack<char> pilha_aspas{};
            pilha_aspas.push(cadeia.at(indice_atual++));

            for(;
                !pilha_aspas.empty() &&
                !eh_fim(tamanho_cadeia, indice_atual) &&
                eh_caractere_string(cadeia.at(indice_atual));
                indice_atual++
            ) {
                char caractere_atual = cadeia.at(indice_atual);
                // verifica se a string encerrou
                if (caractere_atual == pilha_aspas.top()) {
                    pilha_aspas.pop();
                    lexema->token_content.push_back(cadeia.at(indice_atual));
                }

                if (pilha_aspas.size() > 0) {
                    // [caracter^([ASPAS]\)]
                    if (caractere_atual != '\\') {
                        lexema->token_content.push_back(cadeia.at(indice_atual));
                    }
                    // \[caracter]
                    else if (indice_atual + 1 < tamanho_cadeia && 
                            eh_caractere_string(cadeia.at(indice_atual + 1))){
                        lexema->token_content.push_back(cadeia.at(indice_atual++));
                        lexema->token_content.push_back(cadeia.at(indice_atual));
                    }
                }
            }

            if (!pilha_aspas.empty()) {
                throw std::runtime_error(
                    std::string("STRING NAO FECHADA")
                );
            }
        }

        if (lexema->temp_token_name == NomeToken::ID)
            check_reserved_words(lexema);

        if (lexema->temp_token_name == NomeToken::SIM)
            check_simbols(lexema);

        if (!eh_fim(tamanho_cadeia, indice_atual) &&
            indice_inicio_analise == indice_atual) {
            throw std::runtime_error(
                std::string("CARACTER INVALIDO: ") + cadeia.at(indice_atual)
            );
        }

        return Token{
            lexema->temp_token_name,
            lexema->token_content
        };
    }
}
