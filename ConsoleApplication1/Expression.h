#pragma once
#include "Operator.h"
#include <string>
#include <memory>
#include <Windows.h>
#include <CommCtrl.h>
class Expression {
public:
	virtual ~Expression() noexcept = default;
	virtual long long Evaluate() const noexcept = 0;
	virtual HTREEITEM insert_item(HWND hTree, HTREEITEM hParent, HTREEITEM hInsertAfter) const noexcept = 0;
	static std::unique_ptr<Expression> Binary(std::unique_ptr<Expression>&& left, Operator op, std::unique_ptr<Expression>&& right);
};
