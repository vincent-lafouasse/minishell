# notes

A Leftmost, Top-Down Parsing Algorithm
torczon p98
```
root ← node for the start symbol, S;
focus ← root;
push(null);
word ← NextWord();

while (true) do;
    if (focus is a nonterminal) then begin;
        pick next rule to expand focus (A → β1,β2,...,βn);
        build nodes for β1 , β2 . . . βn as children of push(βn, βn−1, ..., β2);
        focus ← β1;
    end;
    else if (word matches focus) then begin;
        word ← NextWord();
        focus ← pop()
    end;
    else if (word = eof and focus = null)
        then accepttheinputandreturnroot;
    else backtrack;
end;
```
