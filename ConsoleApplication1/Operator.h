#pragma once
struct Operator {
	long long (*function_pointer)(long long lhs, long long rhs);
	const char* symbol;
	explicit operator bool() const noexcept
	{
		return function_pointer != nullptr;
	}
};
