#include "person.h"

void Person::step(BoardState& state){
	this->won = false;
	if (this->type == Cell::hunter) {
		if (state.board[x_coord][y_coord] == Cell::prey) {
			this->won = true;
			return;
		}
	}
	bool fault = false;
	moved = false;
	auto &ex = this->ai_script[program_counter];
	auto try_move = [&](size_t& coord, bool forward) {
		bool out_of_bounds = forward ? coord >= board_size-1 : coord <= 0;
		if (out_of_bounds) {
			fault = true;
		}
		else {
			state.board[x_coord][y_coord] = Cell::empty;
			coord += forward ? 1 : -1;
			if (this->type == Cell::hunter) {
				if (state.board[x_coord][y_coord] == Cell::prey) {
					this->won = true;
				}
			}
			state.board[x_coord][y_coord] = this->type;
			this->moved = true;
		}
	};
	auto look_dir = [&](size_t& coord, bool forward) {
		RegB = 0;
		size_t reset = coord; // dont edit actual position and use to reset the coord after
		while (true) {
			RegB++;
			bool out_of_bounds = forward ? coord >= board_size-1 : coord <= 0;
			if (out_of_bounds) {
				RegA = 0;
				break;
			}
			coord += forward ? 1 : -1;
			if (state.board[x_coord][y_coord] == Cell::hunter || state.board[x_coord][y_coord] == Cell::prey) {
				RegA = 1;
				break;
			}
			state.board[x_coord][y_coord] = Cell::light;
			this->moved = true;
		}
		coord = reset;
	};
	switch (ex.code) {
	 case xpr::move:{
		switch (this->dir) {
		 case facing::up:{   try_move(y_coord, false); }break;
		 case facing::down:{ try_move(y_coord, true ); }break;
		 case facing::left:{ try_move(x_coord, false); }break;
		 case facing::right:{try_move(x_coord, true ); }break;
		 default: throw std::runtime_error("Unknown Direction in step");
		}
	 }break;
	 case xpr::look:{
		 switch (this->dir) {
		  case facing::up: {   look_dir(y_coord, false); }break;
		  case facing::down: { look_dir(y_coord, true ); }break;
		  case facing::left: { look_dir(x_coord, false); }break;
		  case facing::right: {look_dir(x_coord, true ); }break;
		  default: throw std::runtime_error("Unknown Direction in step");
		 }
	 }break;
	 case xpr::trnl: {
		 // could have used a plain enum and wrapped it but w/e its readable if more complicated
		 if      (dir == facing::down)  dir = facing::left;
		 else if (dir == facing::left)  dir = facing::up;
		 else if (dir == facing::up)    dir = facing::right;
		 else if (dir == facing::right) dir = facing::down;
	 }break;
	 case xpr::trnr: {
		 if      (dir == facing::down)  dir = facing::right;
		 else if (dir == facing::right) dir = facing::up;
		 else if (dir == facing::up)    dir = facing::left;
		 else if (dir == facing::left)  dir = facing::down;
	 }break;
	 case xpr::jump: {program_counter = ex.opt1; }break;
	 case xpr::jltA: {if (RegA < ex.opt1) program_counter = ex.opt2; }break;
	 case xpr::jltB: {if (RegB < ex.opt1) program_counter = ex.opt2; }break;
	 case xpr::jgtA: {if (RegA > ex.opt1) program_counter = ex.opt2; }break;
	 case xpr::jgtB: {if (RegB > ex.opt1) program_counter = ex.opt2; }break;
	 case xpr::jmpB: { program_counter = RegB; }break;
	 case xpr::jmpA: { program_counter = RegA; }break;
	 case xpr::addA:{ 
		 int tmp = RegA + ex.opt1;
		 if (tmp > std::numeric_limits<decltype(RegA)>::max()) {
			 fault = true;
			 RegA = 0;
		 }
		 else {
			 RegA = tmp;
		 }
	 }break;
	 case xpr::addB: {
		 int tmp = RegB + ex.opt1;
		 if (tmp > std::numeric_limits<decltype(RegB)>::max()) {
			 fault = true;
			 RegB = 0;
		 }
		 else {
			 RegB = tmp;
		 }
	 }break;
	 case xpr::subA:{
		 int tmp = RegA - ex.opt1;
		 if (tmp < 0) {
			 fault = true;
			 RegA = 0;
		 }else {
			 RegA = tmp;
		 }
	 }break;
	 case xpr::subB:{
		 int tmp = RegB - ex.opt1;
		 if (tmp < 0) {
			 fault = true;
			 RegB = 0;
		 }else {
			 RegB = tmp;
		 }
	 }break;
	 case xpr::negA: {RegA = !RegA;       }break;
	 case xpr::negB: {RegB = !RegB;       }break;
	 case xpr::setA: {RegA = ex.opt1;     }break;
	 case xpr::setB: {RegB = ex.opt1;     }break;
	 case xpr::swap:{std::swap(RegA,RegB);}break;
	 default:;// fmt::print("code:{}", (int)ex.code); throw std::runtime_error("Unknown Code in step");
	}
	if (program_counter >= ai_script.size()-1) {
		fault = true;
	}
	if (fault) {
		program_counter = 0;
	}else {
		program_counter++;
	}
}

