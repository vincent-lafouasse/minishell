EXEC_TEST_ROOT="$(
    cd -- "$(dirname "$0")" >/dev/null 2>&1 || exit
    pwd -P
)"
MINISHELL_ROOT="${EXEC_TEST_ROOT}/../../.."
MINISHELL="${MINISHELL_ROOT}/minishell"

BUILD="${MINISHELL_ROOT}/build/exec_test"
INFILE_DIR="${EXEC_TEST_ROOT}/infiles"

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

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
    echo $? >"${minishell_output}/status"

    bash -c "$bash_command" >"${bash_output}/stdout" 2>"${bash_output}/stderr"
    echo $? >"${bash_output}/status"

    if diff "$minishell_output" "$bash_output" >"${BUILD}/${test_name}/log"; then
        echo -e "${GREEN}✓   ${test_name} passed${NC}"
        ((N_PASSED++))
    else
        echo -e "${RED}✗   ${test_name} failed${NC}"
        ((N_FAILED++))
        FAILED_TESTS+=("${test_name}")
    fi
}

main() {
    N_PASSED=0
    N_FAILED=0
    FAILED_TESTS=()

    compare_with_bash 'HelloWorld'          'echo hello world'
    compare_with_bash 'CanTakeInfile'       'cat INFILE_DIR/Makefile'
    compare_with_bash 'SeparateOutfiles'    'echo 420 > OUTFILE_DIR/out'
    compare_with_bash 'EnvStuff'            'export COOL_NUMBER=420; echo $COOL_NUMBER'

    if [ "$N_FAILED" -eq 0 ]; then
        echo -e "\n${GREEN}==========ALL TESTS PASSED======================================================${NC}"
        exit 0
    else
        echo -e "\n${RED}==========SOME TESTS FAILED=====================================================${NC}"
        echo Failed tests:
        for failed in "${FAILED_TESTS[@]}"; do
            echo "    $failed"
        done
        exit 1
    fi
}

main
