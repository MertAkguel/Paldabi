#include "a5_util.hpp"
#include "QGramIndex.hpp"
#include <iostream>
#include <string>
#include <fstream>
/*
int main(int argc, char *argv[]) {

    if (argc != 3) {
       std::cerr << "Wrong parameter" << std::endl;
       return 1;
    } else {
        std::string text{};
        std::string q = argv[2];
        std::ifstream genom;
        genom.open(argv[1]);
        getline(genom, text);

        QGramIndex qgram(text, q.length());

        uint32_t hash = qgram.hash(q);
        std::vector<uint32_t> hits = qgram.getHits(hash);
        std::cout << q << ": ";
        for(uint32_t i = 0; i < hits.size(); ++i) {
            std::cout << hits[i] << " ";
        }
        std::cout << std::endl;
    
    }
}
*/