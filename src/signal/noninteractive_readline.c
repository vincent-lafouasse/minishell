#include "signal.h"
#include <stdio.h>
#include <readline/readline.h>

static int unset_event_hook(void)
{
	rl_event_hook = NULL;
	return (0);
}

// I genuinely, for the life of me, cannot figure out why this works.
// Trying to deep dive into why it does would lead me into reading a
// bunch of readline code, which I don't want to do.
//
// This somehow makes it so that the readline loop simply exists after
// reading all the bytes that were available on stdin, same as how bash does
// it.
//
// For context, we are noodling with the `rl_event_hook` in the first place to
// ensure readline reads changes to the `rl_done` variable, which we set in the
// event of a SIGINT to indicate that readline should return ASAP and stop reading.
// As it turns out, readline won't read this variable (and subsequently not
// return control when it is told to) while it is trying to read characters, and
// the only way to make it respond is in a certain code path that is only
// reached when `rl_event_hook` is set.
//
// The aforementioned code path is sort of quirky for some reason, and for some
// reason DOES NOT respond to an EOF when the standard input is not a terminal,
// making it hang indefinitely! And so the only way to make it respond it by
// unsetting the rl_event_hook, making it exit the loop. I am yet to find any
// documentation explaining this behaviour, let alone documenting it.
//
// Think of it as: when readline HAS to call the `event_hook`, i.e. when it has
// run of characters to read, and is told to wait for input, then we make it
// exit the reading loop. This seems to be preferred for non-terminal stdins.
void make_rl_return_when_input_exhausted(void)
{
	rl_event_hook = unset_event_hook;
}
