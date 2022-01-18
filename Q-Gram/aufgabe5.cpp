#include "QGramIndex.hpp"
#include "a5_util.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

/**
     @brief Constructor: build up a q-gram index from @p text.

     Internally, @p text is stored as const reference (const std::string& ),
     i.e. make sure the text does not go out of scope as long as the class object is used.

     The index is build immediately.

     @param text The sequence (genome, ...) to be indexed
     @param q Length of q-gram (from 1 to 13).
     @throws std::invalid_argument("Invalid q!"); if NOT: 1 <= q <= 13
    */
QGramIndex::QGramIndex(const std::string& text, const uint8_t q)
{
    if ((1 <= q) && (q <= 13))
    {
        this->text2 = text;
        this->q2 = q;
        //Schritt 1: dir mit Nullen auffüllen
        size_t dir_length = pow(4, q);
        dir.resize(dir_length);
        for (size_t i = 0; i < dir_length; i++)
        {
            dir[i] = 0;
        }
        // Schritt 2: q-Gramme zählen
        size_t length = text.size() - q;
        int j = hash(text.substr(0, q));
        dir[j]++;
        suftab.push_back(0);
        for (size_t i = 0; i < length; i++)
        {
            j = hashNext(j, text[q + i]);
            dir[j]++;
            suftab.push_back(0);
        }
        /*       auto is_zero = [](int i) {return i != 0; };
               dir.erase(std::remove_if(dir.begin(), dir.end(), is_zero), dir.end());*/
               // Schritt 3: kumulative Summe bilden
        for (size_t i = 1; i < dir.size(); i++)
        {
            dir[i] += dir[i - 1];
        }

        // Schritt 4: Vorkommen einsortieren
        j = hash(text.substr(0, q));;
        dir[j]--;
        suftab[dir[j]] = 0;
        for (size_t i = 0; i < length; i++)
        {
            j = hashNext(j, text[q + i]);
            dir[j]--;
            suftab[dir[j]] = i + 1;
        }
    }
    else
    {
        throw std::invalid_argument("Invalid q!");
    }

}


/**
  @brief Returns the text.

  @return the full text
*/
const std::string& QGramIndex::getText() const
{
    return this->text2;
}

/**
  @brief Returns a vector of indices into text, where the q-gram with hash @p h occurs.

  The vector might be empty, if the q-gram does not exist, i.e. empty range within the suffix array.
  The hash value must be within [0...|SIGMA|^q), otherwise an exception is thrown.

  @note The indices are in reverse order of occurrence in the text, or might be even "unordered"
  when OpenMP multithreading is used.

  @param h Hash value of the q-gram
  @return Vector of hits (indices into text)
  @throws std::invalid_argument("Invalid hash!"); if h is outside of valid hash values
*/
std::vector<uint32_t> QGramIndex::getHits(const uint32_t h) const
{
    if (h < 0 || h > pow(4, getQ()) - 1)
    {
        throw std::invalid_argument("Invalid hash!");
    }
    std::vector<uint32_t> hits = {};
    if (h == dir.size() - 1) {

        for (uint32_t i = dir[h]; i < suftab.size(); ++i) {
            hits.push_back(suftab[i]);
        }
        return hits;

    }
    else {

        for (uint32_t i = 0; i < (dir[h + 1] - dir[h]); ++i) {
            hits.push_back(suftab[dir[h] + i]);
        }
        return hits;
    }


}

/**
  @brief Get the length of q-grams, i.e. 'q'.
*/
uint8_t QGramIndex::getQ() const
{
    return this->q2;
}

/**
  @brief Compute a full hash for a given q-gram.

  @param qgram A q-gram; must have size 'q'
  @return hash value of @p qgram
  @throws std::invalid_argument("Invalid q-gram. Wrong length!"); if qgram.size() != q

*/
uint32_t QGramIndex::hash(const std::string& qgram) const
{
    if (qgram.size() != this->getQ()-1)
    {
        throw std::invalid_argument("Invalid q-gram. Wrong length!");
    }
    uint32_t hashvalue = 0;

    for (auto c : qgram)
    {
        hashvalue <<= 2;
        hashvalue |= ordValue(c);
    }
    return hashvalue;
}

/**
   @brief Returns the next rolling hash for given new character and previous hash (of previous q-gram).

   The first character going out-of-scope is removed internally by bitmasking (i.e. no need to specify it).

   @param prev_hash Previous hash value
   @param new_pos new character (last position of current q-gram)
   @return The updated hash value
*/
uint32_t QGramIndex::hashNext(const uint32_t prev_hash, const char new_pos) const
{
    
    uint32_t hashvalue = prev_hash * 4;
    unsigned mask = ~0 << 2 * getQ(); 
    hashvalue &= ~mask;
    hashvalue += ordValue(new_pos);
    return hashvalue;
    
}

