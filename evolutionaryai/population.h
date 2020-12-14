#pragma once
#include "pch.h"
#include "person.h"
#include "board_state.h"

class Population{
private:
	using gen_type = std::mt19937;
	gen_type gen;
	BoardState board;
	size_t population_size;
	std::vector<Person> hunters;
	std::vector<Person> prey;
	void runPair(size_t, bool);
	void mainLoop();
	size_t timout_steps;
	size_t epoches;
public:
	Population() : population_size(0), gen(std::random_device{}()), timout_steps(0), epoches(0){}
	Population(size_t pop, size_t timeout, size_t epo);
	~Population() {}
};
