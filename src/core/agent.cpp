#include "core/agent.h"

#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

Agent::Agent()
    : max_searches(10000),
      win_reward(1),
      lose_reward(-1),
      tie_reward(0),
      show_history('y') {
	root = new Node();
	curr_node = new Node();
	Board board = Board();
}

void Agent::update_root(Point move) {
	vector<Node*> children = this->curr_node->getChildren();
	for (Node* child : children) {
		if (child->getMove() == move) {
			this->root = child;
			this->root->setParent(nullptr);
			this->visit(this->root);
			this->board.display_board(this->curr_node->getMove(),
			                          this->show_history, this->max_searches,
			                          this->win_reward, this->lose_reward,
			                          this->tie_reward);
			printf("Score: %d, Move: [%d, %c]\n", child->getValue(),
			       15 - move.first, 'A' + move.second);
			if (~this->board.getNowPlaying() == this->board.getAIColor()) {
				printf("Search time: %.2lf sec\n",
				       (this->end_time - this->start_time) / CLOCKS_PER_SEC);
			}
			return;
		}
	}

	Node* node = new Node(~this->curr_node->getColor(),
	                      this->curr_node->getDepth() + 1, move);
	this->root = node;
	this->root->setParent(nullptr);
	this->visit(this->root);
	this->board.display_board(this->curr_node->getMove(), this->show_history,
	                          this->max_searches, this->win_reward,
	                          this->lose_reward, this->tie_reward);
	printf("Score: %d, Move: [%d, %c]\n", 0, 15 - move.first,
	       'A' + move.second);
}

void Agent::visit(Node* node) {
	this->curr_node = node;
	this->curr_node->setVisits(this->curr_node->getVisits() + 1);
	this->board.play_stone(node->getMove());
}

double UCB(int value, int visits, int total_visits) {
	return ((double)value / (double)visits) +
	       sqrt(2 * log(total_visits) / visits);
}

Node* Agent::chosen_child() {
	time_t t;
	srand((unsigned)time(&t));

	vector<Node*> zero_visits;
	vector<double> UCB_list;
	vector<Node*> children = this->curr_node->getChildren();
	int total_visits = 0;
	if (children.size() != 0) {
		for (Node* child : children) {
			total_visits += child->getVisits();
			if (child->getVisits() == 0) zero_visits.push_back(child);
		}
	}

	if (zero_visits.size() != 0) {
		return zero_visits[rand() % zero_visits.size()];
	} else {
		for (Node* child : children) {
			UCB_list.push_back(
			    UCB(child->getValue(), child->getVisits(), total_visits));
		}
		auto maxElementIt = max_element(UCB_list.begin(), UCB_list.end());
		int idxmax = distance(UCB_list.begin(), maxElementIt);
		return children[idxmax];
	}
}

Node* Agent::best_child() {
	vector<Node*> children = curr_node->getChildren();
	vector<int> visits_list;

	for (Node* child : children) visits_list.push_back(child->getVisits());
	auto maxElementIt = max_element(visits_list.begin(), visits_list.end());
	int idxmax = distance(visits_list.begin(), maxElementIt);
	return children[idxmax];
}

void Agent::expand_curr_node() {
	/* Expand a visited but has no child node with possible moves. */

	vector<short> vacancies = this->board.adj_empty();
	for (auto move : vacancies) {
		Node* parent = this->curr_node;
		Color color = ~this->curr_node->getColor();
		int depth = this->curr_node->getDepth() + 1;
		int i = move / 100, j = move % 100;
		Point nextmove = make_pair(i, j);
		Node* child = new Node(parent, color, depth, nextmove);
		this->curr_node->addChildren(child);
	}
}

int Agent::roll_out() {
	/* Randomly play the rest of the game with itself and return the reward.
	The reward the leaf node gets. `1` for a win, `-1` for a lose and `0` for a
	draw. */

	time_t t;
	srand((unsigned)time(&t));

	while (!this->board.is_ended()) {
		vector<short> vacancies = this->board.adj_empty();
		short loc = vacancies[rand() % vacancies.size()];
		Point move = make_pair(loc / 100, loc % 100);
		this->board.play_stone(move);
	}

	// if (this->board.getWinner() == this->curr_node->getColor())
	if (this->board.getWinner() == this->board.getAIColor())
		return this->win_reward;
	else if (this->board.getWinner() == ~this->board.getAIColor())
		return this->lose_reward;
	else
		return this->tie_reward;
}

