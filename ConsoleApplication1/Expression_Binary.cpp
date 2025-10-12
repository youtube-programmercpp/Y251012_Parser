#include "Expression_Binary.h"
Expression_Binary::Expression_Binary(Expression* left , Operator op, Expression* right) noexcept
	: left (left )
	, right(right)
	, op   (op   )
{
}
Expression_Binary::~Expression_Binary() noexcept
{
	delete right;
	delete left ;
}
long long Expression_Binary::Evaluate() const noexcept
{
	return (*op)(left->Evaluate(), right->Evaluate());
}
