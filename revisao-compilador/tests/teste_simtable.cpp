#include "../analisadores/tokens.hpp"
#include "../tabela_simbolos/tabela_simbolos.hpp"
#include <iostream>

int main(int argv, char* argc[]) {
    tabela_simbolos::SimboloEscopo simbolo_generico{};
    std::vector<tabela_simbolos::SimboloEscopo> tabela_generica{};
    auto pilha = tabela_simbolos::inicializa_pilha_simbolos();

    // testa escopo global
    tabela_simbolos::adiciona_simbolo(
        pilha,
        "numero_dois",
        NomeToken::INT,
        "2"
    );
    simbolo_generico = *tabela_simbolos::busca_simbolo(
        pilha,
        "numero_dois"
    );
    std::cout << "ESPERADO: 2\n";
    std::cout << simbolo_generico.conteudo << '\n';

    tabela_simbolos::adiciona_escopo(pilha);
    tabela_simbolos::adiciona_simbolo(
        pilha,
        "numero_um_cinco",
        NomeToken::FLOAT,
        "1.5"
    );
    tabela_simbolos::adiciona_simbolo(
        pilha,
        "letra_c",
        NomeToken::CHAR,
        "c"
    );
    // procura do escopo global
    simbolo_generico = *tabela_simbolos::busca_simbolo(
        pilha,
        "numero_dois"
    );
    std::cout << "ESPERADO: 2\n";
    std::cout << simbolo_generico.conteudo << '\n';
    // procura do escopo local
    simbolo_generico = *tabela_simbolos::busca_simbolo(
        pilha,
        "letra_c"
    );
    std::cout << "ESPERADO: c\n";
    std::cout << simbolo_generico.conteudo << '\n';

    tabela_simbolos::atualiza_simbolo(
        pilha,
        "numero_um_cinco",
        "1.12"
    );
    // procura atualizacao
    simbolo_generico = *tabela_simbolos::busca_simbolo(
        pilha,
        "numero_um_cinco"
    );
    std::cout << "ESPERADO: 1.12\n";
    std::cout << simbolo_generico.conteudo << '\n';

    tabela_simbolos::remove_simbolo(
        pilha,
        "numero_um_cinco"
    );
    // procura remocao
    auto* simbolo = tabela_simbolos::busca_simbolo(
        pilha,
        "numero_um_cinco"
    );
    std::cout << "ESPERADO: nullptr\n";
    if (simbolo == nullptr)
        std::cout << "nullptr\n";
    else
        std::cout << simbolo->conteudo << '\n';

    // procura tabela inexistente
    auto* tabela = tabela_simbolos::busca_escopo(
        pilha,
        "vinte"
    );
    std::cout << "ESPERADO: nullptr\n";
    if (simbolo == nullptr)
        std::cout << "nullptr\n";
    else
        std::cout << tabela << '\n';

    tabela_simbolos::adiciona_escopo(pilha);
    tabela_simbolos::adiciona_simbolo(
        pilha,
        "numero_dez",
        NomeToken::INT,
        "10"
    );
    tabela_simbolos::adiciona_simbolo(
        pilha,
        "numero_cinco_quarenta_e_dois",
        NomeToken::FLOAT,
        "5.42"
    );
    tabela_simbolos::adiciona_simbolo(
        pilha,
        "letra_z",
        NomeToken::CHAR,
        "z"
    );
    tabela_simbolos::imprime_tabela_simbolos(pilha);
    
    tabela_simbolos::remove_escopo(pilha);
    tabela_simbolos::imprime_tabela_simbolos(pilha);

    return 0;
}