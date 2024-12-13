#include "expand.h"
#include "word/word.h"

#include <stdlib.h>

char *quote_removed_word(const char *word) // bad, dummy implementation
{
	t_error err;
	char *out;

	err = word_clone(word, &out);
	if (err != NO_ERROR)
		return (NULL);
	return (out);
}
