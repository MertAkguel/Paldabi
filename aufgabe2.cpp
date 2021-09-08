#include "aufgabe2.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>


struct help
{
	int index;
	int zahl;
	int zahl2;
	int laenge;
};

constexpr bool help_for_sort(const help a, const help b)
{
	if (a.zahl < b.zahl)
	{
		return true;
	}
	else if (a.zahl == b.zahl && a.zahl2 < b.zahl2)
	{
		return true;
	}
	else if (a.zahl == b.zahl && a.zahl2 == b.zahl2 && a.laenge < b.laenge)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::string create_suftab(const std::vector<uint32_t>& sa, const std::string& text,
	const std::string& query, const int sa_index, const int start = 0)
{
	std::string zwsp = "";
	const unsigned len = std::min(query.size(), text.size() - sa[sa_index]);
	for (unsigned i = start; i < len; i++)
	{
		zwsp += text[sa[sa_index] + i];
	}
	return zwsp;
}

int anzahl(const std::vector<uint32_t>& sa, const std::string& text,
	const std::string& query, const int sa_index)
{
	int output = 0;
	std::string zwsp = create_suftab(sa, text, query, sa_index);
	const unsigned len = std::min(query.size(), text.size() - sa[sa_index]);
	for (unsigned i = 0; i < len; i++)
	{
		if (zwsp[i] != query[i])
		{
			break;
		}
		output++;
	}
	return output;
}

/// Build suffix array from text.
/// Input: the text (might be empty)
/// Output: a suffix array (sorted). The variable 'sa' is cleared before it's filled and returned.
void construct(std::vector<uint32_t>& sa, const std::string& text)
{
	sa.clear();
	if (text == "")
	{
		return;
	}
	std::vector<help> a(text.size());
	help b;
	const int len = text.size();
	for (int i = 0; i < len - 1; i++)
	{
		b.index = i;
		b.zahl = static_cast<int>(text[i]);
		b.zahl2 = static_cast<int>(text[i + 1]);

		b.laenge = len - i;
		a[i] = b;
	}
	b.index = len - 1;
	b.zahl = static_cast<int>(text[len - 1]);
	b.zahl2 = static_cast<int>(text[0]);
	b.laenge = 1;
	a[len - 1] = b;

	std::sort(a.begin(), a.end(), help_for_sort);

	for (int i = 0; i < len - 1; i++)
	{
		int zwischenspeicher1 = a[i + 1].zahl;
		int zwischenspeicher2 = a[i + 1].zahl2;
		int zwischenspeicher3 = a[i].zahl;
		int zwischenspeicher4 = a[i].zahl2;
		int j = 1;
		while (a[i].zahl == a[i + 1].zahl && a[i].zahl2 == a[i + 1].zahl2)
		{

			if (1 < a[i].laenge - j)
			{
				a[i].zahl = static_cast<int>(text[a[i].index + j]);
				a[i].zahl2 = static_cast<int>(text[a[i].index + j + 1]);

				a[i + 1].zahl = static_cast<int>(text[a[i + 1].index + j]);
				if (1 < a[i + 1].laenge - j)
				{

					a[i + 1].zahl2 = static_cast<int>(text[a[i + 1].index + j + 1]);

				}
				else
				{

					a[i + 1].zahl2 = 0;
				}
			}
			else
			{
				a[i].zahl2 = 0;
			}
			std::sort(a.begin() + i, a.begin() + i + 2, help_for_sort);
			j++;
		}
		a[i + 1].zahl = zwischenspeicher1;
		a[i + 1].zahl2 = zwischenspeicher2;
		a[i].zahl = zwischenspeicher3;
		a[i].zahl2 = zwischenspeicher4;
	}

	for (unsigned i = 0; i < a.size(); i++)
	{
		sa.push_back(a[i].index);

	}
}

/// Search a string in a text via suffix array, which was constructed before using 'construct()'. If the query is empty, return empty hits.
/// Input: search for a 'query' string in a suffix array 'sa' build from 'text'.
/// Output: Results are returned in 'hits' (sorted ascending by position in text!). The variable is cleared before its filled and returned.
void find(const std::string& query, const std::vector<uint32_t>& sa,
	const std::string& text, std::vector<uint32_t>& hits)
{
	hits.clear();
	std::string::size_type found = text.find(query);
	if (!(found != std::string::npos)) {
		return;
	}

	if (text == query)
	{

		hits.push_back(0);

		return;
	}
	if (text == "" || query == "")
	{
		return;
	}
	
	//const int query_size = query.size();
	//const int text_size = text.size() - 1;
	double lp;
	double rp;
	///Lp-Suche:
	if (query <= create_suftab(sa, text, query, 0))
	{
		lp = 0.0;
	}
	else if (query > create_suftab(sa, text, query, sa.size() - 1))
	{
		return;
	}
	else
	{
		double L = 0.0;
		double R = sa.size() - 1.0;
		double M;
		while (R - L > 1)
		{
			M = ceil(L + R / 2.0);
			int l = anzahl(sa, text, query, L);
			int r = anzahl(sa, text, query, R);
			int mlr = std::min(l, r);
			std::string temp = create_suftab(sa, text, query, M, mlr);
			unsigned min_len = std::min(query.size(), temp.size());
			bool a = false;
			for (unsigned i = 0; i < min_len; i++)
			{
				if (query[mlr + i] == temp[i])
				{
					a = true;
					continue;
				}
				else if (query[mlr + i] < temp[i])
				{
					a = false;
					R = M;
					break;
				}
				else
				{
					a = false;
					L = M;
					break;
				}
			}
			if (a)
			{
				size_t temp_len = text.size() - sa[(int)(M)];
				if (query.size() <= temp_len)
				//if (query.size() <= temp.size()+mlr)
				{
					R = M;
				}
				else
				{
					L = M;
				}
			}
		}
		lp = R;
	}
	/// Rp-Suche:
	if (query >= create_suftab(sa, text, query, (sa.size() - 1)))
	{
		rp = sa.size() - 1.0;
	}
	else if (query < create_suftab(sa, text, query, 0))
	{
		rp = 0;
	}
	else
	{
		double L = 0.0;
		double R = sa.size() - 1.0;
		double M;
		while (R - L > 1)
		{
			M = ceil(L + R / 2.0);
			int l = anzahl(sa, text, query, L);
			int r = anzahl(sa, text, query, R);
			int mlr = std::min(l, r);
			std::string temp = create_suftab(sa, text, query, M, mlr);
			unsigned min_len = std::min(query.size(), temp.size());
			float zwsp = L;
			for (unsigned i = 0; i < min_len; i++)
			{
				if (query[mlr + i] == temp[i])
				{
					L = M;
					continue;
				}
				else if (query[mlr + i] >= temp[i])
				{
					L = M;
					break;
				}
				else
				{
					R = M;
					L = zwsp;
					break;
				}
			}
		}
		rp = L;
	}

	const int len2 = rp - lp;
	for (int i = 0; i <= len2; ++i)
	{
		hits.push_back(sa[lp + i]);

	}

	std::sort(hits.begin(), hits.end());
}

//int main()
//{
//	std::vector<uint32_t> sa;
//	const std::string text = "Hello";
//	const std::string query = "e";
//	//construct(sa, text);
//	std::vector<uint32_t> hits;
//	//find(query, sa, text, hits);
//	std::cout << hits.size() << std::endl;
//	for (auto c : hits)
//	{
//		std::cout << c << " ";
//	}
//	std::cout << foo(query, text) << std::endl;
//}
