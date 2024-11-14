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
YELLOW='\033[0;33m'
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
    echo -e "${YELLOW}Testing $test_name$NC"

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

refute() {
    local test_name="$1"
    local command="$2"
    local status="$3"
    local partial_stderr="$4"

    local build_dir="${BUILD}/${test_name}"
    mkdir -p "$build_dir"
    echo -e "${YELLOW}Testing $test_name$NC"

    local command="${command//INFILE_DIR/${INFILE_DIR}}"
    local command="${command//OUTFILE_DIR/${build_dir}}"

    $MINISHELL -c "$minishell_command" >"${build_dir}/stdout" 2>"${build_dir}/stderr"
    local actual_status=$?

    local had_error=0

    if [ "$actual_status" -ne "$status" ]; then
        echo -e "    ${RED}Expected status $status was $actual_status$NC"
        had_error=1
    fi

    if [ -z "$partial_stderr" ]; then
        if ! grep -q "${build_dir}/stderr" "$partial_stderr"; then
            echo -e "    ${RED}Stderr doesnt contain pattern \'$partial_stderr\'$NC"
            had_error=1
        fi
    fi

    if [ "$had_error" -eq 0 ]; then
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
    setup

    compare_with_bash 'Simple_HelloWorld'  'echo hello world'
    compare_with_bash 'Simple_pwdIntoFile' 'pwd > OUTFILE_DIR/pwd_log'
    compare_with_bash 'Simple_SearchFile'  'grep thou INFILE_DIR/shakespeare.txt'

    compare_with_bash 'Pipe_FindIncludes' 'grep -r include ./src | sort | uniq'

    compare_with_bash 'EnvStuff' 'export COOL_NUMBER=420 && echo $COOL_NUMBER'

    refute 'Refute_NonExistantCommand' 'man_i_sure_hope_this_command_doesnt_exist' 127 'command not found'

    if [ "$N_FAILED" -eq 0 ]; then
        echo -e "\n${GREEN}==========ALL TESTS PASSED======================================================${NC}"
        exit 0
    else
        echo -e "\n${RED}==========SOME TESTS FAILED=====================================================${NC}"
        echo Failed tests:
        for failed in "${FAILED_TESTS[@]}"; do
            echo -e "    $YELLOW$failed$NC"
        done
        exit 1
    fi
}

main
