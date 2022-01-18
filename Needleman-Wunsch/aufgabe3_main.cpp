#include <iostream>
#include <string>
#include <vector>
#include "Alignment.hpp"


int main(int argc, char* argv[])
{
    std::string seqv = argv[1];
    std::string seqh = argv[2];
    Alignment a(seqv, seqh);
    std::string s, t, u;
    if (argc == 3)
    {
        a.compute(3, -1 - 2, false);
        
        a.getAlignment(s,t,u);
        std::cout << "score:" << a.score() << std::endl;
    }

    else if (argc == 6)
    {

        int match = std::stoi(argv[3]);
        int mismatch = std::stoi(argv[4]);
        int gap = std::stoi(argv[5]);

        a.compute(match, mismatch, gap);
        a.getAlignment(s, t, u);
        std::cout << "score:" << a.score() << std::endl;
        
    }
    else if (argc == 7)
    {
        int match = std::stoi(argv[3]);
        int mismatch = std::stoi(argv[4]);
        int gap = std::stoi(argv[5]);

        a.compute(match, mismatch, gap, true);
        a.getAlignment(s, t, u);
        std::cout << "score:" << a.score() << std::endl;
    }
    else
    {
        std::cerr << "unexpected input" << std::endl;
        return 1;
    }



}
