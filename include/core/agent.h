#ifndef AGENT
#define AGENT

#include "components/board.h"
#include "components/node.h"

class Agent {
	private:
		Node *root;
		Node *curr_node;
		int max_searches;
		Board board;
		double start_time;
		double end_time;

	public:
		Agent();

		void update_root(Point move);

		void visit(Node *node);

		Node *chosen_child();

		Node *best_child();

		void expand_curr_node();

		int roll_out();

		void back_propagate(int reward);

		void search(Point move);

		void game_start();
};

#endif