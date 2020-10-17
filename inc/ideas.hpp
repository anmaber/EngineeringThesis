#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace
{
    constexpr int numOfIdeas = 10;
}

class Ideas
{
private:
    std::vector<std::string> ideas_;  
    std::vector<std::string> fileNames {"../verbs.txt", "../nouns.txt", "../adjectives.txt"};
    void generateIdeas();
    
public:
    std::vector<std::string> getIdeas() const;
    Ideas();
};
