EXEC_TEST_ROOT="$(
    cd -- "$(dirname "$0")" >/dev/null 2>&1 || exit
    pwd -P
)"
MINISHELL_ROOT="${EXEC_TEST_ROOT}/../../.."
MINISHELL="${MINISHELL_ROOT}/minishell"

BUILD="${MINISHELL_ROOT}/build/exec_test"

INFILE_DIR="${EXEC_TEST_ROOT}/infiles"

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

compare_outputs() {
    test_name=$1
    bash_output="${BUILD}/${test_name}/bash"
    minishell_output="${BUILD}/${test_name}/minishell"

    for file in "$bash_output"/*; do
        DIFF="$(diff "$file")"
    done
}

test_command() {
    test_name="$1"
    command="$2"
    setup_test "$test_name"

    bash_output="${BUILD}/${test_name}/bash"
    minishell_output="${BUILD}/${test_name}/minishell"

    command="${command//INFILE_DIR/${INFILE_DIR}}"
    minishell_command="${command//OUTFILE_DIR/${minishell_output}}"
    bash_command="${command//OUTFILE_DIR/${bash_output}}"

    $MINISHELL -c "$minishell_command" >"${minishell_output}/stdout" 2>"${minishell_output}/stderr"
    minishell_status=$?

    bash -c "$bash_command" >"${bash_output}/stdout" 2>"${bash_output}/stderr"
    bash_status=$?

    echo bash status "$bash_status"
    echo mini status "$minishell_status"

    diff --brief "$minishell_output" "$bash_output"
}

main() {
    test_command 'HelloWorld' 'echo hello world'
    test_command 'CanTakeInfile' 'cat INFILE_DIR/Makefile'
    test_command 'SeparateOutfiles' 'echo 420 > OUTFILE_DIR/out'
    test_command 'EnvStuff' 'export COOL_NUMBER=420; echo $COOL_NUMBER'
}

main
