#pragma once
#include "Token.h"
#include <istream>
class Tokenizer {
	std::istream&          istm   ;
	char                   first  ;
public:
	Tokenizer(_Inout_ std::istream& istm);
	Token read_token();
};
