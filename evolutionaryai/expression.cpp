#include "expression.h"

const void Expression::println() {
	switch (code) {
	 case xpr::move: fmt::print("move\n"); break;
	 case xpr::look: fmt::print("look\n"); break;
	 case xpr::trnr: fmt::print("trnr\n"); break;
	 case xpr::trnl: fmt::print("trnl\n"); break;
	 case xpr::jump: fmt::print("jump : {:d}, {:d}\n", (int)opt1, (int)opt2); break;
	 case xpr::jltA: fmt::print("jltA : {:d}, {:d}\n", (int)opt1, (int)opt2); break;
	 case xpr::jltB: fmt::print("jltB : {:d}, {:d}\n", (int)opt1, (int)opt2); break;
	 case xpr::jgtA: fmt::print("jgtA : {:d}, {:d}\n", (int)opt1, (int)opt2); break;
	 case xpr::jgtB: fmt::print("jgtB : {:d}, {:d}\n", (int)opt1, (int)opt2); break;
	 case xpr::jmpA: fmt::print("jmpA\n"); break;
	 case xpr::jmpB: fmt::print("jmpB\n"); break;
	 case xpr::negA: fmt::print("negA\n"); break;
	 case xpr::negB: fmt::print("negB\n"); break;
	 case xpr::addA: fmt::print("addA : {:d}\n", (int)opt1); break;
	 case xpr::addB: fmt::print("addB : {:d}\n", (int)opt1); break;
	 case xpr::setA: fmt::print("setA : {:d}\n", (int)opt1); break;
	 case xpr::setB: fmt::print("setB : {:d}\n", (int)opt1); break;
	 case xpr::subA: fmt::print("subA : {:d}\n", (int)opt1); break;
	 case xpr::subB: fmt::print("subB : {:d}\n", (int)opt1); break;
	 case xpr::swap: fmt::print("swap\n"); break;
	 default: fmt::print("err\n");
	}
}
