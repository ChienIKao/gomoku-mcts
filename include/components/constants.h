#ifndef CONSTATNTS
#define CONSTATNTS

#include <vector>

typedef std::pair<int, int> Point;
typedef std::vector<std::pair<int, int>> Points;

enum Color { BLACK, WHITE, EMPTY = -1 };
constexpr Color operator~(Color c) { return Color(c ^ 1); }

#endif