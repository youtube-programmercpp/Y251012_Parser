#include "Parser.h"
#include "Expression.h"
#include <stdexcept>
std::unique_ptr<Expression> Parser::Primary()
{
	if (auto retval = last_token.Constant()) {
		last_token = tokenizer.read_token();
		return retval;
	}
	else if (last_token.is_punctuator(Punctuator::LeftParen)) {
		last_token = tokenizer.read_token();
		auto retval = Additive();
		if (last_token.is_punctuator(Punctuator::RightParen)) {
			last_token = tokenizer.read_token();
			return retval;
		}
		else
			throw std::runtime_error("missing right parenthesis");
	}
	else
		throw std::runtime_error("invalid syntax");
}
std::unique_ptr<Expression> Parser::Mulitiplicative()
{
	for (auto left = Primary();;) {
		if (const auto op = last_token.MultiplicativeOperator()) {
			last_token = tokenizer.read_token();
			left = Expression::Binary(std::move(left), op, Primary());
			continue;
		}
		else
			return left;
	}
}
std::unique_ptr<Expression> Parser::Additive()
{
	for (auto left = Mulitiplicative();;) {
		if (const auto op = last_token.AdditiveOperator()) {
			last_token = tokenizer.read_token();
			left = Expression::Binary(std::move(left), op, Mulitiplicative());
			continue;
		}
		else
			return left;
	}
}
Parser::Parser(_Inout_ std::istream& istm)
	: tokenizer(istm)
	, last_token()
{
}
std::unique_ptr<Expression> Parser::ParseExpression()
{
	last_token = tokenizer.read_token();
	if (last_token.is_punctuator(Punctuator::Newline))
		return {};
	else
		return Additive();
}
