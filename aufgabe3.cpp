#include "Alignment.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct Tuple
{
	int i;
	int j;
};



/// Constructor with two sequences
/// Makes an internal copy of the sequences.
Alignment::Alignment(const std::string& seq_v, const std::string& seq_h)
{
	this->horizontal = " " + seq_h;
	this->vertikal = " " + seq_v;
	this->matrix.clear();
	this->traceback = fill_None();
}

/// compute the aligment (i.e. score and traceback)
/// given the three alignment parameters match, mismatch and gap
/// if local_align == true, compute the local Smith-Waterman (SW) alignment (extra points), or throw
/// an exception if your implementation does not support SW.
void Alignment::compute(const int match, const int mismatch, const int gap, const bool local_align)
{
	this->matrix = this->fill_null();
	std::vector<char> a(horizontal.size(), 'q');
	std::vector<std::vector<char>> test(vertikal.size(), a);
	
	matrix[0][0] = 0;
	traceback[0][0] = Traceback::NONE;
	//Initialization:
	for (size_t i = 1; i < matrix.size(); i++)
	{
		matrix[i][0] += i*gap;
		traceback[i][0] = Traceback::VERTICAL;
	}
	for (size_t i = 1; i < matrix[0].size(); i++)
	{
		matrix[0][i] += i * gap;
		traceback[0][i] = Traceback::HORIZONTAL;
	}
	//Recurrence:
	for (size_t i = 1; i < matrix.size(); i++)
	{
		for (size_t j = 1; j < matrix[0].size(); j++)
		{
			if (horizontal[j] == vertikal[i])
			{
				//matrix[i][j] = max(matrix[i-1][j-1] + match, max(matrix[i - 1][j] - gap, matrix[i][j - 1] - gap));
				if (matrix[i - 1][j - 1] + match >= matrix[i - 1][j] + gap && matrix[i - 1][j - 1] + match >= matrix[i][j - 1] + gap)
				{
					matrix[i][j] = matrix[i - 1][j - 1] + match;
					traceback[i][j] = Traceback::DIAGONAL;
				}
				else if (matrix[i - 1][j] + gap > matrix[i - 1][j - 1] + match && matrix[i - 1][j] >= matrix[i][j - 1] + gap)
				{
					matrix[i][j] = matrix[i - 1][j] + gap;
					traceback[i][j] = Traceback::VERTICAL;
				}
				else if (matrix[i][j - 1] + gap > matrix[i - 1][j - 1] + match && matrix[i][j - 1] > matrix[i - 1][j] + gap)
				{
					matrix[i][j] = matrix[i][j - 1] + gap;
					traceback[i][j] = Traceback::HORIZONTAL;
				}
			}
			else
			{
				//matrix[i][j] = max(matrix[i-1][j-1] + mismatch, max(matrix[i - 1][j] - gap, matrix[i][j - 1] - gap));
				if (matrix[i - 1][j - 1] + mismatch >= matrix[i - 1][j] + gap && matrix[i - 1][j - 1] + mismatch >= matrix[i][j - 1] + gap)
				{
					matrix[i][j] = matrix[i - 1][j - 1] + mismatch;
					traceback[i][j] = Traceback::DIAGONAL;
				}
				else if (matrix[i - 1][j] + gap > matrix[i - 1][j - 1] + mismatch && matrix[i - 1][j] >= matrix[i][j - 1] + gap)
				{
					matrix[i][j] = matrix[i - 1][j] + gap;
					traceback[i][j] = Traceback::VERTICAL;
				}
				else if (matrix[i][j - 1] + gap > matrix[i - 1][j - 1] + mismatch && matrix[i][j - 1] > matrix[i - 1][j] + gap)
				{
					matrix[i][j] = matrix[i][j - 1] + gap;
					traceback[i][j] = Traceback::HORIZONTAL;
				}
			}
			
		}
	}
	
//	{
//		cout << horizontal << endl;
//		for (size_t i = 0; i < matrix.size(); i++)
//		{
//			cout << vertikal[i] << " ";
//			for (size_t j = 0; j < matrix[0].size(); j++)
//			{
//				cout << matrix[i][j] << " ";
//			}
//			cout << endl;
//		}
//
//		for (size_t i = 0; i < matrix.size(); i++)
//		{
//			for (size_t j = 0; j < matrix[0].size(); j++)
//			{
//				if (traceback[i][j] == Traceback::NONE)
//					test[i][j] = 'N';
//				else if (traceback[i][j] == Traceback::DIAGONAL)
//					test[i][j] = 'D';
//				else if (traceback[i][j] == Traceback::HORIZONTAL)
//					test[i][j] = 'H';
//				else
//					test[i][j] = 'V';
//			}
//			cout << endl;
//		}
//		for (size_t i = 0; i < matrix.size(); i++)
//		{
//			for (size_t j = 0; j < matrix[0].size(); j++)
//			{
//				cout << test[i][j] << " ";
//			}
//			cout << endl;
//		}
//	}
//
}

