#include "aufgabe1.hpp"
#include <iostream>

/**
     * @brief Preprocess (=generate lookup table) and store the pattern
     * @param pat The pattern to search later on.
    */
void Horspool::setPattern(const std::string &pat) {
    this->pattern_ = pat;
    const unsigned m = pat.size();
    std::vector<uint32_t> temp_vec(m);
    for (unsigned i = 0; i < m; i++) {
        temp_vec[i] = m;
        this->dict[pat[i]] = m;
    }
    for (unsigned j = 0; j < (m - 1); j++) {

        temp_vec[j] -= (j + 1);
    }
    for (unsigned d = 0; d < temp_vec.size() - 1; d++) {
        this->dict[pat[d]] = temp_vec[d];
    }
    /*if(this->dict[pat[temp_vec.size()-1]] != m)
    {

    }
     */


    temp_vec.clear();


}

/**
 * @brief Return the currently set pattern (or empty if not set yet)
 * @return The pattern
*/
const std::string &Horspool::getPattern() const {
    return this->pattern_;
}

/**
 * @brief Get all hits of pattern (previously set using setPattern()) in @p text.
 * @param text The haystack to search
 * @return Indices of hits (0-based) of pattern in the text
*/
std::vector<size_t> Horspool::getHits(const std::string &text) const {
    std::vector<size_t> hits{};
    if(text.empty())
    {
        return hits;
    }

    std::vector<size_t> temps;
    int pos = 0;
    int n = text.size();
    int m = this->getPattern().size();


    while (pos <= (n - m)) {
        size_t j = (m - 1);
        alignCheck_(pos);
        temps.push_back(pos);

        while (j > 0 && (text[pos + j] == this->getPattern()[j] || (this->getPattern()[j] == '?') )) {

            j--;
        }
        if (j == 0) {

            hits.push_back(pos);
        }

            pos += this->getShift_(text[pos + (m - 1)]);


    }
    for(auto c: hits)
    {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    for(auto c: temps)
    {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    return hits;

}

/**
     * @brief Use the lookup table created in 'setPattern' to obtain the maximum Horspool shift distance
              given the last character of the current alignment in the text

     * @param last_char The last character of the text infix from the current alignment
     * @return
    */
uint32_t Horspool::getShift_(const char last_char) const {
    if (this->check_in_pattern(last_char)) {
        auto found = this->dict.find(last_char);

        return found->second;
    } else {
        if(this->check_in_pattern('?'))
        {
            auto found = this->dict.find('?');

            return found->second;
        }
        return this->pattern_.size();
    }
}


using namespace std;

int main() {
    string pat = "h?ll?";
    string query = "hello! Is it hellas, hallo, hell yeah!, or hull?";

    Horspool h;
    auto patlen = pat.size();
    h.setPattern(pat);
    h.getHits(query);


}
