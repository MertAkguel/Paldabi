#include "aufgabe1.hpp"
#include <iostream>
#include <fstream>

char complement(char const x) {

	char y;
	if (x == 'A')
	{
		y = 'T';
	}
	else if (x == 'T')
	{
		y = 'A';
	}
	else if (x == 'C')
	{
		y = 'G';
	}
	else if (x == 'G')
	{
		y = 'C';
	}
	else { std::cout << "input is not from alphabet {ACGT}" << std::endl; }

	return y;
}

/// reverse a DNA string and complement all bases
std::string reverseComplement(std::string const& input)
{
	std::string ausgabe;
	for (unsigned i = 0; i < input.size(); i++)
	{
		ausgabe += complement(input[input.size() - i - 1]);
	}
	return ausgabe;
}

/// read a FASTA entry from a file, returning a pair containing meta data and sequence (without linebreaks or whitespace)
/// Returns a pair with empty strings if @p in_file is not readable 
std::pair<std::string, std::string> readFasta(std::string const& in_file)
{
	std::pair <std::string, std::string > fastaread;
	std::string zeilenleser;
	std::ifstream f;
	f.open(in_file);
	if (!f.good())
	{

		fastaread.first = "";
		fastaread.second = "";
		return fastaread;

	}
	/*if (fastaread.first == "" || fastaread.second == "")
	{
		std::cerr << in_file << " is not readable" << std::endl;
	}*/
	int counter = 0;
	while (!f.eof())
	{
		if (counter == 0)
		{
			std::getline(f, zeilenleser);
			fastaread.first += zeilenleser;
			//fastaread.first += "\n";
			counter++;
		}
		else {
			std::getline(f, zeilenleser);
			fastaread.second += zeilenleser;
		}
	}
	f.close();


	return fastaread;

}

/// write a FASTA entry to file
/// Upon writing, sequence is split such that each line is 80 characters long (the last line can be shorter of course)
/// Return true on success, false otherwise (e.g. @p out_file not writable)
bool writeFasta(std::string const& out_file,
	std::string const& meta,
	std::string const& seq)
{

	// did not return false when function is not writeable
	std::ofstream f(out_file);
	if (!f.good())
	{
		//std::cerr << out_file << " not writable!";
		return false;
	}
	f << meta << std::endl;
	std::string zwischenspeicher = "";

	for (unsigned i = 0; i < seq.size(); i++)
	{
		zwischenspeicher += seq[i];

		if (zwischenspeicher.size() == 80)
		{
			zwischenspeicher += "\n";
			f << zwischenspeicher;
			zwischenspeicher = "";

		}

	}
	f << zwischenspeicher;
	f.close();
	return true;
}

/// read a FASTA file with a single sequence, and store the sequence's reverse complement in @p output_file
/// Uses @p readFasta() and @p writeFasta() internally.
/// Returns true on success, false otherwise (in_file not readable, or out_file not writable)
bool reverseComplementFASTA(std::string const& input_file,
	std::string const& output_file)
{

	std::ofstream f(output_file);
	std::ifstream g(input_file);
	if (!f.good() || !g.good())
	{

		return false;
	}


	std::pair <std::string, std::string> zwischenspeicher = readFasta(input_file);

	return writeFasta(output_file, zwischenspeicher.first, reverseComplement(zwischenspeicher.second));



}
