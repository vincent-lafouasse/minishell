# Minishell commands

## Prelimnary

### Valid commands

`in_file` is valid for reading
`out_file` is valid for writing and creation
`cmd1` exists in path and is valid for execution
`cmd2` is a valid and executable command, whether builtin or in path

```sh
< in_file cmd1 | cmd2 > out_file
cmd1 < in_file | > out_file cmd2
```

