#pragma once
#include "pch.h"

enum class Cell {
	empty,
	hunter,
	prey,
	light
};
//template <size_t board_size>

constexpr size_t board_size = 10;

struct BoardState{
	void reset();
	const void printbrd();
	std::array<std::array<Cell, board_size>, board_size> board;
	BoardState() { reset(); }
};

