#include "population.h"

void Population::runPair(size_t idx, bool interactive=false) {
	auto& h = hunters[idx];
	auto& p = prey[idx];
	board.reset();
	std::uniform_int_distribution<> posdist(0, board_size-1);
	h.x_coord = posdist(gen);
	h.y_coord = posdist(gen);
	p.x_coord = posdist(gen);
	p.y_coord = posdist(gen);
	while (h.x_coord == p.x_coord && h.y_coord == p.y_coord) {
		p.y_coord = posdist(gen); // make sure they don't start on the same spot
	}
	board.board[h.x_coord][h.y_coord] = Cell::hunter;
	board.board[p.x_coord][p.y_coord] = Cell::prey;
	h.program_counter = 0;
	p.program_counter = 0;
	for (size_t steps = 0; steps < timout_steps; steps++) {
		h.step(board);
		p.step(board);
		if (interactive) {
			if (h.moved || p.moved) {
				board.printbrd();
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		if (h.won) {
			if(interactive) fmt::print("H Won a round!\n");
			h.won_count++;
			h.selectability += 10;
			h.selectability += (timout_steps - steps)/ 20;
			p.selectability += steps/10;
			p.selectability /= 2;
			break;
		}
	}
	if (!h.won) {
		if (interactive) fmt::print("P Won a round!\n");
		p.won_count++;
		p.selectability += timout_steps;
		h.selectability /= 2; // hunters that lost lose a bunch of thire quality
	}
}
void Population::mainLoop(){
	for (size_t epo = 0; epo < epoches; epo++) {
		for (size_t runs = 0; runs < 5; runs++) {
			std::shuffle(hunters.begin(), hunters.end(), gen);
			for (size_t i = 0; i < population_size; i++) {
				this->runPair(i);
			}
		}
		std::sort(hunters.begin(), hunters.end());
		std::sort(prey.begin(), prey.end());
		auto qurter_pop = population_size / 4;
		// attempt "evolution" randomly mashing.
		for (size_t i = qurter_pop; i < qurter_pop*2; i++) {
			this->hunters[i]              = hunters[i - qurter_pop];
			this->prey[i]                 = hunters[i - qurter_pop];
			this->hunters[i + qurter_pop] = hunters[i - qurter_pop];
			this->prey[i + qurter_pop]    = hunters[i - qurter_pop];
			this->hunters[i]             .cross_genome(hunters[i + 2], gen);
			this->prey[i]                .cross_genome(prey[i + 2], gen);
			this->hunters[i + qurter_pop].cross_genome(hunters[i + 1], gen);
			this->prey[i + qurter_pop]   .cross_genome(prey[i + 1], gen);
		}
		auto second_half = population_size / 2;
		size_t number_new = 10;
		// mutate a couple randomly by a few points as copies
		for (size_t i = second_half; i < second_half* 2 - 1 - number_new; i++) {
			this->hunters[i] = hunters[i - second_half];
			this->prey[i]    = hunters[i - second_half];
			this->hunters[i]  .mutate_genome(gen);
			this->prey[i]     .mutate_genome(gen);
		}
		// completly randomly generate some genomes from scratch.
		for (size_t i = population_size - 10; i < population_size - 1; i++) {
			this->hunters[i].reset();
			this->prey[i].reset();
			this->hunters[i].random_genome(gen);
			this->prey[i].random_genome(gen);
		}
		fmt::print("epo:{}\n", epo);
	}
	fmt::print("Running interactive now! for top of both sections.");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	this->runPair(0, true);
	fmt::print("H has won {} times and P has won {} times!\n", this->hunters[0].won_count, this->prey[0].won_count );
	fmt::print("Running interactive one more time!");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	this->runPair(1, true);
	fmt::print("H has won {} times and P has won {} times!\n\nselectability is {} and {}", 
		this->hunters[1].won_count, this->prey[1].won_count,
		this->hunters[1].selectability, this->prey[1].selectability);
	
	fmt::print("\nAI script for top H is:\n");
	this->hunters[0].printScript();
	fmt::print("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	fmt::print("\nAI script for top P is:\n");
	this->prey[0].printScript();
}

Population::Population(size_t pop, size_t timeouts, size_t epo) : population_size(pop), gen(std::random_device{}()), timout_steps(timeouts), epoches(epo) {
	for (size_t _ = 0; _ < pop; ++_) {
		hunters.emplace_back(Cell::hunter);
		prey.emplace_back(Cell::prey);
		hunters.back().random_genome(gen);
		prey.back().random_genome(gen);
	}
	mainLoop();
}