void Person::reset() { selectability = 10; program_counter = 0; won_count = 0; }

void Person::random_genome(gen_type gen){
	this->ai_script.clear();
	std::uniform_int_distribution<> linedist(20, 40);
	int num_lines = linedist(gen);
	std::uniform_int_distribution<short> smalldist(0, std::numeric_limits<uint8_t>::max());
	std::uniform_int_distribution<short> optdist(0, xpr::MAX_CODE);
	this->ai_script.emplace_back(xpr::move, (uint8_t)smalldist(gen), (uint8_t)smalldist(gen));
	for (size_t _ = 0; _ < num_lines; ++_) {
		this->ai_script.emplace_back((xpr::code)optdist(gen), (uint8_t)smalldist(gen), (uint8_t)smalldist(gen));
	}
}
void Person::mutate_genome(gen_type gen){
	reset();
	std::uniform_int_distribution<short> smalldist(0, std::numeric_limits<uint8_t>::max());
	std::uniform_int_distribution<> linedistfull(0, this->ai_script.size() - 1);
	
	std::uniform_int_distribution<short> optdist(0, xpr::MAX_CODE);
	std::uniform_int_distribution<> editdist(-3, 3);
	int num_lines = editdist(gen) + 8;
	
	while (num_lines > 0) {
		num_lines--;
		size_t line_to_edit = linedistfull(gen);
		ai_script[line_to_edit].code = (xpr::code)optdist(gen);
		ai_script[line_to_edit].opt1 = smalldist(gen);
		ai_script[line_to_edit].opt2 = smalldist(gen);
	}
	num_lines = editdist(gen);
	if (ai_script.size() < 10) {
		num_lines += 20;
	}
	if (num_lines < 0) {
		std::uniform_int_distribution<> linedistsafe(0, this->ai_script.size() - 2 + num_lines);
		while (num_lines < 0) {
			num_lines++;
			size_t line_to_edit = linedistsafe(gen);
			ai_script.erase(ai_script.begin() + line_to_edit);
		}
	}else if (num_lines > 0) {
		while (num_lines > 0) {
			num_lines--;
			size_t line_to_edit = linedistfull(gen);
			ai_script.emplace(ai_script.begin() + line_to_edit, (xpr::code)optdist(gen), smalldist(gen), smalldist(gen));
		}
	}
}
void Person::printScript() {
	for (auto& line : this->ai_script) {
		line.println();
	}
}
void Person::cross_genome(Person &Parent2, gen_type gen){
	reset();
	std::uniform_int_distribution<> linedistsafe(0, this->ai_script.size()/2 - 3);
	std::uniform_int_distribution<> linedistsafeother(0, Parent2.ai_script.size() / 2 - 3);
	auto start = linedistsafe(gen);
	auto lines = linedistsafe(gen);
	ai_script.erase(ai_script.begin() + start, ai_script.begin() + start + lines);
	auto start2 = linedistsafeother(gen);
	auto lines2 = linedistsafeother(gen);
	ai_script.insert(ai_script.begin() + start, Parent2.ai_script.begin() + start2, Parent2.ai_script.begin() + start2 + lines2);
}
