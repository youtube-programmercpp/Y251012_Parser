#pragma once
#include "Operator.h"
#include <memory>
class Expression {
public:
	virtual ~Expression() noexcept = default;
	virtual long long Evaluate() const noexcept = 0;
	static std::unique_ptr<Expression> Binary(std::unique_ptr<Expression>&& left, Operator op, std::unique_ptr<Expression>&& right);
};
