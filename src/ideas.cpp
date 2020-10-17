#include "ideas.hpp"

Ideas::Ideas()
{
    generateIdeas();
}


std::vector<std::string> Ideas::getIdeas() const
{
    return ideas_;
}

void Ideas::generateIdeas()
{
    for(int i = 0; i < numOfIdeas; ++i)
    {
        std::string idea;

        for(auto & fileName : fileNames)
        {
            std::vector<std::string> words;
            std::ifstream fileHandler(fileName);
            std::string word;
            while (getline (fileHandler, word))
            {
                words.push_back(word);
            }
            auto randomWordIndex = std::rand() % words.size();
            idea.append(words.at(randomWordIndex)).append(" ");
        }
        ideas_.push_back(idea);
    }
}
