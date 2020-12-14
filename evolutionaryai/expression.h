#pragma once
#include "pch.h"

namespace xpr { // its too annoying to use a enum class and static cast every single code
	enum code: uint8_t {
		move,
		look,
		trnr, // turn left
		trnl, // turn right
		jump, // jmp absolute
		jltA, // jump less than A
		jltB, // jump less than B
		jgtA, // jump less than A
		jgtB, // jump less than B
		jmpA,
		jmpB,
		negA,
		negB,
		addA,
		addB,
		setA,
		setB,
		subA,
		subB,
		swap,
		MAX_CODE
	};
}
struct Expression{
	xpr::code code;
	uint8_t   opt1;
	uint8_t   opt2;
	const void println();
};

