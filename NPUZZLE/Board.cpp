#include "Board.hpp"
static int INF = std::numeric_limits<int>::max();
vector<int> slice(vector<int>& arr,
	int X, int Y)
{
	auto start = arr.begin() + X;
	auto end = arr.begin() + Y + 1;
	vector<int> result(Y - X + 1);
	copy(start, end, result.begin());
	return result;
}
Board::Board(vector<vector<int>> _matrix, int goalPositionOfZero, string _move, Board* _parent, int _totalCost)
{
	size_t size = _matrix.size();
	for (size_t i = 0; i < size; i++)
	{
		matrix.push_back({});
		for (size_t j = 0; j < size; j++)
		{
			matrix[i].push_back(_matrix[i][j]);
		}
	}
	positionOfZero = find_position(matrix, 0);
	move = _move;
	if (solution.empty())
	{
		generate_solution(size, goalPositionOfZero);
	}
	heuristics = manhattan_distance_and_linear_conflicts();
	totalCost = _totalCost;
	parent = _parent;

}

Board::Board(vector<vector<int>> _matrix, tuple<int, int> _positionOfZero,
	string _move, vector<vector<int>> _solution, Board* _parent, int _totalCost)
{
	matrix = _matrix;
	move = _move;
	positionOfZero = _positionOfZero;
	solution = _solution;
	heuristics = manhattan_distance_and_linear_conflicts();
	parent = _parent;
	totalCost = _totalCost;
}


void Board::generate_solution(int n, int goalPositionOfZero)
{
	solution.clear();
	int counter = 1;
	for (int row = 0; row < n; row++)
	{
		solution.push_back({});
		for (int col = 0; col < n; col++)
		{
			solution[row].push_back(counter);
			counter++;
		}
	}
	if (goalPositionOfZero == -1)
	{
		solution[n - 1][n - 1] = 0;
	}
	else if (goalPositionOfZero < -1 || goalPositionOfZero > n * n - 1)
	{
		cout << "Wrong input of position of zero!!!" << endl;
		exit(-1);
	}
	else
	{
		int rowCoordGoal = goalPositionOfZero / n;
		int colCoordGoal = goalPositionOfZero % n;
		solution[rowCoordGoal][colCoordGoal] = 0;

		int x = rowCoordGoal + (colCoordGoal + 1) / n;
		int y = (colCoordGoal + 1) % n;
		for (x; x < n; x++)
		{
			for (y; y < n; y++)
			{
				solution[x][y]--;
			}
			y = 0;
		}
	}
}

tuple<int, int> Board::find_position(const vector<vector<int>>& matrix, int number)
{
	int x, y;
	bool found = false;
	for (size_t row = 0; (row < matrix.size() && found == false); row++)
	{
		for (size_t col = 0; col < matrix[row].size(); col++)
		{
			if (matrix[row][col] == number)
			{
				x = row;
				y = col;
				found = true;
				break;
			}
		}
	}
	tuple<int, int> result(x, y);
	return result;
}
Board Board::swap(const string& position, int rowCoord, int colCoord)
{
	vector<vector<int>> newMatrix;
	newMatrix = this->matrix;
	int newRowCoord = 0;
	int newColCoord = 0;
	if (position == "down")
	{
		newMatrix[rowCoord][colCoord] = newMatrix[rowCoord - 1][colCoord];
		newMatrix[rowCoord - 1][colCoord] = 0;
		newRowCoord = rowCoord - 1;
		newColCoord = colCoord;
	}
	else if (position == "up")
	{
		newMatrix[rowCoord][colCoord] = newMatrix[rowCoord + 1][colCoord];
		newMatrix[rowCoord + 1][colCoord] = 0;
		newRowCoord = rowCoord + 1;
		newColCoord = colCoord;
	}
	else if (position == "right")
	{
		newMatrix[rowCoord][colCoord] = newMatrix[rowCoord][colCoord - 1];
		newMatrix[rowCoord][colCoord - 1] = 0;
		newRowCoord = rowCoord;
		newColCoord = colCoord - 1;
	}

	else if (position == "left")
	{
		newMatrix[rowCoord][colCoord] = newMatrix[rowCoord][colCoord + 1];
		newMatrix[rowCoord][colCoord + 1] = 0;
		newRowCoord = rowCoord;
		newColCoord = colCoord + 1;
	}
	else
	{
		cout << "Incorrect usage of swap function" << endl;
		exit(-1);
	}
	tuple<int, int> newZeroCoords(newRowCoord, newColCoord);
	Board result(newMatrix, newZeroCoords, position, solution, this);
	return result;
}
vector<Board> Board::generate_moves()
{
	vector<Board> vectorOfBoards;
	int rowPositionOfZero = get<0>(this->positionOfZero);
	int colPositionOfZero = get<1>(this->positionOfZero);

	if ((size_t)(rowPositionOfZero + 1) < matrix.size())
	{
		vectorOfBoards.push_back(this->swap("up", rowPositionOfZero, colPositionOfZero));
	}


	if ((rowPositionOfZero - 1) >= 0)
	{
		vectorOfBoards.push_back(this->swap("down", rowPositionOfZero, colPositionOfZero));
	}


	if ((size_t)(colPositionOfZero + 1) < matrix.size())
	{
		vectorOfBoards.push_back(this->swap("left", rowPositionOfZero, colPositionOfZero));
	}


	if (colPositionOfZero - 1 >= 0)
	{
		vectorOfBoards.push_back(this->swap("right", rowPositionOfZero, colPositionOfZero));
	}

	return vectorOfBoards;
}

