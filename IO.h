#pragma once

#include <vector>
#include <fstream>

class IO
{
public:


	static auto readFile(const std::string& filename) -> std::vector<char>;

}; // class IO