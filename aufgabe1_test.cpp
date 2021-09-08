/// This file tests the interface (functions) defined in aufgabe1.hpp.
/// Implement the functions in aufgabe1.cpp (case sensitive name, i.e. do not use another name!) and 
///
/// compile with
///    g++ -std=c++17 -Wall -pedantic -O0 aufgabe1.cpp aufgabe1_test.cpp -o aufgabe1_test
/// Then execute
///    ./aufgabe1_test
/// If you see "Test passed!" at the end, your implementation is correct. If there is an error, fix it and try again.

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include "aufgabe1.hpp"

void test_complement()
{
    std::cout << "Testing 'complement()' ... ";
    assert(complement('C') == 'G');
    assert(complement('G') == 'C');
    assert(complement('A') == 'T');
    assert(complement('T') == 'A');
    std::cout << "success!\n";
}

void test_reverseComplement()
{
    std::cout << "Testing 'reverseComplement()' ... ";
    assert(reverseComplement("") == "");
    assert(reverseComplement("ACGTT") == "AACGT");
    assert(reverseComplement("A") == "T");
    assert(reverseComplement(std::string(300, 'A')) == std::string(300, 'T'));
    std::cout << "success!\n";
}

void test_readAndWriteFASTA(std::string const& seq, std::string const& a1_test_file)
{
    std::cout << "Testing 'readAndWriteFASTA()' ... ";
    /// read a FASTA entry from a file, returning a pair containing meta data and sequence
    /// Returns a pair with empty strings if in_file not readable 

    // test corner cases:
    std::pair<std::string, std::string> entry = readFasta("/this/file/does/not/exist");
    assert(entry.first == "");
    assert(entry.second == "");

    if (writeFasta("/this/file/does/not/exist", ">meta", seq))
    {
        std::cerr << "Function 'writeFasta()' did not return 'false' when writing to a non-existant file\n";
        exit(1); // return with 1 (error)
    }

    // test actual data  
    // create a file first:
    if (!writeFasta(a1_test_file, ">meta", seq))
    {
        std::cerr << "Test could not create FASTA file '" << a1_test_file << "' (or writeFASTA is broken)\n";
        exit(1); // return with 1 (error)
    }
    std::pair<std::string, std::string> p = readFasta(a1_test_file);
    assert(p.first == ">meta");
    assert(p.second == seq);
    // test if lines are 80 chars long
    std::ifstream fstr(a1_test_file.c_str());
    std::string line;
    assert(fstr.good());
    assert(getline(fstr, line)); // read first line (header)
    assert(getline(fstr, line)); // read second line
    assert(line.size() == 80);   // check length
    assert(getline(fstr, line)); // read third line
    assert(line.size() == 80);   // check length

    std::cout << "success!\n";
}

void test_reverseComplementFASTA(std::string const& seq, std::string const& a1_test_file, std::string  const& a1_test_rc_file)
{
    std::cout << "Testing 'reverseComplementFASTA()' ... ";
    //bool reverseComplementFASTA(std::string const input_file, std::string const output_file);
    assert(reverseComplementFASTA(a1_test_file, a1_test_rc_file) == true);
    std::pair<std::string, std::string> p = readFasta(a1_test_rc_file);
    assert(p.first == ">meta");
    assert(p.second == reverseComplement(seq));
    std::cout << "success!\n";
}

int main(int argc, char* argv[])
{
    if (argc != 1)
    {
        std::cerr << "No arguments required. All arguments will be ignored!" << std::endl;
    }

    std::string seq(300, 'A'); // 300 'A's -- to test line breaks
    seq += "ACGTT";

    const std::string a1_test_file = "a1_test.tmp";
    const std::string a1_test_rc_file = "a1_test_rc.tmp";

    test_complement();
    test_reverseComplement();
    test_readAndWriteFASTA(seq, a1_test_file);
    test_reverseComplementFASTA(seq, a1_test_file, a1_test_rc_file);
    std::cout << "Test passed (10/10 points)!\n";
}
