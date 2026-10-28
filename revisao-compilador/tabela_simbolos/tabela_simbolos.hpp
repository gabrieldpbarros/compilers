#ifndef TABELA_SIMBOLOS
#define TABELA_SIMBOLOS
#include "../analisadores/tokens.hpp"
#include <string>
#include <vector>
#include <optional>

namespace tabela_simbolos {
    typedef enum {
        VOID,
        BOOL,
        INT,
        FLOAT,
        DOUBLE,
        CHAR,
        ERRO,
    } Tipos;

    struct SimboloEscopo {
        std::string nome{};
        Tipos tipo{};
        std::string conteudo{};
    };

    std::vector<std::vector<SimboloEscopo>> inicializa_pilha_simbolos();
    Tipos token_para_tipo(NomeToken nome_token);
    std::vector<SimboloEscopo>* busca_escopo(
        std::vector<std::vector<SimboloEscopo>> &pilha_simbolos,
        std::string nome
    );
    void adiciona_escopo(std::vector<std::vector<SimboloEscopo>> &pilha_simbolos);
    void remove_escopo(std::vector<std::vector<SimboloEscopo>> &pilha_simbolos);
    int retorna_indice_simbolo(
        std::vector<SimboloEscopo> &tabela_simbolos,
        std::string nome
    );
    SimboloEscopo* busca_simbolo(
        std::vector<std::vector<SimboloEscopo>> &pilha_simbolos,
        std::string nome
    );
    void adiciona_simbolo(
        std::vector<std::vector<SimboloEscopo>> &pilha_simbolos,
        std::string nome,
        NomeToken tipo_token,
        std::string conteudo
    );
    void atualiza_simbolo(
        std::vector<std::vector<SimboloEscopo>> &pilha_simbolos,
        std::string nome,
        std::string novo_conteudo
    );
    void remove_simbolo(
        std::vector<std::vector<SimboloEscopo>> &pilha_simbolos,
        std::string nome
    );
    void imprime_tabela_simbolos(
        const std::vector<std::vector<SimboloEscopo>> &pilha_simbolos
    );
}

#endif