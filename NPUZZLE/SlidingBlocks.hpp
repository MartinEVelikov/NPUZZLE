#ifndef SLIDING_BLOCKS
#define SLIDING_BLOCKS

#include "Board.hpp"
class SlidingBlocks
{
private:
	Board* board;

public:
	SlidingBlocks(Board* _board=nullptr);
	void play();
	string ida_star(Board* root);
	string search(vector<Board*>& path, int g, int bound);
	void print_solution(Board* node);
};
#endif
