#ifndef WORDS_H
#define WORDS_H

extern "C" {
#include "word/t_word_list/t_word_list.h"
}

#include <vector>
#include <string>
#include <iostream>

struct Words
{
    t_word_list *words;

    Words(const std::vector<std::string> &words);
	Words(const Words &other) noexcept;
    Words(t_word_list *wl);
    ~Words();


    friend std::ostream& operator<<(std::ostream& os, const Words& words)
    {
        os << "Words {";
		for (t_word_list *wl = words.as_list(); wl != NULL; wl = wl->next)
        {
            os << "'" << wl->contents << "', ";
        }
        os << "}" << std::endl;
        return (os);
    }

    t_word_list *as_list() const;
    bool operator==(const Words &other) const;
    bool operator!=(const Words &other) const;
};

#endif // WORDS_H
