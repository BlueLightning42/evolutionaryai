#include "board_state.h"

void BoardState::reset(){
	for (auto& row : board) {
		for (auto& c : row) {
			c = Cell::empty;
		}
	}
}
#if defined(WIN32) | defined(_WIN32) | defined(_WIN64)
#define NOMINMAX 1
#include <Windows.h>
void clear_screen() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}
#endif //

const void BoardState::printbrd(){
	clear_screen();
	for (const auto& row : board) {
		for (const auto& cell : row) {
			switch (cell) {
			case Cell::empty:  fmt::print(fg(fmt::color::chocolate), "{}", (char)177); break;
			case Cell::hunter: fmt::print(fg(fmt::color::orange_red), "H"); break;
			case Cell::prey:   fmt::print(fg(fmt::color::cornflower_blue), "P"); break;
			case Cell::light:  fmt::print(fg(fmt::color::light_golden_rod_yellow),"{}", (char)176); break;
			}
		}fmt::print("\n");
	}
}
