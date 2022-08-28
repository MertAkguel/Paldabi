#include "BLAST_Neighborhood.hpp"


/**
 Berechnet die BLAST Nachbarschaft fuer eine Query.

 @param matrix          Die Scoring-Matrix fuer das Berechnen der Nachbarschaft
 @param word_size       Die zu verwendende Wortgroesse
 @param score_threshold Der minimale Score den ein Wort haben muss um zur Nachbarschaft zugehoeren
 @param threads         Die Anzahl der Threads die verwendet werden sollen
 @return Die Nachbarschaften der Query-Infixe, sortiert nach Position der Infixe in Query (d.h. 1. Infix, 2. Infix, ...).
         Die Nachbarschaft jedes Infixes ist alphabetisch sortiert.
 @throw Wirft eine Exception, wenn @p threads != 1 ist (ohne Zusatzaufgabe), bzw <= 0 (mit Zusatzaufgabe)
*/
std::vector<NHResult> BLAST_Neighborhood::generateNeighborhood(const std::string &query,
                                                               const ScoreMatrix &matrix,
                                                               const int word_size,
                                                               const int score_threshold,
                                                               const int threads) {

    if (threads < 1) {
        throw std::invalid_argument("Thread must be 1 or higher");
    }

    if (word_size > int(query.size())) {
        std::vector<NHResult> empty_output;
        return empty_output;
    }
    const std::string alphabet = "ACDEFGHIKLMNPQRSTVWY";
    std::string infix;
    std::vector<NHResult> outputvector(query.size() - word_size + 1);

    std::pair<std::string, int> foo = {};

    std::vector<std::string> v = {};
    // build all permutations
    this->allPermutation(alphabet, "", alphabet.size(), word_size, v);

    omp_set_num_threads(threads);
#pragma omp parallel
    {
#pragma omp for
        for (unsigned i = 0; i < (query.size() - word_size + 1); i++) {
            // build infix
            infix = query.substr(i, word_size);
            outputvector[i].infix = infix;
            for (auto &x: v) {
                int score = 0;
                // calculate score
                for (int index = 0; index < word_size; index++) {

                    score += (matrix.score(infix[index], x[index]));
                }
                // put neighbor to neighbors if score higher than the threshhold
                if (score >= score_threshold) {
                    foo.first = x;
                    foo.second = score;
                    outputvector[i].neighbors.push_back(foo);
                }
            }
        }
    }


    return outputvector;
}