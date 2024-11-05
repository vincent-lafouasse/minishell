root_dir=~/code/42/cc/minishell
src_dir="${root_dir}/test/src/execution_test"

setup () {
	make -C "$root_dir"
}

main () {
	setup
}

main
