#pragma once // include guard

#include <string>
#include <vector>

/// Build suffix array from text.
/// Input: the text (might be empty)
/// Output: a suffix array (sorted). The variable 'sa' is cleared before it's filled and returned.
void construct(std::vector<uint32_t>& sa, const std::string& text);

/// Search a string in a text via suffix array, which was constructed before using 'construct()'. If the query is empty, return empty hits.
/// Input: search for a 'query' string in a suffix array 'sa' build from 'text'.
/// Output: Results are returned in 'hits' (sorted ascending by position in text!). The variable is cleared before its filled and returned.
void find(const std::string& query, const std::vector<uint32_t>& sa, 
		  const std::string& text, std::vector<uint32_t>& hits);
