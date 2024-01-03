#include "components/board.h"

#include <algorithm>
#include <iostream>
using namespace std;

Board::Board() : size(15), win_len(5) {
	board = vector<vector<Color>>(size, vector<Color>(size, Color::EMPTY));
	moves = vector<Point>();
	now_playing = Color::BLACK;
	winner = Color::EMPTY;
}

bool Board::is_legal(Point move) {
	int i = move.first, j = move.second;
	bool is_inside =
	    ((i >= 0) && (i < this->size) && (j >= 0) && (j < this->size)) ? true
	                                                                   : false;
	if (!is_inside) return false;

	bool is_empty = (this->board[i][j] == Color::EMPTY) ? true : false;
	if (!is_empty) return false;

	return true;
}

void Board::play_stone(Point move) {
	int i = move.first, j = move.second;
	if (!this->is_legal(move))
		printf("Cannot play a stone at (%d, %d).\n", i, j);
	else {
		this->board[i][j] = this->now_playing;
		this->moves.push_back(move);
		this->now_playing = ~this->now_playing;
	}
}

void Board::display_board(Point move) {
	for (int i = 0; i < this->size; i++) {
		printf("%2d ", 15 - i);
		for (int j = 0; j < this->size; j++) {
			if (j != 0) printf("─");

			if (i == move.first && j == move.second) {
				if (this->board[i][j] == Color::BLACK) {
					printf("\033[31m◯\033[0m");
					continue;
				} else if (this->board[i][j] == Color::WHITE) {
					printf("\033[31m⬤\033[0m");
					continue;
				}
			} else {
				if (this->board[i][j] == Color::BLACK) {
					printf("◯");
					continue;
				} else if (this->board[i][j] == Color::WHITE) {
					printf("⬤");
					continue;
				}
			}

			if (i == 0 && j == 0) {
				printf("┌");
				continue;
			}
			if (i == 0 && j == 14) {
				printf("┐");
				continue;
			}
			if (i == 14 && j == 0) {
				printf("└");
				continue;
			}
			if (i == 14 && j == 14) {
				printf("┘");
				continue;
			}
			if (i == 0) {
				printf("┬");
				continue;
			}
			if (i == 14) {
				printf("┴");
				continue;
			}
			if (j == 0) {
				printf("├");
				continue;
			}
			if (j == 14) {
				printf("┤");
				continue;
			}
			printf("┼");
		}
		printf("\n");
	}
	printf("   A B C D E F G H I J K L M N O\n");
}

vector<short> Board::adj_empty() {
	unordered_set<short> occupied;
	unordered_set<short> vacancies;
	vector<short> res;

	if (this->moves.size() != 0) {
		for (Point move : this->moves)
			occupied.insert(move.first * 100 + move.second);

		int bias[4] = {-1, 0, 1, 2};
		for (Point move : this->moves) {
			for (int i : bias) {
				if (move.first - i < 0 || move.first - i >= this->size)
					continue;
				for (int j : bias) {
					if (move.second - j < 0 || move.second - j >= this->size)
						continue;
					vacancies.insert((move.first - i) * 100 +
					                 (move.second - j));
				}
			}
		}

		for (int occ : occupied) vacancies.erase(occ);
		for (auto vac : vacancies) res.push_back(vac);
	}
	return res;
}

bool Board::is_ended() {
	if (this->moves.size() == 0) return false;

	Point loc = this->moves[this->moves.size() - 1];
	Color color = ~this->now_playing;
	int sgn_i[4] = {1, 0, 1, 1};
	int sgn_j[4] = {0, 1, 1, -1};
	for (int it = 0; it < 4; it++) {
		int length = 0;
		int prm1 = (sgn_i[it] == 1 ? loc.first : loc.second);
		int prm2 =
		    (sgn_j[it] == 1
		         ? loc.second
		         : (sgn_j[it] == 0 ? loc.first : this->size - 1 - loc.second));
		int start_bias =
		    (min(prm1, prm2) < this->win_len - 1 ? -min(prm1, prm2)
		                                         : -this->win_len + 1);
		int end_bias = (max(prm1, prm2) > (this->size - this->win_len)
		                    ? this->size - 1 - max(prm1, prm2)
		                    : this->win_len - 1);
		for (int k = start_bias; k <= end_bias; k++) {
			Color stone = this->board[loc.first + k * sgn_i[it]]
			                         [loc.second + k * sgn_j[it]];
			if (color == stone && stone != Color::EMPTY)
				length++;
			else
				length = 0;

			if (length == this->win_len) {
				this->winner = color;
				return true;
			}
		}
	}

	if (this->moves.size() == this->size * this->size)
		return true;
	else
		return false;
}

void Board::setWinner(Color color) { this->winner = color; }
Color Board::getWinner() { return this->winner; }

void Board::setMoves(vector<Point> moves) { this->moves = moves; }
vector<Point> Board::getMoves() { return this->moves; }

void Board::resetBoard() {
	for (int i = 0; i < this->size; i++) {
		for (int j = 0; j < this->size; j++) {
			this->board[i][j] = Color::EMPTY;
		}
	}
}

void Board::setBoard(int i, int j, Color color) { this->board[i][j] = color; }

void Board::set_now_playing(Color color) { this->now_playing = color; }

void Board::setBoardSize(int n) { this->size = n; }
int Board::getBoardSize() { return this->size; }

Color Board::getNowPlaying() { return this->now_playing; }

void Board::setAIColor(Color color) { this->AI_color = color; }
Color Board::getAIColor() { return this->AI_color; }