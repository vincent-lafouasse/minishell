setup() {
    load 'bats-extension/bats-support/load'
    load 'bats-extension/bats-assert/load'

    bats_require_minimum_version 1.5.0

    DIR="$(cd "$(dirname "$BATS_TEST_FILENAME")" >/dev/null 2>&1 && pwd)"
    root_dir="${DIR}/../../.."
    minishell="${root_dir}/minishell"
    build_dir="${root_dir}/build"
    mkdir -p "$build_dir"
}

teardown() {
    rm -rf "$build_dir"
}

@test "can build minishell" {
    make -C "$root_dir"
}

@test "hello world" {
    command="echo hello world"
    bash_output="$(bash -c "$command")"
    run -$? bash -c "$minishell -c $command"
    assert_output "$bash_output"
}

@test "command not found" {
    command="i_sure_hope_this_command_doesnt_exist"
    bash_output="$(bash -c "$command")"
    run -$? bash -c "$minishell -c $command" # run -127
    assert_output --partial 'command not found'
}
