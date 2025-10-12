#pragma once
#include "Token.h"
#include "Expression.h"
#include "Tokenizer.h"
#include <memory>
class Parser {
	Tokenizer tokenizer ;
	Token     last_token;
	std::unique_ptr<Expression> Primary        ();
	std::unique_ptr<Expression> Mulitiplicative();
	std::unique_ptr<Expression> Additive       ();
public:
	Parser(_Inout_ std::istream& istm);
	std::unique_ptr<Expression> ParseExpression();
};