void Agent::back_propagate(int reward) {
	/* Update all values on the way back to the root. */

	while (this->curr_node->getParent() != nullptr) {
		this->curr_node->setValue(this->curr_node->getValue() + reward);
		this->curr_node = this->curr_node->getParent();
		reward = reward * -1;
	}
}

void Agent::search(Point move) {
	/* Search the best move according to current state and play a stone. */

	this->update_root(move);
	if (this->board.is_ended()) return;

	this->start_time = clock();
	for (int s = 0; s < this->max_searches; s++) {
		printf("Searching: %.0lf%\r",
		       round((double)s * 100 / this->max_searches));
		while (this->curr_node->getChildren().size() != 0 &&
		       !this->board.is_ended())
			this->visit(this->chosen_child());
		if (this->curr_node->getVisits() > 0) {
			this->expand_curr_node();
			this->visit(this->chosen_child());
		}
		this->back_propagate(this->roll_out());
		vector<Point> moves = this->board.getMoves();
		moves.erase(moves.begin() + this->root->getDepth(), moves.end());
		this->board.setMoves(moves);
		this->board.resetBoard();

		Color color = BLACK;
		for (Point move : this->board.getMoves()) {
			this->board.setBoard(move.first, move.second, color);
			color = ~color;
		}
		this->board.set_now_playing(~this->root->getColor());
		this->board.setWinner(Color::EMPTY);
	}
	Point best_move = this->best_child()->getMove();
	this->end_time = clock();
	this->update_root(best_move);
}

void Agent::printConf() {
	printf("\nConfiguration\n");
	printf("- Max search: %d\n", this->max_searches);
	printf("- Win reward: %d\n", this->win_reward);
	printf("- Lose reward: %d\n", this->lose_reward);
	printf("- Tie reward: %d\n", this->tie_reward);
	printf("- Show history: %c\n\n", this->show_history);
}

void Agent::game_start() {
	printConf();

	char setConf;
	printf("Do you want to set configuration (y/n) ? ");
	setConf = getchar();

	if (setConf == 'y' || setConf == 'Y') {
		printf("Set max search: ");
		scanf("%d", &this->max_searches);

		printf("Set win reward: ");
		scanf("%d", &this->win_reward);

		printf("Set lose reward: ");
		scanf("%d", &this->lose_reward);

		printf("Set tie reward: ");
		scanf("%d", &this->tie_reward);

		printf("Show history (y/n) ? ");
		getchar();
		this->show_history = getchar();
	}

	system("cls");
	printConf();

	char player_color;
	while (true) {
		getchar();
		printf("Please choose black(b) or white(w) ? ");
		scanf("%c", &player_color);
		getchar();
		if (player_color == 'w' || player_color == 'W' || player_color == 'b' ||
		    player_color == 'B')
			break;
		else
			printf("Invalid input! Please try again!\n");
	}

	if (player_color == 'w' || player_color == 'W') {
		this->board.setAIColor(Color::BLACK);
		int center = round((this->board.getBoardSize() - 1) * 0.5);
		this->update_root(make_pair(center, center));
	} else {
		this->board.setAIColor(Color::WHITE);
		this->board.display_board(
		    this->curr_node->getMove(), this->show_history, this->max_searches,
		    this->win_reward, this->lose_reward, this->tie_reward);
	}

	while (!this->board.is_ended()) {
		int i, j;
		char c;
		printf("Enter move [row, col]: ");
		scanf("%d %c", &i, &c);
		i = 15 - i;
		j = c - 'A';
		while (!this->board.is_legal(make_pair(i, j))) {
			printf("Invalid move! Please try again.\n");
			printf("Enter move [row, col]: ");
			scanf("%d %c", &i, &c);
			i = 15 - i;
			j = c - 'A';
		}
		this->search(make_pair(i, j));
	}

	if (this->board.getWinner() == Color::BLACK)
		printf("Black wins!\n");
	else
		printf("White wins!\n");
}