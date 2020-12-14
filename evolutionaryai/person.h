#pragma once
#include "pch.h"
#include "expression.h"
#include "board_state.h"

enum class facing {
	up,
	down,
	left,
	right
};
class Person{
private:
	std::vector<Expression> ai_script;
	uint8_t RegA = 0,
            RegB = 0;
	facing dir = facing::up;
public:
	size_t won_count = 0;
	size_t program_counter = 0;
	using gen_type = std::mt19937;
	size_t x_coord = 0,
           y_coord = 0;
	bool won = false;
	bool moved = false;
	size_t selectability = 10;
	bool operator<(const Person& other) {
		return this->selectability > other.selectability; // Note switched so high selectability is low
	}
	void reset();
	Cell type = Cell::hunter;
	Person() {}
	Person(Cell t): type(t) {}
	void random_genome(gen_type gen);
	void mutate_genome(gen_type gen);
	void printScript();
	void cross_genome(Person& Parent2, gen_type gen);
	void step(BoardState&);
};

