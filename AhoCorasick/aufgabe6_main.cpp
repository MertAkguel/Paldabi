#include "ACTrie.hpp"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << ("too few arguments") << std::endl;
    }

    std::string query = argv[1];
    std::vector<std::string> needles{};
    for (int i = 2; i < argc; i++) {
        needles.emplace_back(argv[i]);

    }
    ACTrie trie(needles);

    trie.setQuery(query);
    std::vector<Hit> matches{};
    std::vector<std::vector<Hit>> got;
    std::cout << "Hits (position in query, pattern):" << std::endl;
    while (trie.next(matches))
    {
        got.push_back(matches);
    }

    for(auto & x : got)
    {
        for(auto & y : x)
        {
            std::cout << "pos " << y.pos << ", " << needles[y.index] << std::endl;
        }
    }
    std::cout << trie.getTree() << std::endl;
    return 0;
}
