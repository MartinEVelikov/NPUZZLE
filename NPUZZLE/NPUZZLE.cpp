#include "Board.hpp"
#include "SlidingBlocks.hpp"

using namespace std;
int main()
{
	cout << "Input number of blocks: " << endl;
	int num_of_blocks;
	cin >> num_of_blocks;
	int block_matrix_dimension = (int)sqrt(num_of_blocks + 1);
	vector<vector<int>> matrix;
	cout << "Input position of zero: " << endl;
	int pos_of_zero;
	cin >> pos_of_zero;
	int elem;
	for (int i = 0; i < block_matrix_dimension; i++)
	{
		matrix.push_back({});
		for (int j = 0; j < block_matrix_dimension; j++)
		{
			cin >> elem;
			matrix[i].push_back(elem);
		}
	}

	Board board(matrix, pos_of_zero, "start");
	auto start = std::chrono::high_resolution_clock::now();
	if (board.is_solvable())
	{
		SlidingBlocks slidingBlocks(&board);
		slidingBlocks.play();
	}
	else
	{
		cout << "Board is not solvable!" << endl;
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	cout << "Execution Time: " << duration.count() / 1000 << '.' << duration.count() % 1000 << " seconds" << endl;

	return 0;
}

