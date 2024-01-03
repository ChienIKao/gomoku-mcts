#include "components/node.h"

using namespace std;

Node::Node()
    : parent(nullptr),
      children(vector<Node*>()),
      color(Color::WHITE),
      move(make_pair(-1, -1)),
      depth(0),
      visits(0),
      value(0) {}

Node::Node(Color color, int depth, Point move) {
	Node();
	this->color = color;
	this->depth = depth;
	this->move = move;
}

Node::Node(Node* parent, Color color, int depth, Point move) {
    Node();
    this->parent = parent;
    this->color = color;
    this->depth = depth;
    this->move = move;
}

Node* Node::getParent() { return this->parent; }
void Node::setParent(Node* parent) { this->parent = parent; }

vector<Node*> Node::getChildren() { return this->children; }
void Node::addChildren(Node* child) { this->children.push_back(child); }

Point Node::getMove() { return this->move; }

void Node::setVisits(int n) { this->visits = n; }
int Node::getVisits() { return this->visits; }

int Node::getValue() { return this->value; }
void Node::setValue(int n) { this->value = n; }

Color Node::getColor() { return this->color; }

int Node::getDepth() { return this->depth; }
void Node::addDepth(int n) { this->depth += n; }