#include "Expression_Constant.h"
Expression_Constant::Expression_Constant(long long value) noexcept
	: value(value)
{
}
long long Expression_Constant::Evaluate() const noexcept
{
	return value;
}
