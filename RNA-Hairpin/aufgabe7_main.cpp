#include "PDA.hpp"
#include <stdexcept>
int main(int argc, char *argv[]) {

    if(argc != 2)
    {
        throw std::invalid_argument("only two arguments must given");
    }
    std::string inputstring = argv[1];
    inputstring +="$";
    PDA::State s = PDA::State::IN_PROGRESS;
    PDA p(PDA::Language::HAIRPIN);
    for (auto e : inputstring)
    {
        if ((s = p.next(e)) == PDA::State::FAIL) break;
    }
    if (s == PDA::State::SUCCESS)
    {
        std::cout << "ACCEPT" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "FAIL" << std::endl;
        return 1;
    }


}
