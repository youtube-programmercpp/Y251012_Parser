#include "Parser.h"
#include <iostream>
#include <exception>

int main()
{
	Parser parser(std::cin);
	while (std::cin) {
		try {
			if (const auto expression = parser.ParseExpression())
				std::cout << "計算結果: " << expression->Evaluate() << '\n';
			else
				break;
		}
		catch (const std::exception& e) {
			std::cerr << "エラー: " << e.what() << '\n';
		}
	}
}
