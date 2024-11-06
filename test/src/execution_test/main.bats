root_dir=~/code/42/cc/minishell
minishell="${root_dir}/minishell"
src_dir="${root_dir}/test/src/execution_test"

@test "can build minishell" {
    make -C "$root_dir"
}
