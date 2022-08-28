#include "Alignment.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>


/// Constructor with two sequences
/// Makes an internal copy of the sequences.
Alignment::Alignment(const std::string &seq_v, const std::string &seq_h) {
    this->horizontal = " " + seq_h; // for i
    this->vertikal = " " + seq_v;
    this->matrix.clear();
    this->traceback = fill_None();
    this->sm_max_element = 0;
    this->sm_max_element_koordianten = {0, 0};
    this->local = false;
}

/// compute the aligment (i.e. score and traceback)
/// given the three alignment parameters match, mismatch and gap
/// if local_align == true, compute the local Smith-Waterman (SW) alignment (extra points), or throw
/// an exception if your implementation does not support SW.
void Alignment::compute(const int match, const int mismatch, const int gap, const bool local_align) {
    this->local = local_align;
    this->matrix = this->fill_null(); // filling matrix

    // Needle-man-Wunsch
    if (!local_align) {
        matrix[0][0] = 0;
        traceback[0][0] = Traceback::NONE;
        //Initialization:
        for (size_t i = 1; i < matrix.size(); i++) {
            matrix[i][0] += i * gap;
            traceback[i][0] = Traceback::VERTICAL;
        }
        for (size_t i = 1; i < matrix[0].size(); i++) {
            matrix[0][i] += i * gap;
            traceback[0][i] = Traceback::HORIZONTAL;
        }
        //Recurrence:
        for (size_t i = 1; i < matrix.size(); i++) {
            for (size_t j = 1; j < matrix[0].size(); j++) {
                // case there is a match
                if (horizontal[j] == vertikal[i]) {
                    // Diagonal
                    if (matrix[i - 1][j - 1] + match >= matrix[i - 1][j] + gap &&
                        matrix[i - 1][j - 1] + match >= matrix[i][j - 1] + gap) {
                        matrix[i][j] = matrix[i - 1][j - 1] + match;
                        traceback[i][j] = Traceback::DIAGONAL;
                    }
                        // Vertical
                    else if (matrix[i - 1][j] + gap > matrix[i - 1][j - 1] + match &&
                             matrix[i - 1][j] >= matrix[i][j - 1] + gap) {
                        matrix[i][j] = matrix[i - 1][j] + gap;
                        traceback[i][j] = Traceback::VERTICAL;
                    }
                        // Horizontal
                    else if (matrix[i][j - 1] + gap > matrix[i - 1][j - 1] + match &&
                             matrix[i][j - 1] > matrix[i - 1][j] + gap) {
                        matrix[i][j] = matrix[i][j - 1] + gap;
                        traceback[i][j] = Traceback::HORIZONTAL;
                    }
                }
                    // case there is a mismatch
                else {
                    // Diagonal
                    if (matrix[i - 1][j - 1] + mismatch >= matrix[i - 1][j] + gap &&
                        matrix[i - 1][j - 1] + mismatch >= matrix[i][j - 1] + gap) {
                        matrix[i][j] = matrix[i - 1][j - 1] + mismatch;
                        traceback[i][j] = Traceback::DIAGONAL;
                    }
                        // Vertikal
                    else if (matrix[i - 1][j] + gap > matrix[i - 1][j - 1] + mismatch &&
                             matrix[i - 1][j] >= matrix[i][j - 1] + gap) {
                        matrix[i][j] = matrix[i - 1][j] + gap;
                        traceback[i][j] = Traceback::VERTICAL;
                    }
                        // Horizontal
                    else if (matrix[i][j - 1] + gap > matrix[i - 1][j - 1] + mismatch &&
                             matrix[i][j - 1] > matrix[i - 1][j] + gap) {
                        matrix[i][j] = matrix[i][j - 1] + gap;
                        traceback[i][j] = Traceback::HORIZONTAL;
                    }
                }

            }
        }
    }
        // Smith-Waterman
    else {

        matrix[0][0] = 0;
        traceback[0][0] = Traceback::NONE;
        //Initialization:
        for (size_t i = 1; i < matrix.size(); i++) {
            matrix[i][0] = 0;
            traceback[i][0] = Traceback::NONE;
        }
        for (size_t i = 1; i < matrix[0].size(); i++) {
            matrix[0][i] = 0;
            traceback[0][i] = Traceback::NONE;
        }
        // Recurrence:
        for (size_t i = 1; i < matrix.size(); i++) {
            for (size_t j = 1; j < matrix[0].size(); j++) {
                // case there is a match
                if (horizontal[j] == vertikal[i]) {
                    // Diagonal
                    if (matrix[i - 1][j - 1] + match >= matrix[i - 1][j] + gap &&
                        matrix[i - 1][j - 1] + match >= matrix[i][j - 1] + gap && matrix[i - 1][j - 1] + match > 0) {
                        matrix[i][j] = matrix[i - 1][j - 1] + match;
                        traceback[i][j] = Traceback::DIAGONAL;
                    }
                        // Vertikal
                    else if (matrix[i - 1][j] + gap > matrix[i - 1][j - 1] + match &&
                             matrix[i - 1][j] >= matrix[i][j - 1] + gap && matrix[i - 1][j] + gap > 0) {
                        matrix[i][j] = matrix[i - 1][j] + gap;
                        traceback[i][j] = Traceback::VERTICAL;
                    }
                        // Horizontal
                    else if (matrix[i][j - 1] + gap > matrix[i - 1][j - 1] + match &&
                             matrix[i][j - 1] > matrix[i - 1][j] + gap && matrix[i][j - 1] + gap > 0) {
                        matrix[i][j] = matrix[i][j - 1] + gap;
                        traceback[i][j] = Traceback::HORIZONTAL;
                    } else {
                        matrix[i][j] = 0;
                        traceback[i][j] = Traceback::NONE;
                    }
                }
                    // case there is a mismatch
                else {
                    // Diagonal
                    if (matrix[i - 1][j - 1] + mismatch >= matrix[i - 1][j] + gap &&
                        matrix[i - 1][j - 1] + mismatch >= matrix[i][j - 1] + gap &&
                        matrix[i - 1][j - 1] + mismatch > 0) {
                        matrix[i][j] = matrix[i - 1][j - 1] + mismatch;
                        traceback[i][j] = Traceback::DIAGONAL;
                    }
                        // Vertikal
                    else if (matrix[i - 1][j] + gap > matrix[i - 1][j - 1] + mismatch &&
                             matrix[i - 1][j] >= matrix[i][j - 1] + gap && matrix[i - 1][j] > 0) {
                        matrix[i][j] = matrix[i - 1][j] + gap;
                        traceback[i][j] = Traceback::VERTICAL;
                    }
                        // Horizontal
                    else if (matrix[i][j - 1] + gap > matrix[i - 1][j - 1] + mismatch &&
                             matrix[i][j - 1] + gap > matrix[i - 1][j] + gap && matrix[i][j - 1] + gap > 0) {
                        matrix[i][j] = matrix[i][j - 1] + gap;
                        traceback[i][j] = Traceback::HORIZONTAL;
                    }
                        // case maximum is 0
                    else {
                        matrix[i][j] = 0;
                        traceback[i][j] = Traceback::NONE;
                    }
                }

            }
        }
        // searching for the max element for Tracebook
        this->sm_max_element = max_element();
    }
}

