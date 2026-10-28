#include "analisador_lexico.hpp"
#include "tokens.hpp"
#include <string_view>

namespace analisador_lexico {
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

    Token get_next_token(std::string_view cadeia, int &indice_atual, Lexema* lexema) {
        // zeramos o lexema
        lexema->string_index = {};
        lexema->temp_token_name = {};
        lexema->token_content = {};

        // consumo de caracteres inuteis
        while (eh_inutil(cadeia.at(indice_atual)))
            indice_atual++;

        // fim do arquivo
        if (cadeia.at(indice_atual) == '\0') {
            lexema->string_index = {indice_atual};
            lexema->temp_token_name = {NomeToken::END};
        }

        // inicio de token
        // possivel ID: [A-Za-z_]
        else if (eh_alfabetico(cadeia.at(indice_atual)) ||
            eh_underscore(cadeia.at(indice_atual))) {
            lexema->string_index = {indice_atual};
            lexema->temp_token_name = {NomeToken::ID};
            lexema->token_content.push_back(cadeia.at(indice_atual));

            // ([A-Za-z0-9_]*)
            for (/*nao precisamos de nada aqui*/;
                eh_alfabetico(cadeia.at(indice_atual)) ||
                eh_numero(cadeia.at(indice_atual))     ||
                eh_underscore(cadeia.at(indice_atual));
                indice_atual++
            )
                lexema->token_content.push_back(cadeia.at(indice_atual));
        }

        // possivel numero: [0-9]+(\.[0-9]+)?
        else if (eh_numero(cadeia.at(indice_atual))) {
            lexema->string_index = {indice_atual};
            lexema->temp_token_name = {NomeToken::NUM};
            lexema->token_content.push_back(cadeia.at(indice_atual));

            // [0-9]+
            for (; eh_numero(cadeia.at(indice_atual)); indice_atual++) 
                lexema->token_content.push_back(cadeia.at(indice_atual));
            
            // (\.[0-9]+)?
            if (eh_ponto_final(cadeia.at(indice_atual))) {
                lexema->token_content.push_back(cadeia.at(indice_atual));

                for (; eh_numero(cadeia.at(indice_atual)); indice_atual++)
                    lexema->token_content.push_back(cadeia.at(indice_atual));
            }
        }

        // possivel simbolo: (\+|\-|\*|\/|;|\(|\)|\[\|\]|\{|\}|\=|\=\=)
        else if (eh_simbolo(cadeia.at(indice_atual))) {
            lexema->string_index = {indice_atual};
            lexema->temp_token_name = {NomeToken::SIM};
            lexema->token_content.push_back(cadeia.at(indice_atual++));

            // caso de atribuicao (==)
            if (cadeia.at(indice_atual) == '=')
                lexema->token_content.push_back(cadeia.at(indice_atual));
        }

        return Token{
            lexema->temp_token_name,
            lexema->token_content
        };
    }
}