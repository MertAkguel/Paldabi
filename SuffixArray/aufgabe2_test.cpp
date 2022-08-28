//compile with
//g++ -std=c++17 -Wall -pedantic -fsanitize=address  -O3 aufgabe2_test.cpp aufgabe2.cpp -o aufgabe2_test

#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include "aufgabe2.hpp"

using namespace std;

static vector<uint32_t> sa;
static vector<uint32_t> hits;

bool test_banana()
{
    string text = "banana";
    construct(sa, text);
    unsigned res[] = { 5, 3, 1, 0, 4, 2 };
    if (sa.size() != text.size()) return false;

    for (unsigned i = 0; i < sa.size(); ++i)
    {
        if (res[i] != sa[i]) return false;
    }

    return true;
}


bool test_empty_text()
{
    string text = "";
    string query("hello");
    construct(sa, text);
    find(query, sa, text, hits);
    bool res = (hits.empty());

    return res;
}
bool test_empty_query()
{
    string text = "hello";
    string query("");
    construct(sa, text);
    find(query, sa, text, hits);
    bool res = (hits.empty());

    return res;
}

bool test_same_word()
{
    string text = "Hello";
    string query(text);
    construct(sa, text);
    find(query, sa, text, hits);
    bool res = (hits.size() == 1 && hits[0] == 0);

    return res;
}

bool test_single_char()
{
    string text = "Hello";
    string query = "e";
    construct(sa, text);
    find(query, sa, text, hits);
    bool res = (hits.size() == 1 && hits[0] == 1);
    return res;
}

bool test_overlap()
{
    string text = "xxxxxxxxxxx";
    string query = "xxx";
    construct(sa, text);
    find(query, sa, text, hits);
    bool res = (hits.size() == 1 + text.size() - query.size());
    if (!res) return false;

    for (unsigned i = 0; i < hits.size(); ++i)
    {
        if (hits[i] != i) return false;
    }
    return true;
}

bool test_2ndUsage()
{
    string text = "xxxxxxxxxxx";
    string query = "xxx";
    construct(sa, text);
    if (sa.size() != text.size()) return false;
    /* construct again*/
    construct(sa, text);
    if (sa.size() != text.size()) return false;

    find(query, sa, text, hits);
    /* search again*/
    find("x", sa, text, hits);
    bool res = (hits.size() == text.size());
    if (!res) return false;

    for (unsigned i = 0; i < hits.size(); ++i)
    {
        if (hits[i] != i) return false;
    }
    return true;
}

bool test_no_hit()
{
    { // local scope
        string text = "BANANA";
        string query = "X";   // beyond last entry in SA
        construct(sa, text);
        find(query, sa, text, hits);
        if (!hits.empty()) return false;
    }
    string text = "BANANA";
    string query = "KKK"; // hit in the middle of SA
    construct(sa, text);
    find(query, sa, text, hits);
    return (hits.size() == 0);
}


//test if the implementation does not use the naive implementation using std::vector<string>
bool test_speed()
{
    srand(0);
    std::string big(100000, ' '); // 'large' random text
    for (auto& c : big) c = rand() % 128;
    /*  time the construction*/
    auto begin = std::chrono::steady_clock::now();
    construct(sa, big);
    auto end = std::chrono::steady_clock::now();

    size_t time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    std::cout << "Construction time: " << time << "\n";

    if (time < 100)
    {
        std::cout << "SA Construction seems correctly implemented using indices\n";
        return true;
    }
    else
    {
        std::cout << "SA Construction takes way too long. Did you compile with -O3 and are you using a real CPU :)? Are you sure you're just sorting numbers?\n";
        return false;
    }
}

/*
int main()
{

    int err_count = 0;

    if (!test_banana())
    {
        err_count += 3;
        std::cerr << "test_banana failed!\n";
    }

    if (!test_empty_text())
    {
        ++err_count;
        std::cerr << "test_empty_text failed!\n";
    }

    if (!test_empty_query())
    {
        ++err_count;
        std::cerr << "test_empty_query failed!\n";
    }

    if (!test_same_word())
    {
        ++err_count;
        std::cerr << "test_same_word failed!\n";
    }

    if (!test_single_char())
    {
        ++err_count;
        std::cerr << "test_single_char failed!\n";
    }

    if (!test_overlap())
    {
        ++err_count;
        std::cerr << "test_overlap failed!\n";
    }

    if (!test_2ndUsage())
    {
        ++err_count;
        std::cerr << "test_2ndUsage failed!\n";
    }

    if (!test_no_hit())
    {
        ++err_count;
        std::cerr << "test_no_hit failed!\n";
    }

    if (!test_speed())
    {
        err_count += 5;
        std::cerr << "test_speed failed! (your implementation will be inspected)\n";
    }

    if (err_count) std::cout << "Error count: " << err_count << " => " << std::max(0, 10 - err_count) << " points\n";
    else std::cout << "All tests pass (10 points)\n";
    std::cout << "(if code is correctly pushed into Git && aufgabe2_main.cpp is correct && your implementation uses MLR )\n";

    return err_count;
}
*/