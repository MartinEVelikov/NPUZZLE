#ifndef BOARD
#define BOARD
#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <string>
#include <algorithm>
#include <limits>
#include <chrono>
using namespace std;
class Board
{
private:
	vector<vector<int>> solution;
	vector<vector<int>> matrix;
	tuple<int, int> positionOfZero;
public:
	int totalCost;
	int heuristics;
	Board* parent = nullptr;
	string move;
	Board(vector<vector<int>>, int, string, Board* =nullptr, int=0);
	Board(vector<vector<int>>, tuple<int, int>, string, vector<vector<int>>, Board* _parent=nullptr, int _totalCost=0);
	Board(Board _board, Board* _parent, int _totalCost)
	{
		this->matrix = _board.matrix;
		this->solution = _board.solution;
		this->positionOfZero = _board.positionOfZero;
		this->move = _board.move;
		this->heuristics = _board.heuristics;
		this->totalCost = _board.totalCost;
		this->parent = _board.parent;
	}
	Board(const Board& other)
	{
		solution = other.solution;
		matrix = other.matrix;
		positionOfZero = other.positionOfZero;
		move = other.move;
		totalCost = other.totalCost;
		heuristics = other.heuristics;
		parent = other.parent;
	}

	void generate_solution(int n, int goalPositionOfZero);
	tuple<int, int> find_position(const vector<vector<int>>& matrix, int number);
	Board swap(const string& position, int rowCoord, int colCoord);
	vector<Board> generate_moves();
	bool is_solvable();
	tuple<vector<int>, int> mergeSort_inversions(vector<int>);
	bool is_solution() { return matrix == solution; }
	int manhattan_distance_and_linear_conflicts();

};


#endif