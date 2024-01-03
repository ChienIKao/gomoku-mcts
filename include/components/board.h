#ifndef BOARD
#define BOARD
#include <vector>
#include <unordered_set>

#include "components/constants.h"

class Board {
	private:
		int size = 15;
		int win_len = 5;
		std::vector<std::vector<Color>> board;
		std::vector<Point> moves;
		Color now_playing;
		Color winner;

	public:
		Board();

		bool is_legal(Point move);

		void play_stone(Point move);

		void display_board(Point move);

		std::vector<short> adj_empty();

		bool is_ended();

		void setWinner(Color color);
		Color getWinner();

		void setMoves(std::vector<Point> moves);
		std::vector<Point> getMoves();

		void resetBoard();
		void setBoard(int i, int j, Color color);

		void set_now_playing(Color color);

		void setBoardSize(int n);
		int getBoardSize();
};
#endif
