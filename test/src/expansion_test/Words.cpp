#include "Words.h"

extern "C" {
#include "word/t_word_list/t_word_list.h"
}

#include <vector>
#include <string>
#include <cstring>

Words::Words(const std::vector<std::string> &words) {
    this->words = NULL;
    for (const auto &word : words)
    {
        wl_push_back(&this->words, const_cast<char *>(strdup(word.c_str())));
    }
}

Words::Words(const Words &other) noexcept {
    this->words = NULL;
	t_word_list *other_wl = other.as_list();
	while (other_wl)
	{
        wl_push_back(&this->words, const_cast<char *>(strdup(other_wl->contents)));
		other_wl = other_wl->next;
	}
}

Words::Words(t_word_list *wl) : words(wl) {}

t_word_list *Words::as_list() const {
    return this->words;
}

bool Words::operator==(const Words &other) const {
    t_word_list *a = this->as_list();
    t_word_list *b = other.as_list();
    while (a && b)
    {
        if (strcmp(a->contents, b->contents) != 0)
            return (false);
        a = a->next;
        b = b->next;
    }
    return (!a && !b);
}

bool Words::operator!=(const Words &other) const { return !(*this == other); }

Words::~Words() {
    wl_clear(&this->words, free);
}
