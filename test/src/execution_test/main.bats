setup() {
    load 'bats-extension/bats-support/load'
    load 'bats-extension/bats-assert/load'

    DIR="$(cd "$(dirname "$BATS_TEST_FILENAME")" >/dev/null 2>&1 && pwd)"
    root_dir="${DIR}/../../.."
    minishell="${root_dir}/minishell"
}

@test "can build minishell" {
    make -C "$root_dir"
}
