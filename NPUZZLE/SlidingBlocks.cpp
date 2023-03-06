#include "SlidingBlocks.hpp"

SlidingBlocks::SlidingBlocks(Board* _board) : board(_board) {}

void SlidingBlocks::play()
{
	Board* starting_node = this->board;
	this->ida_star(starting_node);
}

string SlidingBlocks::search(vector<Board*>& path, int g, int bound)
{
	Board* currentBoard = path.back();
	currentBoard->totalCost = g + currentBoard->heuristics;

	int minVal = std::numeric_limits<int>::max();
	if (currentBoard->totalCost > bound)
	{
		return to_string(currentBoard->totalCost);
	}
	if (currentBoard->is_solution())
	{
		this->print_solution(currentBoard);

		return "FOUND";
	}

	for (auto& sibling : currentBoard->generate_moves())
	{
		Board* nextBoard = new Board(sibling, currentBoard, 0);
		if (std::count(path.begin(), path.end(), nextBoard) == 0)
		{
			path.push_back(nextBoard);
			string temp = this->search(path, g + 1, bound);
			if (temp == "FOUND")
				return temp;
			if (stoi(temp) < minVal)
				minVal = stoi(temp);
			delete path.back();
			path.pop_back();
		}
	}
	return  to_string(minVal);
}
string SlidingBlocks::ida_star(Board* root)
{
	int bound = root->heuristics;
	vector<Board*>path;
	path.push_back(root);
	while (true)
	{
		string temp = this->search(path, 0, bound);
		if (temp == "FOUND")
		{
			return "FOUND";
		}
		if (stoi(temp) == std::numeric_limits<int>::max())
		{
			return "NOT FOUND";
		}
		bound = stoi(temp);
	}
}

void SlidingBlocks::print_solution(Board* node)
{
	vector<string> steps_list;
	while (node->parent != nullptr)
	{
		steps_list.push_back(node->move);
		node = node->parent;
	}
	cout << steps_list.size() << endl;
	int indexElem = steps_list.size() - 1;
	for (indexElem; indexElem >= 0; indexElem--)
	{
		cout << steps_list[indexElem] << endl;
	}

}