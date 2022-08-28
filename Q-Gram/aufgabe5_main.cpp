#include <iostream>
#include <string>
#include <vector>
#include "QGramIndex.hpp"
#include <fstream>

int main(int argc, char *argv[]) {

    if (argc == 3) {
        std::string read_line;
        const std::string genome_file = argv[1];
        const std::string query = argv[2];
        const unsigned q = query.size();
        std::fstream f;
        std::string genome_file_text;
        f.open(genome_file, std::ios::in);
        if (!f.good()) {
            return 1;
        }
        while (!f.eof()) {
            std::getline(f, read_line);
            genome_file_text += read_line;
        }
        f.close();
        QGramIndex output(genome_file_text, q);
        std::vector<uint32_t> hit_list = output.getHits(output.hash(query));
        std::cout << query << ": ";
        for (unsigned int i: hit_list) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    } else {
        std::cerr << "unexpected input" << std::endl;
        return 1;
    }


}
