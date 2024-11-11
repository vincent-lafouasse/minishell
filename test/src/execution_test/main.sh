EXEC_TEST_ROOT="$(
    cd -- "$(dirname "$0")" >/dev/null 2>&1
    pwd -P
)"
MINISHELL_ROOT="${EXEC_TEST_ROOT}/../../.."
MINISHELL="${MINISHELL_ROOT}/minishell"

BUILD="${MINISHELL_ROOT}/build/exec_test"

setup() {
    make -C "$MINISHELL_ROOT"
    mkdir -p "$BUILD"
}

setup_test() {
    test_name="$1"
    build="${BUILD}/${test_name}"
    bash_output="${build}/bash"
    minishell_output="${build}/minishell"
    mkdir -p "${bash_output}"
    mkdir -p "${minishell_output}"
}

test_command() {
    command="$1"
    test_name="$2"
    setup_test "$test_name"

    infile_dir="${EXEC_TEST_ROOT}/${test_name}"
    command="${command//INFILE_DIR/${infile_dir}}"
    bash_output="${BUILD}/${test_name}/bash"
    minishell_output="${BUILD}/${test_name}/minishell"

    $MINISHELL -c "$command" >"${minishell_output}/stdout" 2>"${minishell_output}/stderr"
    minishell_status=$?

    $MINISHELL -c "$command" >"${bash_output}/stdout" 2>"${bash_output}/stderr"
    bash_status=$?

    echo bash status "$bash_status"
    echo mini status "$minishell_status"
}

main() {
    test_command "echo hello world" "HelloWorld"
    test_command "cat INFILE_DIR/Makefile" "CanTakeInfile"
}

main
