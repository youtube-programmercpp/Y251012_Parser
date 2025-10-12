#pragma once
#include "Expression.h"
class Expression_Binary : public Expression {
	Expression    * left ;
	Expression    * right;
	Operator        op   ; //‰‰ŽZŽq
public:
	Expression_Binary(Expression* left, Operator op, Expression* right) noexcept;
	~Expression_Binary() noexcept override;
	long long Evaluate() const noexcept override;
};