/// return the score of the alignment;
/// Throws an exception if compute(...) was not called first
int Alignment::score() const {
    if (!this->matrix.empty()) {
        // Score for Needleman-Wunsch
        if (!this->local) {
            return this->matrix[vertikal.size() - 1][horizontal.size() - 1];
        }
            // Score for Smith-Waterman
        else {
            return this->sm_max_element;
        }
    } else {
        throw std::invalid_argument("function compute() was not called before");
    }
}

/// output alignment into three strings.
/// Gaps are denoted as '-' in sequences.
/// The gap-string uses '|' (match) and ' ' (mismatch/gap).
/// Note: all strings are equal in size
/// e.g.
/// a1:   "IMISSMISSIS-SIPPI-"
/// gaps: " |   ||||||  |||| "
/// a2:   "-M--YMISSISAHIPPIE"
/// , where a1 corresponds to seq1, etc.
/// Throws an exception if compute(...) was not called first
void Alignment::getAlignment(std::string &a1, std::string &gaps, std::string &a2) const {
    if (!this->matrix.empty()) {
        a1.clear();
        gaps.clear();
        a2.clear();

        // Alignment for Needleman-Wunsch
        if (!this->local) {
            const int vertikal_size = vertikal.size() - 1;
            const int horizontal_size = horizontal.size() - 1;
            // Starting by m and n where m is the length of the horizontal sequence and n of the vertical sequence
            Tuple foo = {vertikal_size, horizontal_size};
            while (foo.i != 0 || foo.j != 0) // until (i or j == 0)
            {
                // match or mismatch case
                if (traceback[foo.i][foo.j] == Traceback::DIAGONAL) {
                    a2 += horizontal[foo.j];
                    a1 += vertikal[foo.i];

                    // match case
                    if (horizontal[foo.j] == vertikal[foo.i]) {
                        gaps += "|";
                    }
                        // mismatch case
                    else {
                        gaps += " ";
                    }
                    foo.i--;
                    foo.j--;
                }
                    // no match or mismatch case
                    // gap on upper sequence/ first given sequence
                else if (traceback[foo.i][foo.j] == Traceback::HORIZONTAL) {

                    a2 += horizontal[foo.j];
                    a1 += "-";
                    gaps += " ";
                    foo.j--;

                }
                    // no match or mismatch case
                    // gap on lower sequence/ second given sequence
                else {
                    a2 += "-";
                    a1 += vertikal[foo.i];
                    gaps += " ";

                    foo.i--;
                }

            }
        }
            // Alignment for Smith-Waterman
        else {
            // starting by maximum of the matrix
            Tuple foo = {sm_max_element_koordianten.i, sm_max_element_koordianten.j};
            while (matrix[foo.i][foo.j]) // until values are not null
            {
                // match or mismatch case
                if (traceback[foo.i][foo.j] == Traceback::DIAGONAL) {
                    a2 += horizontal[foo.j];
                    a1 += vertikal[foo.i];

                    // match case
                    if (horizontal[foo.j] == vertikal[foo.i]) {
                        gaps += "|";
                    }
                        // mismatch case
                    else {
                        gaps += " ";
                    }
                    foo.i--;
                    foo.j--;
                }
                    // no match or mismatch case
                    // gap on upper sequence/ first given sequence
                else if (traceback[foo.i][foo.j] == Traceback::HORIZONTAL) {
                    a2 += horizontal[foo.j];
                    a1 += "-";
                    gaps += " ";
                    foo.j--;

                }
                    // no match or mismatch case
                    // gap on lower sequence/ second given sequence
                else {
                    a2 += "-";
                    a1 += vertikal[foo.i];
                    gaps += " ";

                    foo.i--;
                }
            }
        }

        // Alignments must be reversed because we started by the end
        std::reverse(a1.begin(), a1.end());
        std::reverse(a2.begin(), a2.end());
        std::reverse(gaps.begin(), gaps.end());
        std::cout << a1 << std::endl;
        std::cout << gaps << std::endl;
        std::cout << a2 << std::endl;

    } else {
        throw std::invalid_argument("function compute() was not called before");
    }
}

