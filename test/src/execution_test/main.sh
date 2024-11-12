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
    local test_name="$1"

    local build="${BUILD}/${test_name}"
    local bash_output="${build}/bash"
    local minishell_output="${build}/minishell"
    mkdir -p "${bash_output}"
    mkdir -p "${minishell_output}"
}

# wip
compare_outputs() {
    local test_name=$1

    local bash_output="${BUILD}/${test_name}/bash"
    local minishell_output="${BUILD}/${test_name}/minishell"

    for file in "$bash_output"/*; do
        DIFF="$(diff "$file")"
    done
}

compare_with_bash() {
    local test_name="$1"
    local command="$2"

    setup_test "$test_name"

    local bash_output="${BUILD}/${test_name}/bash"
    local minishell_output="${BUILD}/${test_name}/minishell"

    local command="${command//INFILE_DIR/${INFILE_DIR}}"
    local minishell_command="${command//OUTFILE_DIR/${minishell_output}}"
    local bash_command="${command//OUTFILE_DIR/${bash_output}}"

    $MINISHELL -c "$minishell_command" >"${minishell_output}/stdout" 2>"${minishell_output}/stderr"
    echo $? > "${minishell_output}/status"

    bash -c "$bash_command" >"${bash_output}/stdout" 2>"${bash_output}/stderr"
    echo $? > "${bash_output}/status"

    if ! diff --brief "$minishell_output" "$bash_output"; then
        HAD_ERROR=1
    fi
}

main() {
    HAD_ERROR=0

    compare_with_bash 'HelloWorld'       'echo hello world'
    compare_with_bash 'CanTakeInfile'    'cat INFILE_DIR/Makefile'
    compare_with_bash 'SeparateOutfiles' 'echo 420 > OUTFILE_DIR/out'
    compare_with_bash 'EnvStuff'         'export COOL_NUMBER=420; echo $COOL_NUMBER'

    exit "$HAD_ERROR"
}

main
