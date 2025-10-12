#include "Tokenizer.h"
#include <stdexcept>
#include <ctype.h>
Tokenizer::Tokenizer(_Inout_ std::istream& istm)
	: istm(istm)
{
	istm.read(&first, 1);
}
Token Tokenizer::read_token()
{
	while (istm) {
		switch (first) {
		case '-' : istm.read(&first, 1); return Punctuator::Minus     ;
		case '+' : istm.read(&first, 1); return Punctuator::Plus      ;
		case '*' : istm.read(&first, 1); return Punctuator::Asterisk  ;
		case '/' : istm.read(&first, 1); return Punctuator::Slash     ;
		case '(' : istm.read(&first, 1); return Punctuator::LeftParen ;
		case ')' : istm.read(&first, 1); return Punctuator::RightParen;
		case '\n': first = ' '         ; return Punctuator::Newline   ;
		case ' ' : istm.read(&first, 1); continue;
		default:
			if (isdigit(static_cast<unsigned char>(first))) {
				auto value = static_cast<long long>(first) - '0';
				for (;;) {
					char following;
					if (istm.read(&following, 1)) {
						if (isdigit(static_cast<unsigned char>(following))) {
							(value *= 10) += static_cast<long long>(following) - '0';
							continue;
						}
						else {
							first = following;
							return value;
						}
					}
					else
						return value;
				}
			}
			else
				throw std::runtime_error("invalid character");
		}
	}
	first = ' ';
	return Punctuator::Newline;
}
