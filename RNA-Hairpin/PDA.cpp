#include "PDA.hpp"


/// Constructor, which specifies which language to check (HAIRPIN by default)
/// and internally builds up the production rules
PDA::PDA(const PDA::Language l) {
    if (l != Language::HAIRPIN) {
        return;
    }
    // production rules
    this->productions['0'] = {"a1u", "c1g", "g1c", "u1a"};
    this->productions['1'] = {"a2u", "c2g", "g2c", "u2a"};
    this->productions['2'] = {"a3u", "c3g", "g3c", "u3a"};
    this->productions['3'] = {"g4a"};
    this->productions['4'] = {"aa", "ca"};
    this->stark.push('&');
    this->stark.push('0');

}

/// Read the next symbol and internally traverse states
/// If a fail state was reached in the previous call, the fail state will be maintained
/// @param a The symbol to read
/// @return current state of PDA
PDA::State PDA::next(const char a) {
    if (a == '$' && this->stark.size() == 1) {
        return State::SUCCESS;
    }
    if (a != 'a' && a != 'c' && a != 'g' && a != 'u') {
        return State::FAIL;
    }
    if (this->state == State::FAIL) {
        return this->state;
    }

    char letter = this->stark.top();
    this->stark.pop();
    // until letter is a number and not a,c,g,u, the loop will not stop
    while (letter != 'a' && letter != 'c' && letter != 'g' && letter != 'u') {

        if (letter == '&') {
            this->state = State::SUCCESS;

        } else if (letter == '0') {
		// searching for the first match in the production rule
            for (unsigned i = 0; i < this->productions[letter].size(); i++) {
                if (productions[letter][i][0] == a) {
			// produce the production rule
                    for (int j = this->productions[letter][i].size()-1; j >= 0; j--) {
                        this->stark.push(productions[letter][i][j]);
                    }
                }
            }
        } else if (letter == '1') {
            for (unsigned i = 0; i < this->productions[letter].size(); i++) {
                if (productions[letter][i][0] == a) {
                    for (int j = this->productions[letter][i].size()-1; j >= 0; j--) {
                        this->stark.push(productions[letter][i][j]);
                    }
                }
            }
        } else if (letter == '2') {
            for (unsigned i = 0; i < this->productions[letter].size(); i++) {
                if (productions[letter][i][0] == a) {
                    for (int j = this->productions[letter][i].size()-1; j >= 0; j--) {
                        this->stark.push(productions[letter][i][j]);
                    }
                }
            }
        } else if (letter == '3') {
            for (unsigned i = 0; i < this->productions[letter].size(); i++) {
                if (productions[letter][i][0] == a) {
                    for (int j = this->productions[letter][i].size()-1; j >= 0; j--) {
                        this->stark.push(productions[letter][i][j]);
                    }
                }
            }
        } else if (letter == '4') {
            for (unsigned i = 0; i < this->productions[letter].size(); i++) {
                if (productions[letter][i][0] == a) {
                    for (int j = this->productions[letter][i].size()-1; j >= 0; j--) {
                        this->stark.push(productions[letter][i][j]);
                    }
                }
            }
        }
        letter = this->stark.top();
        this->stark.pop();

    }
    return state;





    //return State::IN_PROGRESS;
}

/// Reset the automaton to a state as if newly constructed
/// I.e. init stack and set state to s0 (IN_PROGRESS).
void PDA::reset() {
    std::stack<char>().swap(this->stark);
    this->stark.push('&');
    this->stark.push('0');
    this->state = State::IN_PROGRESS;
}

/*
using namespace std;

int main()
{
    stack<char> s;
    s.push('+');
    s.push('0');
    char letter = s.top();

    cout << letter << endl;
}
*/


