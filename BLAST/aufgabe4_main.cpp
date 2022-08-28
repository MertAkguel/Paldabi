#include "iostream"
#include "vector"
#include "string"
#include "a4_util.h"
#include <numeric>
#include "BLAST_Neighborhood.hpp"


int main(int argc, char *argv[]) {

    if (argc != 6) {
        throw ("It must be 5 arguments");
    }
    double start;
    double end;
    ScoreMatrix m;
    m.load(argv[2]);
    start = omp_get_wtime();
    BLAST_Neighborhood nh;
    std::vector<NHResult> r = nh.generateNeighborhood(argv[1], m, std::stoi(argv[3]), std::stoi(argv[4]),
                                                      std::stoi(argv[5]));


    for (const auto &c: r) {

        std::cout << c.infix << ": ";

        for (const auto &d: c.neighbors) {

            std::cout << "(" << d.first << ", " << d.second << ")" << " ";

        }
        std::cout << std::endl;
    }
    end = omp_get_wtime();
    std::cout << "time: " << (end - start) << std::endl;


}
