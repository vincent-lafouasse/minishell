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
    else if (word = eof and focus = null) then
        accepttheinputandreturnroot;
    else backtrack;
end;
```

```cpp
Node* root = node for the start symbol, S;
Node* focus = root;
Stack<Node*> to_process {};
Token current_token = next_token();

while (true)
{
    if (focus is a nonterminal)
    {
        pick next rule to expand focus (A → β1,β2,...,βn);
        build nodes for β1 , β2 . . . βn as children of A
        to_process.push(βn, βn−1, ..., β1);
        focus = to_process.pop(); // β1
    }
    else if (current_token matches focus)
    {
        current_token = next_token();
        focus = to_process.pop();
    }
    else if (current_token == EOF && to_process.is_empty())
        return Ok(Root);
    else
        backtrack;
}
```

```cpp
typedef Token Terminal;

struct NonTerminal
{
    NonTerminalKind kind;
    List<Node> children;
};

union Node
{
    Terminal t;
    NonTerminal nt;
};
```
