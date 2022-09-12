#include "ACTrie.hpp"
#include <stdexcept>
#include <exception>
#include <queue>
#include <deque>

/// Constructor with a vector of needles (i.e. multiple patterns to be found in a hackstack)
/// Needles may only contain letters from 'A' - 'Z'. All other characters lead to a std::exception being thrown.
/// At least one needle is required. Passing an empty vector leads to throwing of std::logic_error.
ACTrie::ACTrie(const std::vector<std::string> &needles) {
    this->start = 0;
    this->pos_text = 0;
    // exception cases
    if (needles.empty()) {
        throw std::logic_error("At least one needle is required.");

    }

    for (const auto &needle: needles) {
        for (char y: needle) {
            if (int(y) < 65 || int(y) > 90) {

                throw std::exception();
            }
        }
    }

    // build trie
    std::vector<ACNode> trie{};
    ACNode root = ACNode();
    trie.push_back(root);
    for (unsigned i = 0; i < needles.size(); i++) {
        unsigned x = 0;
        unsigned j = 0;
        unsigned current = 0;
        // iterates through Trie as long as the pattern is present
        while (j < needles[i].size() && delta(needles[i][j], trie[current].children, x)) {
            current = x;
            j++;

        }
        // create new nodes if the pattern does not exist
        while (j < needles[i].size()) {
            ACNode nextNode = ACNode();
            nextNode.parent_link = current;
            nextNode.depth = j;
            nextNode.letter = needles[i][j];
            trie.push_back(nextNode);
            trie[current].children.insert({needles[i][j], trie.size() - 1});
            current = trie.size() - 1;
            j++;

        }
        // index of pattern is saved in vector needle_indices if
        // pattern is completely existing
        // else we mark the node as terminal
        if (trie[current].terminal) {
            trie[current].needle_indices.push_back(i);
        } else {
            trie[current].terminal = true;
            trie[current].needle_indices.push_back(i);
        }

    }
    //a dummynode is used to identify the root
    ACNode dummynode = ACNode();
    trie.push_back(dummynode);
    trie[0].suffix_link = trie.size() - 1;

    std::queue<unsigned> foo{};
    unsigned current = 0;
    // childrennode are pushed into the queue
    for (auto node: trie[current].children) {
        foo.push(node.second);
    }
    // Breitensuche on trie
    while (!(foo.empty())) {
        current = foo.front();
        foo.pop();

        for (auto node: trie[current].children) {
            foo.push(node.second);
        }

        unsigned parent = trie[current].parent_link;
        unsigned down = trie[parent].suffix_link;

        // loop ends when suffixlink of the parent node also known as down
        // or a children node exist in down
        while (down != (trie.size() - 1) && !delta(trie[current].letter, trie[down].children, down)) {
            down = trie[down].suffix_link;
        }


        if (down != (trie.size() - 1)) {
            // setting suffixlink of current to down
            trie[current].suffix_link = down;
            if (trie[down].terminal) {
                trie[current].output_link = trie[current].suffix_link;
            } else {
                while (trie[down].suffix_link != 0 && !(trie[trie[down].suffix_link].terminal)) {
                    down = trie[down].suffix_link;
                }
                trie[current].output_link = trie[down].suffix_link;
            }
        } else {
            trie[current].suffix_link = 0;

        }

    }


    this->trie = trie;


}

/// Set a haystack (query) where the needles (patterns) are to be searched
/// This also resets the current trie-node to ROOT
void ACTrie::setQuery(const std::string &haystack) {

    this->query = haystack;
    this->start = 0;
    this->pos_text = 0;
}

/// Resume at the last position in the haystack and node in the trie:
///   consume a character at a time until one or more patterns are found (mind to check output links at every step as well).
/// Fill @p hits with all patterns found at the current node (incl. output links) and return true.
/// If the end of the haystack is reached without a match, return false.
/// When this function is called multiple times, it resumes from the last position (state of the trie).
bool ACTrie::next(std::vector<Hit> &hits) {
    hits.clear();

    // variable that allows to iterate the query only if true
    bool go_on = true;

    // as long as we don't find a pattern, the loop will go on
    while ((this->pos_text < this->query.size()) && hits.empty()) {

        // case if we find a letter in the children vector
        if (delta(this->query[this->pos_text], this->trie[this->start].children, this->start)) {

            // case if we find a letter in the children map
            if (this->trie[this->start].terminal) {
                for (unsigned j = 0; j < this->trie[this->start].needle_indices.size(); j++) {
                    // put match into the vector Hit
                    Hit match = {this->trie[this->start].needle_indices[j],
                                 this->pos_text - this->trie[this->start].depth};
                    hits.push_back(match);
                }
            }

            // following outputlinks as long as the outputlinks shows to the root
            unsigned temp = this->start;
            while (this->trie[temp].output_link != 0) {
                temp = this->trie[temp].output_link;
                for (unsigned x = 0; x < this->trie[temp].needle_indices.size(); x++) {
                    // put match into the vector Hit
                    Hit match = {this->trie[temp].needle_indices[x],
                                 this->pos_text - this->trie[temp].depth};
                    hits.push_back(match);
                }
            }
        } else if (this->trie[this->start].suffix_link != (this->trie.size() - 1)) {
            // following the suffixlinks
            while (this->trie[this->start].suffix_link != (this->trie.size() - 1)) {
                this->start = this->trie[this->start].suffix_link;
                // variable to save start value
                unsigned temp2 = this->start;
                if (delta(this->query[this->pos_text], this->trie[this->start].children, this->start)) {
                    this->start = temp2;
                    go_on = false;
                    break;
                }

            }
        }
        // without that, pos_text will go on, even if he has only landed at the root
        if (go_on) {

            this->pos_text++;
        }
        go_on = true;
    }

    if ((hits.empty())) {

        return false;
    }

    return true;
}


/// Get Tree as string representation (for scoring the construction) using bracket notation.
/// Starting at root (with special label '0'), enumerate all children in alphabetical order (!) in a depth-first search (DFS) and
/// enclose each subtree in '()'.
/// If you are implementing a 'fully resolved' AC trie, only print the edges which increase in depth (i.e. were not created by following suffix links).
/// E.g. given the AC tree of 'ga' and 'c'
///       0
///     /   \
///    g     c
///   /
///  a
/// Output: "0(c)(g(a))"
std::string ACTrie::getTree() const {

    std::string tree{};
    tree += "0";
    unsigned current = 0;
    std::deque<unsigned> stack{};
    unsigned openBracketCount = 0;

    // put children of the root to the stack
    for (auto it = this->trie[current].children.rbegin(); it != this->trie[current].children.rend(); ++it) {
        stack.push_back(it->second);
    }
    // use Tiefensuche
    while (!(stack.empty())) {

        current = stack.back();
        stack.pop_back();
        tree += '(';
        openBracketCount++;
        tree += trie[current].letter;

        for (auto it = this->trie[current].children.rbegin(); it != this->trie[current].children.rend(); ++it) {
            stack.push_back(it->second);
        }
        // if children map is empty close bracket
        if (this->trie[current].children.empty()) {
            tree += ')';
            --openBracketCount;
        }
        // if stack is empty close bracket
        if (!stack.empty()) {
            if (this->trie[current].depth > this->trie[stack.back()].depth) {
                for (unsigned i = 0; i < (this->trie[current].depth - this->trie[stack.back()].depth); ++i) {
                    tree += ')';
                    --openBracketCount;
                }
            }
        } else {
            // close all existing brackets
            for (unsigned i = 0; i < openBracketCount; ++i) {
                tree += ')';
            }
        }


    }
    return tree;
}




