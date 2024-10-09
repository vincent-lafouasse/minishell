#pragma once

#include <vector>
#include <string>
#include <iostream>

extern "C"
{
#include "parse/t_command/t_command.h"
#include "word/t_word_list/t_word_list.h"
};

struct Words
{
    std::vector<std::string> words;

    Words(const std::vector<std::string> &words);
    Words(const t_simple *simple);
    ~Words();


    friend std::ostream& operator<<(std::ostream& os, const Words& words)
    {
        os << "Words {";
        for (const auto &word: words.words)
        {
            os << "'" << word << "', ";
        }
        os << "}" << std::endl;
        return (os);
    }

    t_word_list *to_list() const;
    bool operator==(const Words &other) const;
    bool operator!=(const Words &other) const;
};
