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

test_command() {
    command="$1"
    test_name="$2"
    build="${BUILD}/${test_name}"
    mkdir -p "${build}/bash"
    mkdir -p "${build}/minishell"
}

main() {
    test_command "echo hello world" "HelloWorld"
}

main
