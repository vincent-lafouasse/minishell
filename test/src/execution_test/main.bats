setup() {
    load 'bats-extension/bats-support/load'
    load 'bats-extension/bats-assert/load'

    bats_require_minimum_version 1.5.0

    DIR="$(cd "$(dirname "$BATS_TEST_FILENAME")" >/dev/null 2>&1 && pwd)"
    root_dir="${DIR}/../../.."
    minishell="${root_dir}/minishell"
}

@test "can build minishell" {
    make -C "$root_dir"
}

@test "hello world" {
    input="echo hello world"
    run "$minishell" -c "$input"
    assert_output "$(bash -c "$input")"
}

@test "command not found" {
    input="i_sure_hope_this_command_doesnt_exist"
    run -127 "$minishell" -c "$input"
    assert_output --partial 'command not found'
}
