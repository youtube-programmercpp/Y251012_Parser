#pragma once
#include "Expression.h"
class Expression_Constant : public Expression {
	long long value;
public:
	Expression_Constant(long long value) noexcept;
	Expression_Constant(const Expression_Constant&) = delete;
	Expression_Constant& operator=(const Expression_Constant&) = delete;
	long long Evaluate() const noexcept override;
	HTREEITEM insert_item(HWND hTree, HTREEITEM hParent, HTREEITEM hInsertAfter) const noexcept override;
};
