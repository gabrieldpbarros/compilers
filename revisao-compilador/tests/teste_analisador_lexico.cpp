#include <iostream>
#include <string_view>
#include <vector>
#include "../analisadores/tokens.hpp"
#include "../analisadores/analisador_lexico.hpp"

int main() {
  using namespace std::string_view_literals;
  std::vector<std::string_view> strings{
    "x = 1 + 2;"sv,
    "if (x) { return 0; }"sv,
    "while (i == 10) { x / 2; }"sv,
    "if"sv,
    "int x2;"sv,
    "float a = 1.7;"sv,
    "@"sv
  };

  for (unsigned i = 0; i < strings.size(); i++) {
    Token temp_token{};
    int index = 0;
    Lexema lexema{};

    while(temp_token.name != NomeToken::END) {
      using namespace analisador_lexico;
      temp_token = get_next_token(
        strings[i],
        index,
        &lexema
      );

      _print_token(temp_token);
    }
    std::cout << '\n';
  }

  return 0;
}