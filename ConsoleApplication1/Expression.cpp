#include "Expression.h"
#include "Expression_Binary.h"
std::unique_ptr<Expression> Expression::Binary(std::unique_ptr<Expression>&& left, Operator op, std::unique_ptr<Expression>&& right)
{
	auto retval = std::make_unique<Expression_Binary>(left.get(), op, right.get());
	(void)left .release();
	(void)right.release();
	return retval;
}
