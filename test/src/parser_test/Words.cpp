#include "Words.h"

extern "C" {
#include "word/t_word_list/t_word_list.h"
}

#include <string>
#include <cstring>

Words::Words(const std::vector<std::string> &words) : words(words) {}

Words::Words(const t_simple *simple) : words(std::vector<std::string>{})
{
    t_word_list *word_list = simple->words;

    while (word_list)
    {
        this->words.push_back(std::string(word_list->contents));
        word_list = word_list->next;
    }
}

t_word_list *Words::to_list() const {
    t_word_list *out = nullptr;

    for (const auto &word : words)
    {
        wl_push_back(&out, const_cast<char *>(strdup(word.c_str())));
    }

    return (out);
}

bool Words::operator==(const Words &other) const
{
    return (this->words == other.words);
}

bool Words::operator!=(const Words &other) const { return !(*this == other); }

Words::~Words() {}