bool Board::is_solvable()
{
	vector<int> elements;
	for (auto& row : this->matrix)
	{
		for (auto& elem : row)
		{
			elements.push_back(elem);
		}
	}

	size_t i = 0;
	for (i; i < (elements.size()); i++)
	{
		if (elements[i] == 0)
		{
			break;
		}
	}
	if (i == elements.size() - 1)
	{
		elements.erase(elements.end() - 1);
	}
	else
	{
		elements.erase(elements.begin() + i);
	}

	tuple<vector<int>, int> sortedVectorAndInversions = this->mergeSort_inversions(elements);
	int size = this->matrix.size();
	int permutations = get<1>(sortedVectorAndInversions);

	return
		(size % 2 != 0 && permutations % 2 == 0) ||
		(size % 2 == 0 && ((permutations + get<0>(this->positionOfZero)) % 2 != 0));
}
tuple<vector<int>, int> Board::mergeSort_inversions(vector<int> vect)
{

	if (vect.size() <= 1)
	{
		return tuple<vector<int>, int>(vect, 0);
	}
	else
	{
		vector<int> firstHalf = slice(vect, 0, vect.size() / 2 - 1);
		vector<int> secondHalf = slice(vect, vect.size() / 2, vect.size() - 1);
		tuple<vector<int>, int> resultFirstHalf = mergeSort_inversions(firstHalf);
		tuple<vector<int>, int> resultSecondHalf = mergeSort_inversions(secondHalf);
		vector<int> result;
		size_t i = 0;
		size_t j = 0;
		int inversionsFirstHalf = get<1>(resultFirstHalf);
		int inversionsSecondHalf = get<1>(resultSecondHalf);
		vector<int> vectorResultFirstHalf = get<0>(resultFirstHalf);
		vector<int> vectorResultSecondHalf = get<0>(resultSecondHalf);
		int inversions = 0 + inversionsFirstHalf + inversionsSecondHalf;
		while (i < vectorResultFirstHalf.size() && j < vectorResultSecondHalf.size())
		{
			if (vectorResultFirstHalf[i] <= vectorResultSecondHalf[j])
			{
				result.push_back(vectorResultFirstHalf[i]);
				i++;
			}
			else
			{
				result.push_back(vectorResultSecondHalf[j]);
				j++;
				inversions += (vectorResultFirstHalf.size() - i);
			}
		}
		for (auto& elem : slice(vectorResultFirstHalf, i, vectorResultFirstHalf.size() - 1))
		{
			result.push_back(elem);
		}
		for (auto& elem : slice(vectorResultSecondHalf, j, vectorResultSecondHalf.size() - 1))
		{
			result.push_back(elem);
		}
		return tuple<vector<int>, int>(result, inversions);
	}
}
/*int Board::manhattan_distance()
{
	int manhattanDistance = 0;
	for (int i = 1; i<pow(matrix.size(), 2); i++)
	{
		int current_pos_row = get<0>(this->find_position(this->matrix, i));
		int current_pos_col = get<1>(this->find_position(this->matrix, i));
		int ideal_pos_row = get<0>(this->find_position(this->solution, i));
		int ideal_pos_col = get<1>(this->find_position(this->solution, i));

		manhattanDistance += abs(current_pos_row - ideal_pos_row) + abs(current_pos_col - ideal_pos_col);
	}
	return manhattanDistance;
}*/
int Board::manhattan_distance_and_linear_conflicts()
{

	int numOfBlocks = (int)pow(matrix.size(), 2);
	int sqrtNumOfBlocks = matrix.size();
	int verticalConflicts = 0;
	int horizontalConflicts = 0;
	std::vector<int> distsI(numOfBlocks, 0);
	std::vector<int> distsJ(numOfBlocks, 0);

	for (int i = 0; i < sqrtNumOfBlocks; ++i)
	{
		for (int j = 0; j < sqrtNumOfBlocks; ++j)
		{
			distsI[matrix[i][j]] += i;
			distsJ[matrix[i][j]] += j;
			distsI[solution[i][j]] -= i;
			distsJ[solution[i][j]] -= j;
		}
	}


	for (int i = 0; i < sqrtNumOfBlocks; ++i)
	{
		int maxByRow = -1, maxByCol = -1;
		for (int j = 0; j < sqrtNumOfBlocks; ++j)
		{
			if (matrix[i][j] != 0 && distsI[matrix[i][j]] == 0)
			{
				if (matrix[i][j] > maxByRow)
				{
					maxByRow = matrix[i][j];
				}
				else
				{
					horizontalConflicts += 2;
				}
			}

			if (matrix[j][i] != 0 && distsJ[matrix[j][i]] == 0)
			{
				if (matrix[j][i] > maxByCol)
				{
					maxByCol = matrix[j][i];
				}
				else
				{
					verticalConflicts += 2;
				}
			}
		}
	}
	heuristics = verticalConflicts + horizontalConflicts;

	for (int i = 1; i < numOfBlocks; ++i)
	{
		heuristics += abs(distsI[i]) + abs(distsJ[i]);
	}
	return heuristics;
}