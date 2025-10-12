#pragma once
#include "Operator.h"
#include "Punctuator.h"
#include "Expression.h"
#include <variant>
#include <memory>
class Token : private std::variant<Punctuator, long long> {
public:
	Token() noexcept = default;
	Token(Punctuator punct) noexcept;
	Token(long long  value) noexcept;
	std::unique_ptr<Expression> Constant() const;
	Operator MultiplicativeOperator() const noexcept;
	Operator AdditiveOperator      () const noexcept;
	bool is_punctuator(Punctuator punct) const noexcept;
};
