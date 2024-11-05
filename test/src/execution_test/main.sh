root_dir=~/code/42/cc/minishell
minishell="${root_dir}/minishell"
src_dir="${root_dir}/test/src/execution_test"

setup () {
	if make -C "$root_dir"; then
		echo minishell build ok
	else
		echo Error: couldnt build minishell
		exit 1
	fi
}


test () {
	input="$1"
}

main () {
	setup
	test "echo hello"
}

main
