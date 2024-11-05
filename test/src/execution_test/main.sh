root_dir=~/code/42/cc/minishell
src_dir="${root_dir}/test/src/execution_test"

setup () {
	if make -C "$root_dir"; then
		echo minishell build ok
	else
		echo Error: couldnt build minishell
		exit 1
	fi
}

main () {
	setup
}

main
