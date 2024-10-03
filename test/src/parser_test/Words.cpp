#include "Words.h"

Words::Words(const std::vector<const char*>& words) : self(nullptr)
{
	for (const char* word: words)
		wl_push_back(&self, (char*)word);
}

Words::Words(const t_simple* simple) : self(nullptr)
{
	t_word_list* words = simple->words;

	while (words)
	{
		wl_push_back(&self, words->contents);
		words = words->next;
	}
}

t_word_list* Words::get() const
{
	return this->self;
}

bool Words::operator==(const Words& other) const
{
	t_word_list* rhs = this->get();
	t_word_list* lhs = other.get();

	while (rhs && lhs)
	{
		if (strcmp(rhs->contents, lhs->contents) != 0)
			return false;
		rhs = rhs->next;
		lhs = lhs->next;
	}

	return (!rhs && !lhs);
}

bool Words::operator!=(const Words& other) const
{
	return !(*this == other);
}
