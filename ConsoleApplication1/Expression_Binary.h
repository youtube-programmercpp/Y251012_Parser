#pragma once
#include "Expression.h"
#include <string>
class Expression_Binary : public Expression {
	Expression    * left ;
	Expression    * right;
	Operator        op   ; //‰‰ŽZŽq
public:
	Expression_Binary(Expression* left, Operator op, Expression* right) noexcept;
	~Expression_Binary() noexcept override;
	Expression_Binary(const Expression_Binary&) = delete;
	Expression_Binary& operator=(const Expression_Binary&) = delete;
	long long Evaluate() const noexcept override;
	HTREEITEM insert_item(HWND hTree, HTREEITEM hParent, HTREEITEM hInsertAfter) const noexcept override;
};
