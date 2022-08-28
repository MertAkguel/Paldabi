#include "a5_util.hpp"
#include "QGramIndex.hpp"
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <cstdlib>
//QGramIndex::QGramIndex(const std::string &text, const uint8_t q) : text(text)
//das hat leider nicht funktioniert deshalb leider text nicht als Referenz übergeben
//Wissen nicht wieso das auf dem Server nicht funktioniert hat.
//Haben auch in der hpp entsprechend const std::string &text geschrieben hat aber nichts geändert
//Sie können das auch an den Commits sehen die ganzen Versuche
QGramIndex::QGramIndex(const std::string &text, const uint8_t q) {

    if((1 <= q) && (q <= 13)) {
        this->text = text;
        this->q = q;

        //construct QGI
        std::vector <u_int32_t> dir(construct_QGI());

        //Schritt 2 Hashing dir
        uint32_t hashvalue = hash(this->text.substr(0, this->q));
        uint32_t hashvalue2 = hashvalue;
        ++dir[hashvalue];
        for (uint32_t i = this->q; i < this->text.length(); ++i) {
            hashvalue = hashNext(hashvalue, this->text[i]);
            ++dir[hashvalue];
        }
        
        //Schritt 3 die kumultative Summe
        uint32_t helpsum = dir[0];
        for (uint32_t i = 1; i < dir.size(); ++i) {
            helpsum = helpsum + dir[i];
            dir[i] = helpsum;
        }
                
        // Schritt 4 suftab erstellen und fertiges dir
        std::vector <u_int32_t> suftab((this->text.length() - this->q) + 1);

        --dir[hashvalue2];
        suftab[dir[hashvalue2]] = 0;
        uint32_t helpcount = 1;
        for (uint32_t i = this->q; i < this->text.length(); ++i) {
            hashvalue2 = hashNext(hashvalue2, this->text[i]);
            dir[hashvalue2] = dir[hashvalue2] - 1;
            suftab[dir[hashvalue2]] = helpcount;
            helpcount++;
        }
        this->dir = dir;
        this->suftab = suftab;
    } else {
        throw std::invalid_argument("Invalid q!");
    }
}

const std::string &QGramIndex::getText() const {
    return this->text;
}

std::vector <uint32_t> QGramIndex::getHits(const uint32_t h) const {
    
    if(h > (this->dir.size() - 1)) {
        throw std::invalid_argument("Invalid hash!");
    } else {
        std::vector <uint32_t> output{};
        
        if(h == dir.size()-1) {
            
            for(uint32_t i = dir[h]; i < suftab.size(); ++i) {
                output.push_back(suftab[i]);
            }
            return output;
            
        } else {
            
            for(uint32_t i = 0; i < (dir[h + 1] - dir[h]); ++i) {
                output.push_back(suftab[dir[h]+i]);
            }
            return output;
        }
    }
}

uint8_t QGramIndex::getQ() const {
 return this->q;
}

uint32_t QGramIndex::hash(const std::string &qgram) const {
    
    if(qgram.size() != this->q) {
        throw std::invalid_argument("Invalid q-gram. Wrong length!");
    } else {
        uint32_t hashvalue = ordValue(qgram[0]);
        for(uint8_t i = 1; i < this->q; ++i) {
            hashvalue <<= 2;
            hashvalue |= ordValue(qgram[i]);
        }

        return hashvalue;
    }
}

uint32_t QGramIndex::hashNext(const uint32_t prev_hash, const char new_pos) const {

    uint32_t hash = prev_hash;
    //Dadurch das die bitmaske kürzer ist um 2 Stellen wird durch das verunden der maske mit dem hash
    //die letzten beiden Stellen auf null gesetzt.
    uint32_t mask = ~0 << (2* (this->q-1));
    hash &= ~mask;
    hash <<= 2;
    hash |= ordValue(new_pos);
    return hash;
}

unsigned long QGramIndex::construct_QGI() const {
    return (1 << (2*this->q));
}
