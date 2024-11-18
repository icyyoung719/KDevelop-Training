#ifndef __KX_PERSON_H__
#define __KX_PERSON_H__

#include<vector>
#include<string>
#include<fstream>

// write vector results(contains the positions of the pattern in the text) to file
void write_results(const std::vector<int>& results, const std::string& out_path);

// Read a whole file into a string
std::string read_whole_file(std::string file_path);
#endif