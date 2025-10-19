#include "Token.h"
#include "Expression_Constant.h"
Token::Token(Punctuator punct) noexcept
	: std::variant<Punctuator, long long>(punct)
{
}
Token::Token(long long  value) noexcept
	: std::variant<Punctuator, long long>(value)
{
}
std::unique_ptr<Expression> Token::Constant() const
{
	if (const auto value = std::get_if<long long>(this))
		return std::make_unique<Expression_Constant>(*value);
	else
		return {};
}
Operator Token::MultiplicativeOperator() const noexcept
{
	if (const auto op = std::get_if<Punctuator>(this)) {
		switch (*op) {
		case Punctuator::Asterisk:
			return { [](long long lhs, long long rhs) noexcept
			{
				return lhs * rhs;
			}, "*" };
		case Punctuator::Slash:
			return { [](long long lhs, long long rhs) noexcept
			{
				return lhs / rhs;
			}, "/" };
		default:
			return { };
		}
	}
	else
		return { };
}
Operator Token::AdditiveOperator() const noexcept
{
	if (const auto op = std::get_if<Punctuator>(this)) {
		switch (*op) {
		case Punctuator::Plus:
			return { [](long long lhs, long long rhs) noexcept
			{
				return lhs + rhs;
			}, "+" };
		case Punctuator::Minus:
			return { [](long long lhs, long long rhs) noexcept
			{
				return lhs - rhs;
			}, "-" };
		default:
			return {};
		}
	}
	else
		return {};
}

bool Token::is_punctuator(Punctuator punct) const noexcept
{
	if (const auto p = std::get_if<Punctuator>(this))
		return *p == punct;
	else
		return false;
}
