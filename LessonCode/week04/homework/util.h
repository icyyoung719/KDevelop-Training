#ifndef __KX_PERSON_H__
#define __KX_PERSON_H__

#include<vector>
#include<string>
#include<fstream>

// set up config
bool set_config(std::string& file_path, std::string& keywords_path, std::string& output_file_path, bool& search_algorithm);

// write vector results(contains the positions of the pattern in the text) to file
void write_results(const std::vector<int>& results, const std::string& out_path);

// read file content to string
void loadKeywords(const std::string& path, std::vector<std::string>& keywords);
#endif