/// return the score of the alignment;
/// Throws an exception if compute(...) was not called first
int Alignment::score() const
{
	if (!this->matrix.empty())
	{
		return this->matrix[vertikal.size() - 1][horizontal.size() - 1];;
	}
	else {
		throw std::invalid_argument("function compute() was not called before");
	}
}

/// output alignment into three strings.
/// Gaps are denoted as '-' in sequences.
/// The gap-string uses '|' (match) and ' ' (mismatch/gap).
/// Note: all strings are equal in size
/// e.g.
/// a1:   "IMISSMISSIS-SIPPI-"
/// gaps: " |   ||||||  |||| "
/// a2:   "-M--YMISSISAHIPPIE"
/// , where a1 corresponds to seq1, etc.
/// Throws an exception if compute(...) was not called first
void Alignment::getAlignment(std::string& a1, std::string& gaps, std::string& a2) const
{
	if (!this->matrix.empty())
	{
		a1.clear();
		gaps.clear();
		a2.clear();
		Tuple foo = { vertikal.size() - 1,horizontal.size() - 1 };
		int x = 0;
		int y = 0;
		while (foo.i != 0 || foo.j != 0)
		{
			if (traceback[foo.i][foo.j] == Traceback::DIAGONAL)
			{
				a2 += horizontal[foo.j];
				a1 += vertikal[foo.i];


				if (horizontal[foo.j] == vertikal[foo.i])
				{
					gaps += "|";
				}
				else
				{
					gaps += " ";
				}
				foo.i--;
				foo.j--;
			}
			else if (traceback[foo.i][foo.j] == Traceback::HORIZONTAL)
			{
				a2 += horizontal[foo.j];
				a1 += "-";
				gaps += " ";
				foo.j--;

			}
			else
			{
				a2 += "-";
				a1 += vertikal[foo.i];
				gaps += " ";

				foo.i--;
			}

		}

		std::reverse(a1.begin(), a1.end());
		reverse(a2.begin(), a2.end());
		reverse(gaps.begin(), gaps.end());
		cout << a1 << endl;
		cout << gaps << endl;
		cout << a2 << endl;
	}
	else 
	{
		throw std::invalid_argument("function compute() was not called before");
	}
}

//int main()
//{
//	/*string horizontal = "Mert";
//	string vertikal = "akguel";
//	int gap = 2;
//	std::vector<int> rows(horizontal.size(), 0);
//	std::vector<std::vector<int>> matrix(vertikal.size(), rows);
//	matrix[0][0] = 0;
//	for (size_t i = 0; i < matrix.size(); i++)
//	{
//		matrix[i][0] -= i * gap;
//	}
//	for (size_t i = 0; i < matrix[0].size(); i++)
//	{
//		matrix[0][i] -= i * gap;
//	}
//	int x = 42;
//	int y = 24;
//	int z = 69;*/
//	cout << max(x, max(y, z));
//	Alignment a("IMISSMISSISSIPPI", "MYMISSISAHIPPIE");
//	/*Alignment a("ACTG", "ACT");*/
//	a.compute(3, -40, -1);
//	int highscore = a.score();
//	cout << highscore << endl;
//	
//	string s, t, u;
//	a.getAlignment(s,t,u);
//	/*string horizontal = " ACTG";
//	string vertikal = " ACT";
//	std::vector<int> rows(horizontal.size(), 0);
//	std::vector<std::vector<int>> matrix(vertikal.size(), rows);
//	for (size_t i = 0; i < matrix.size(); i++)
//	{
//		for (size_t j = 0; j < matrix[0].size(); j++)
//		{
//			cout << matrix[i][j] << " ";
//		}
//		cout << endl;
//	}
//	return 0;*/
//}