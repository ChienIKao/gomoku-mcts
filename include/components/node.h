#ifndef NODE
#define NODE

#include "components/constants.h"

class Node {
	private:
		Node *parent = nullptr;
		std::vector<Node *> children;
		Color color = WHITE;
		Point move;
		int depth = 0;
		int visits = 0;
		long long value = 0;

	public:
		Node();
		Node(Color color, int depth, Point move);
		Node(Node* parent, Color color, int depth, Point move);

        Node* getParent();
		void setParent(Node *parent);

		std::vector<Node *> getChildren();
		void addChildren(Node* child);

		Point getMove();

        void setVisits(int n);
        int getVisits();
		
		int getValue();
		void setValue(int n);

		Color getColor();

		int getDepth();
		void addDepth(int n);
};
#endif