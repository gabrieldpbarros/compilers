#include "tabela_simbolos.hpp"
#include "../analisadores/tokens.hpp"
#include <iostream>

namespace tabela_simbolos {
    std::vector<std::vector<SimboloEscopo>> inicializa_pilha_simbolos() {
        return std::vector<std::vector<SimboloEscopo>>{{}};
    }

    Tipos token_para_tipo(NomeToken nome_token) {
        switch (nome_token) {
            case NomeToken::VOID: return Tipos::VOID;
            case NomeToken::BOOL: return Tipos::BOOL;
            case NomeToken::INT: return Tipos::INT;
            case NomeToken::FLOAT: return Tipos::FLOAT;
            case NomeToken::DOUBLE: return Tipos::DOUBLE;
            case NomeToken::CHAR: return Tipos::CHAR;
            default: return Tipos::ERRO;
        }
    }

    std::vector<SimboloEscopo>* busca_escopo(
        std::vector<std::vector<SimboloEscopo>> &pilha_simbolos,
        std::string nome
    ) {
        for(auto tabela_simbolos = pilha_simbolos.rbegin();
            tabela_simbolos != pilha_simbolos.rend();
            ++tabela_simbolos) {
                for (SimboloEscopo &simbolo : *tabela_simbolos)
                    if (simbolo.nome == nome) return &*tabela_simbolos;
            };
        return nullptr;
    }

    void adiciona_escopo(std::vector<std::vector<SimboloEscopo>> &pilha_simbolos) {
        pilha_simbolos.emplace_back();
    }

    void remove_escopo(std::vector<std::vector<SimboloEscopo>> &pilha_simbolos) {
            pilha_simbolos.pop_back();
    }

    int retorna_indice_simbolo( 
        std::vector<SimboloEscopo> &tabela_simbolos,
        std::string nome
    ) {
        int size_tabela = tabela_simbolos.size();
        for (int i = 0; i < size_tabela; i++)
            if (tabela_simbolos[i].nome == nome) return i;

        return -1;
    }

    SimboloEscopo* busca_simbolo(
        std::vector<std::vector<SimboloEscopo>> &pilha_simbolos,
        std::string nome
    ) {
        for(auto tabela_simbolos = pilha_simbolos.rbegin();
            tabela_simbolos != pilha_simbolos.rend();
            ++tabela_simbolos) {
                for (SimboloEscopo &simbolo : *tabela_simbolos)
                    if (simbolo.nome == nome) return &simbolo;
            };
        return nullptr;
    }

    void adiciona_simbolo(
        std::vector<std::vector<SimboloEscopo>> &pilha_simbolos,
        std::string nome,
        NomeToken tipo_token,
        std::string conteudo
    ) {
        if (busca_simbolo(pilha_simbolos, nome) != nullptr) {
            std::cout << "Símbolo já existe na tabela.\n";
            throw -1;
        } else {
            Tipos tipo = token_para_tipo(tipo_token);
            if (tipo == Tipos::ERRO) {
                std::cout << "Tipo de " << nome << " não foi encontrado.\n";
                throw -1;
            } else {
                SimboloEscopo simbolo{
                    nome,
                    tipo,
                    conteudo
                };
    
                std::vector<SimboloEscopo> &tabela{pilha_simbolos.back()};
                tabela.push_back(simbolo);
            }
        }
    }

    void atualiza_simbolo(
        std::vector<std::vector<SimboloEscopo>> &pilha_simbolos,
        std::string nome,
        std::string novo_conteudo
    ) {
        SimboloEscopo* simbolo = busca_simbolo(pilha_simbolos, nome);
        if (simbolo == nullptr) {
            std::cout << "Símbolo não existe na tabela.\n";
            throw -1;
        }
        else {
            simbolo->conteudo = novo_conteudo;
        }
    }

    void remove_simbolo(
        std::vector<std::vector<SimboloEscopo>> &pilha_simbolos,
        std::string nome
    ) {
        std::vector<SimboloEscopo>* tabela = busca_escopo(pilha_simbolos, nome);
        if (tabela == nullptr) {
            std::cout << "Símbolo não existe na tabela.\n";
            throw -1;
        } else {
            int indice_simbolo = retorna_indice_simbolo(*tabela, nome);
            tabela->erase(tabela->cbegin() + indice_simbolo);
        }
    }

    void imprime_tabela_simbolos(
        const std::vector<std::vector<SimboloEscopo>> &pilha_simbolos
    ) {
        for (std::size_t i = 0; i < pilha_simbolos.size(); ++i) {
            std::cout << "Escopo " << i << (i == 0 ? " (global)" : "") << ":\n";
            for (const SimboloEscopo &simbolo : pilha_simbolos[i]) {
                const char *tipo = "erro";
                switch (simbolo.tipo) {
                    case Tipos::VOID: tipo = "void"; break;
                    case Tipos::BOOL: tipo = "bool"; break;
                    case Tipos::INT: tipo = "int"; break;
                    case Tipos::FLOAT: tipo = "float"; break;
                    case Tipos::DOUBLE: tipo = "double"; break;
                    case Tipos::CHAR: tipo = "char"; break;
                    case Tipos::ERRO: break;
                }
                std::cout << "  " << simbolo.nome << " | " << tipo
                          << " | " << simbolo.conteudo << '\n';
            }
        }
    }
}