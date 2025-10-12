#pragma once
#include "Expression.h"
class Expression_Constant : public Expression {
	long long value;
public:
	Expression_Constant(long long value) noexcept;
	long long Evaluate() const noexcept override;
};
