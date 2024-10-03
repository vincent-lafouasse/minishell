#pragma once

#include <vector>

extern "C"
{
#include "parse/t_command/t_command.h"
#include "word/t_word_list/t_word_list.h"
};

struct Words
{
    t_word_list *self;

    Words(const std::vector<const char *> &words);
    Words(const t_simple *simple);
    ~Words();

    t_word_list *get() const;
    bool operator==(const Words &other) const;
    bool operator!=(const Words &other) const;
};
