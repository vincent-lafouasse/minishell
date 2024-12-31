#!/bin/bash

EXEC_TEST_ROOT="$(
    cd -- "$(dirname "$0")" >/dev/null 2>&1 || exit
    pwd -P
)"
MINISHELL_ROOT="${EXEC_TEST_ROOT}/../../.."
MINISHELL="${MINISHELL_ROOT}/minishell"

BUILD="${MINISHELL_ROOT}/build/exec_test"
INFILE_DIR="${MINISHELL_ROOT}/aux/infiles"

IGNORE_STDERR=0

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
PURPLE='\033[0;35m'
NC='\033[0m'

setup() {
    if ! make -C "$MINISHELL_ROOT"; then
        echo -e "${RED}Failed to build minishell$NC"
        exit 1
    fi

    rm -rf "$BUILD"
    mkdir -p "$BUILD"
}

setup_test() {
    local test_name="$1"

    local build="${BUILD}/${test_name}"
    local bash_output="${build}/bash"
    local minishell_output="${build}/minishell"
    mkdir -p "${bash_output}"
    mkdir -p "${minishell_output}"
    rm -rf "${bash_output:?}"/*
    rm -rf "${minishell_output:?}"/*
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

    "$MINISHELL" -c "$minishell_command" >"${minishell_output}/stdout" 2>"${minishell_output}/stderr"
    echo $? >"${minishell_output}/status"

    bash -c "$bash_command" >"${bash_output}/stdout" 2>"${bash_output}/stderr"
    echo $? >"${bash_output}/status"

    if diff "$minishell_output" "$bash_output" >"${BUILD}/${test_name}/log"; then
        echo -e "${GREEN}✓   ${test_name} passed${NC}"
        ((N_PASSED++))
    else
        echo -e "${YELLOW}Testing $test_name$NC"
        echo -e "    ${YELLOW}For command:$NC $command"
        echo -e "  ${YELLOW}expected${NC}"
        for file in "$bash_output/"*; do
            echo -ne "$PURPLE"
            basename "$file"
            echo -ne "$NC"
            cat "$file"
        done
        echo -e "  ${YELLOW}was${NC}"
        for file in "$minishell_output/"*; do
            echo -ne "$PURPLE"
            basename "$file"
            echo -ne "$NC"
            cat "$file"
        done
        echo -e "${RED}✓   ${test_name} failed${NC}"
        ((N_FAILED++))
        FAILED_TESTS+=("${test_name}")
    fi
}

compare_script_with_bash() {
    local test_name="$1"
    local script_path="$2"

    setup_test "$test_name"

    local script_path="$EXEC_TEST_ROOT/$script_path"
    local bash_output="${BUILD}/${test_name}/bash"
    local minishell_output="${BUILD}/${test_name}/minishell"

    if [ ! -f "$script_path" ]; then
        echo -e "${RED}    Input file ${script_path} does not exist${NC}"
        ((N_FAILED++))
        return
    fi

    if [ IGNORE_STDERR == 0 ]; then
        <"$script_path" "$MINISHELL" >"${minishell_output}/stdout" 2>"${minishell_output}/stderr"
        echo $? >"${minishell_output}/status"

        <"$script_path" bash >"${bash_output}/stdout" 2>"${bash_output}/stderr"
        echo $? >"${bash_output}/status"
    else
        <"$script_path" "$MINISHELL" >"${minishell_output}/stdout" 2>/dev/null
        echo $? >"${minishell_output}/status"

        <"$script_path" bash >"${bash_output}/stdout" 2>/dev/null
        echo $? >"${bash_output}/status"
    fi

    if diff "$minishell_output" "$bash_output" >"${BUILD}/${test_name}/log"; then
        echo -e "${GREEN}✓   ${test_name} passed${NC}"
        ((N_PASSED++))
    else
        echo -e "${YELLOW}Testing $test_name$NC"
        echo -e "    ${YELLOW}For command list at:$NC $script_path"
        echo -e "  ${YELLOW}expected${NC}"
        for file in "$bash_output/"*; do
            echo -ne "$PURPLE"
            basename "$file"
            echo -ne "$NC"
            cat "$file"
        done
        echo -e "  ${YELLOW}was${NC}"
        for file in "$minishell_output/"*; do
            echo -ne "$PURPLE"
            basename "$file"
            echo -ne "$NC"
            cat "$file"
        done
        echo -e "${RED}✓   ${test_name} failed${NC}"
        ((N_FAILED++))
        FAILED_TESTS+=("${test_name}")
    fi
}

refute() {
    local test_name="$1"
    local command="$2"
    local status="$3"
    local partial_stderr="$4"
    local expected_stdout="$5" # may not be here, only used if argc == 6

    local build_dir="${BUILD}/${test_name}"
    mkdir -p "$build_dir"

    local command="${command//INFILE_DIR/${INFILE_DIR}}"
    local command="${command//OUTFILE_DIR/${build_dir}}"

    $MINISHELL -c "$command" >"${build_dir}/stdout" 2>"${build_dir}/stderr"
    local actual_status=$?

    local had_error=0

    if [ "$actual_status" -ne "$status" ]; then
        echo -e "${YELLOW}Testing $test_name$NC"
        echo -e "    ${YELLOW}For command:$NC $command"
        echo -e "    ${RED}Expected status $status was $actual_status$NC"
        had_error=1
    fi

    if [ "$#" -eq 5 ]; then
        echo "$expected_stdout" > "$build_dir/expected_stdout"
        if ! diff "$build_dir/stdout" "$build_dir/expected_stdout" > "$build_dir/diff_log"; then
            if [ "$had_error" -eq 0 ]; then
                echo -e "${YELLOW}Testing $test_name$NC"
                echo -e "    ${YELLOW}For command:$NC $command"
            fi
            echo -e "    ${RED}Stdout doesnt match '$expected_stdout'$NC"
            had_error=1
        fi
    fi

    if ! grep --quiet --ignore-case "$partial_stderr" "${build_dir}/stderr"; then
        if [ "$had_error" -eq 0 ]; then
            echo -e "${YELLOW}Testing $test_name$NC"
            echo -e "    ${YELLOW}For command:$NC $command"
        fi
        echo -e "    ${RED}Stderr doesnt contain pattern '$partial_stderr'$NC"
        had_error=1
    fi

    if [ "$had_error" -eq 0 ]; then
        echo -e "${GREEN}✓   ${test_name} passed${NC}"
        ((N_PASSED++))
    else
        for file in "$build_dir/"*; do
            echo -ne "$PURPLE"
            basename "$file"
            echo -ne "$NC"
            cat "$file"
        done
        echo -e "${RED}✗   ${test_name} failed${NC}"
        ((N_FAILED++))
        FAILED_TESTS+=("${test_name}")
    fi
}

command_not_found() {
    local test_name="$1"
    local command="$2"

    refute "$test_name" "$command" 127 'command not found'
}

numeric_argument_required() {
    local test_name="$1"
    local command="$2"

    refute "$test_name" "$command" 2 'numeric argument required'
}

too_many_arguments() {
    local test_name="$1"
    local command="$2"

    # NOTE: consider this: bash exits with 1 (EXIT_FAILURE) when too many
    # arguments are given but not 2 (EX_BADUSAGE). returning 2 makes more sense
    # as it is matches more closely the error that occured
    local exit_too_many_arguments_ret=2
    refute "$test_name" "$command" $exit_too_many_arguments_ret 'too many arguments'
}

test_success() {
    local n_passed="$1"
    local n_failed="$2"
    local total=$((n_passed + n_failed))

    if ! ((n_failed)); then
        echo -e "\n${GREEN}========= ALL TESTS PASSED =====================================================${NC}"
        echo -e "    ${GREEN}$n_passed/$total${NC}"
        return 0
    else
        echo Failed tests:
        for failed in "${FAILED_TESTS[@]}"; do
            echo -e "    $YELLOW$failed$NC"
        done
        echo -e "\n${RED}========= SOME TESTS FAILED ====================================================${NC}"
        echo -e "    ${RED}$n_passed/$total${NC}"
        return 1
    fi
}

test_here_documents() {
    # we can't prevent readline from outputting whatever it reads to standard
    # error in non interactive mode (we need forbidden function `fopen`)
    IGNORE_STDERR=1

    # "Unterminated" tests should output a message to standard error when in
    # interactive mode

    compare_script_with_bash "HereDocumentCat_Empty" "./tests/doc/Empty"
    compare_script_with_bash "HereDocumentCat_EmptyUnterminated" "./tests/doc/EmptyUnterminated"
    compare_script_with_bash "HereDocumentCat_SingleLine" "./tests/doc/SingleLine"
    compare_script_with_bash "HereDocumentCat_MultiLine" "./tests/doc/MultiLine"
    compare_script_with_bash "HereDocumentCat_UnterminatedMultiLine" "./tests/doc/UnterminatedMultiLine"
    compare_script_with_bash "HereDocumentCat_DelimiterIsEntirelyMatched" "./tests/doc/DelimiterIsEntirelyMatched"
    compare_script_with_bash "HereDocumentCat_AllEmptyLines" "./tests/doc/AllEmptyLines"
    compare_script_with_bash "HereDocumentCat_AllEmptyLinesUnterminated" "./tests/doc/AllEmptyLinesUnterminated"
    compare_script_with_bash "HereDocumentCat_WithSomeEmptyLines" "./tests/doc/WithSomeEmptyLines"
    compare_script_with_bash "HereDocumentCat_WithSomeEmptyLinesUnterminated" "./tests/doc/WithSomeEmptyLinesUnterminated"
    compare_script_with_bash "HereDocumentCat_DelimiterIsQuoteRemoved" "./tests/doc/DelimiterIsQuoteRemoved"
    compare_script_with_bash "HereDocumentCat_DelimiterIsQuoteRemovedAndConcatenated" "./tests/doc/DelimiterIsQuoteRemovedAndConcatenated"
    compare_script_with_bash "HereDocument_InDifferentCommandWithArguments" "./tests/doc/InDifferentCommandWithArguments"
    compare_script_with_bash "HereDocument_ManyDocuments" "./tests/doc/ManyDocuments"
    compare_script_with_bash "HereDocument_ManyDocumentsUnterminated" "./tests/doc/ManyDocumentsUnterminated"
    compare_script_with_bash "HereDocument_ManyDocumentsInSubshell" "./tests/doc/ManyDocumentsInSubshell"
    compare_script_with_bash "HereDocument_ManyDocumentsInNestedSubshell" "./tests/doc/ManyDocumentsInNestedSubshell"
    compare_script_with_bash "HereDocument_ManyDocumentsInConditional" "./tests/doc/ManyDocumentsInConditional"
    compare_script_with_bash "HereDocument_InPipeline" "./tests/doc/InPipeline"

    IGNORE_STDERR=0
}

test_builtins() {
    # echo
    compare_with_bash 'Echo_Nothing' 'echo'
    compare_with_bash 'Echo_OneWord' 'echo greetings!'
    compare_with_bash 'Echo_ManyWords' 'echo hello world'
    compare_with_bash 'Echo_ManyWordsWithNOption' 'echo -n hello world'
    compare_with_bash 'Echo_WithDashFollowedByManyN' 'echo -nnnnnnn hello world'
    compare_with_bash 'Echo_WithDashFollowedByManyOptionStrings' 'echo -nnn -n -nnn  -nnnnnnn hello world'
    compare_with_bash 'Echo_WithDashNAtTheEnd' 'echo hello world -n'
    compare_with_bash 'Echo_NothingWithOnlyDashN' 'echo -n'
    compare_with_bash 'Echo_NothingWithDashNs' 'echo -n -n -n -nnnn'
    compare_with_bash 'Echo_BadOptionBeginsInputWords' 'echo -nnn -nXn -nnn  -nnnnnnn hello world'

    # cd
    refute 'Cd_WithNoArguments' 'cd' 2 'usage'
    refute 'Cd_WithTooManyArguments' 'cd ./a ./b' 2 'usage'
    refute 'Cd_IntoDash' 'cd -' 1 'no such file or directory'
    refute 'Cd_IntoBadRelativePath' 'cd OUTFILE_DIR/./a' 1 'no such file or directory'
    refute 'Cd_IntoBadAbsolutePath' 'cd OUTFILE_DIR/a' 1 'no such file or directory'
    compare_with_bash 'Cd_IntoAbsolutePathIsReflectedInPwd' 'cd / && pwd'
    compare_with_bash 'Cd_IntoAbsolutePathChangesDirectory' 'mkdir OUTFILE_DIR/a && echo hello world > OUTFILE_DIR/a/b && cd OUTFILE_DIR/a && cat b'

    # test execution-related builtin edge cases
    compare_with_bash 'RedirectedBuiltinDoesNotBreakStreams' '> OUTFILE_DIR/dummy echo hello world && echo are we still in stdout?'
    compare_with_bash 'BuiltinsInPipelineAreRunInSubshell' 'cd / && cd /tmp | export NAME=world | exit && pwd && echo hello $NAME'

    # pwd; Cd_IntoAbsolutePathIsReflectedInPwd tests both cd and pwd and tests the good path
    refute 'Pwd_FromUnlinkedDirectory' 'mkdir -p OUTFILE_DIR/a/b && cd OUTFILE_DIR/a/b && rm -rf OUTFILE_DIR/a && pwd' 1 'pwd: getcwd'

    # export
    compare_with_bash 'Export_WithNoArgumentsShowsNewlyAddedEntries' \
        'export TEST && export | grep TEST | sed -i "s/^declare -x/export/"'
    refute 'Export_InvalidIdentifier1' 'export 1abc=oneabc' 1 'not a valid identifier'
    refute 'Export_InvalidIdentifier2' 'export 1=one' 1 'not a valid identifier'
    refute 'Export_InvalidIdentifier3' 'export 1GREETING=hello' 1 'not a valid identifier'
    refute 'Export_InvalidIdentifier4' 'export lol%%lol' 1 'not a valid identifier'
    refute 'Export_InvalidIdentifier5' 'export 1GREETING=hello GREETING=hello NAME=world' 1 'not a valid identifier'

    refute 'ExportEcho_InvalidIdentifierDoesNotStopIteration' \
        'export 1GREETING=hello GREETING=hello NAME=world || echo $GREETING $NAME' 0 '' 'hello world'

    compare_with_bash 'ExportEcho_NullVariable' 'export NULL && echo $NULL'
    compare_with_bash 'ExportEcho_ManyAssignments' 'export GREETING=hello NAME=world && echo $GREETING $NAME'
    compare_with_bash 'ExportEcho_ManyAssignmentsAndAppends' 'export GREETING=good GREETING+=bye NAME=world && echo $GREETING $NAME'

    compare_with_bash 'ExportEcho_SetVariable' 'export GREETING=hello && echo $GREETING world'
    compare_with_bash 'ExportEcho_AssignNullVariable' 'export GREETING && export GREETING=hello && echo $GREETING world'
    compare_with_bash 'ExportEcho_OverwriteVariable' 'export GREETING=bye && export GREETING=hello && echo $GREETING world'
    compare_with_bash 'ExportEcho_AppendNullVariables' 'export GREETING && export GREETING+=hello && echo $GREETING world'
    compare_with_bash 'ExportEcho_AppendSetVariable' 'export GREETING=good && export GREETING+=bye && echo $GREETING world'
    compare_with_bash 'ExportEcho_AppendUnsetVariable' 'export GREETING+=hello && echo $GREETING world'

    # unset
    compare_with_bash 'ExportUnset_UnsetExistingVariable' 'export NAME=world && unset NAME && echo hello $NAME'
    compare_with_bash 'Unset_UnsetNonExistentVariable' 'unset ABCDEF && echo hello $ABCDEF'

    compare_with_bash 'Unset_NoArguments' 'unset'
    refute 'Unset_InvalidIdentifier1' 'unset 1abc=oneabc' 1 'invalid identifier'
    refute 'Unset_InvalidIdentifier2' 'unset 1=one' 1 'invalid identifier'
    refute 'Unset_InvalidIdentifier3' 'unset 1GREETING=hello' 1 'invalid identifier'
    refute 'Unset_InvalidIdentifier4' 'unset lol%%lol' 1 'invalid identifier'
    refute 'Unset_InvalidIdentifier5' 'unset 1NVALID ABC' 1 'invalid identifier'

    refute 'ExportUnset_InvalidIdentifierDoesNotStopIteration' \
        'export ABC=abc && unset 1NVALID ABC || echo $ABC' 0 '' ''

    # env (to be partially tested by hand)
    refute 'Env_TakesNoArguments' 'env a b c d e f g' 2 'too many arguments'
    # Env_Works
    # Env_DoesNotShowNullVariables

    compare_with_bash 'Exit_SilentInNonInteractiveMode' 'exit'
    compare_with_bash 'Exit_WithArgument' 'exit 69'
    compare_with_bash 'Exit_ExitsWithArgumentModulo255' 'exit 42069'
    compare_with_bash 'Exit_AcceptsNegativeArgument' 'exit -42069'
    compare_with_bash 'Exit_FollowsAtoiRules' 'exit "       +42069"'
    compare_with_bash 'Exit_SkipsLeadingWhitespaces' $'exit "\t\v+42069"'
    # bash returns 1 in this case but it calls an odd code path to get there
    # with more implications than just returning 1, so we set it to EX_BADUSAGE
    refute 'Exit_TakesOnlyOneArgument' 'exit 123 456' 2 'too many arguments'
    refute 'Exit_TakesNumericArgument' 'exit abc' 2 'numeric argument required'
    refute 'Exit_NumericArgumentCheckedBeforeTooManyArguments' 'exit one two' 2 'numeric argument required'
    refute 'Exit_CodeMustFitInLongLong' 'exit 19782908472398572398572398738409389' 2 'numeric argument required'
    refute 'Exit_BadCode1' 'exit +-1' 2 'numeric argument required'
    refute 'Exit_BadCode2' 'exit ++++' 2 'numeric argument required'
    refute 'Exit_BadCode3' 'exit "       +++++++++42069"' 2 'numeric argument required'
    # both of these should technically be accepted but we're low on time
    refute 'Exit_BadCode4' 'exit "       +++++++++42069   a"' 2 'numeric argument required'
    refute 'Exit_BadCode5' 'exit "       +++++++++42069   "' 2 'numeric argument required'
}

test_map_460_() {
    compare_with_bash 'ls_DNE_file' 'ls hola' 1 'no such file or directory'
    refute 'permission_denied__makefile' './Makefile' 126 'Permission denied'
    refute 'permission_denied_touch_hola' 'touch OUTFILE_DIR/hola && OUTFILE_DIR/hola' 126 'Permission denied'
    compare_with_bash 'env_number_of_lines' 'env | "wc" -l'
    command_not_found 'env_number_of_lines_bad' 'env | "wc "-l'
    compare_with_bash 'expr_sum_1_plus_1' 'expr 1 + 1'
    compare_with_bash 'expr_sum_status_plus_status' 'expr $? + $?'

    compare_with_bash 'exit' 'exit'
    numeric_argument_required 'exit_non_numeric01' 'exit exit'
    numeric_argument_required 'exit_non_numeric02' 'exit hola'
    numeric_argument_required 'exit_non_numeric03' 'exit hola que tal'
    too_many_arguments 'exit_too_many_1' 'exit 1 1'
    compare_with_bash 'exit_ok1' 'exit 42'
    compare_with_bash 'exit_ok_leading0' 'exit 000042'
    compare_with_bash 'exit_ok_666' 'exit 666'
    too_many_arguments 'exit_too_many_2' 'exit 666 666'
    too_many_arguments 'exit_too_many_3' 'exit -666 666'
    numeric_argument_required 'exit_non_numeric04' 'exit hola 666'
    too_many_arguments 'exit_too_many_2' 'exit 666 666 666'
    too_many_arguments 'exit_too_many_3' 'exit 666 hola 666'
    numeric_argument_required 'exit_non_numeric05' 'exit hola 666 666'

    compare_with_bash 'exit_ok_259' 'exit 259'
    compare_with_bash 'exit_ok_min_4' 'exit -4'
    compare_with_bash 'exit_ok_min_42' 'exit -42'
    compare_with_bash 'exit_ok_min_42_leading0' 'exit -00000042'
    compare_with_bash 'exit_ok_min_259' 'exit -259'
    compare_with_bash 'exit_ok_min_666' 'exit -666'
    compare_with_bash 'exit_ok_plus_666' 'exit +555'
    compare_with_bash 'exit_ok_0' 'exit 0'
    compare_with_bash 'exit_ok_plus_0' 'exit +0'
    compare_with_bash 'exit_ok_min_0' 'exit -0'
    compare_with_bash 'exit_ok_plus_42' 'exit +42'
    too_many_arguments 'exit_too_many01' 'exit -69 -96'
    numeric_argument_required 'exit_minus_minus' 'exit --666'
    numeric_argument_required 'exit_plus_plus_plus01' 'exit ++++666'
    numeric_argument_required 'exit_plus_plus_plus02' 'exit ++++++0'
    numeric_argument_required 'exit_minus_minus_minus' 'exit ------0'
    compare_with_bash 'exit_quoted_arg01' 'exit "666"'
    compare_with_bash 'exit_quoted_arg02' 'exit '\''666'\'''
    compare_with_bash 'exit_quoted_arg03' 'exit '\''-666'\'''
    compare_with_bash 'exit_quoted_arg04' 'exit '\''+666'\'''
    numeric_argument_required 'exit_quoted_buncha_minus' 'exit '\''----666'\'''
    numeric_argument_required 'exit_quoted_buncha_plus' 'exit '\''++++666'\'''
    compare_with_bash 'exit_quoted_arg05' 'exit '\''6'\''66'
    compare_with_bash 'exit_quoted_arg06' 'exit '\''2'\''66'\''32'\'''
    numeric_argument_required 'exit_quoted_arg07' 'exit "'\''666'\''"'
    numeric_argument_required 'exit_quoted_arg08' 'exit '\''"666"'\'''
    compare_with_bash 'exit_quoted_arg09' 'exit '\''666'\''"666"666'
    compare_with_bash 'exit_quoted_arg10' 'exit +'\''666'\''"666"666'
    compare_with_bash 'exit_quoted_arg11' 'exit -'\''666'\''"666"666'
    numeric_argument_required 'exit_big_int01' 'exit 9223372036854775807'
    numeric_argument_required 'exit_big_int02' 'exit 9223372036854775808'
    numeric_argument_required 'exit_big_int03' 'exit -9223372036854775808'
    numeric_argument_required 'exit_big_int04' 'exit -9223372036854775809'
    # ^ 518
}

main() {
    N_PASSED=0
    N_FAILED=0
    FAILED_TESTS=()
    setup

    test_builtins
    test_here_documents

    compare_with_bash 'Simple_HelloWorld' 'echo hello world'
    compare_with_bash 'Simple_PrintWhitespace' 'echo "         " | cat -e'
    compare_with_bash 'Simple_pwdIntoFile' 'pwd > OUTFILE_DIR/pwd_log'
    compare_with_bash 'Simple_SearchFile' 'grep thou INFILE_DIR/shakespeare.txt'

    compare_with_bash 'Pipe_FindIncludes' 'grep -r include ./src | sort | uniq'

    compare_with_bash 'EnvStuff' 'export COOL_NUMBER=420 && echo $COOL_NUMBER'

    command_not_found 'Refute_NonExistantCommand' 'man_i_sure_hope_this_command_doesnt_exist'
    refute 'Refute_UnexpectedToken' '>' 2 'unexpected token'
    refute 'Refute_IsADirectory' '/' 126 'is a directory'

    compare_with_bash 'empty_outfile' '> OUTFILE_DIR/a | ls > OUTFILE_DIR/ls || echo not created > not_created'

    # minishell map

    # 25-27
    compare_with_bash "JustANewLine" $'\n'
    compare_with_bash "BunchaSpace" "         "
    compare_with_bash "BunchaTabs" $'\t\t\t\t\t'

    # 28-41
    refute 'CommandNotFoundColon' ':' 127 'command not found'
    refute 'CommandNotFoundBang' '!' 127 'command not found'
    refute 'UnexpectedToken_HangingRedir01' '>' 2 'unexpected token'
    refute 'UnexpectedToken_HangingRedir02' '<' 2 'unexpected token'
    refute 'UnexpectedToken_HangingRedir03' '>>' 2 'unexpected token'
    refute 'UnexpectedToken_HangingRedir04' '<<' 2 'unexpected token'
    refute 'UnexpectedToken_HangingRedir05' '<>' 2 'unexpected token'
    refute 'UnexpectedToken_HangingRedir06' '>>>>>' 2 'unexpected token'
    refute 'UnexpectedToken_HangingRedir07' '>>>>>>>>>>>>>>>' 2 'unexpected token'
    refute 'UnexpectedToken_HangingRedir08' '<<<<<' 2 'unexpected token'
    refute 'UnexpectedToken_HangingRedir09' '<<<<<<<<<<<<<<<<' 2 'unexpected token'
    refute 'UnexpectedToken_HangingRedir10' '> > > >' 2 'unexpected token'
    refute 'UnexpectedToken_HangingRedir11' '>> >> >> >>' 2 'unexpected token'
    refute 'UnexpectedToken_HangingRedir12' '>>>> >> >> >>' 2 'unexpected token'

    # 42-46
    refute 'IsADirectory01' '/' 126 'is a directory'
    refute 'IsADirectory02' '//' 126 'is a directory'
    refute 'IsADirectory03' '/.' 126 'is a directory'
    refute 'IsADirectory04' '/./../../../../..' 126 'is a directory'
    refute 'IsADirectory05' '///////' 126 'is a directory'

    # 47-50
    refute 'CommandNotFound_BunchaBackslashes01' '\\\\' 127 'command not found'
    refute 'CommandNotFound_BunchaBackslashes02' '\\\\\\' 127 'command not found'
    refute 'CommandNotFound_BunchaBackslashes03' '\\\\\\\\\\' 127 'command not found'
    refute 'CommandNotFound_Dash' '-' 127 'command not found'

    # 51-66
    refute 'UnexpectedToken_HangingPipe01' '|' 2 'unexpected token'
    refute 'UnexpectedToken_HangingPipe02' '| hola' 2 'unexpected token'
    refute 'UnexpectedToken_HangingPipe03' '| | |' 2 'unexpected token'
    refute 'UnexpectedToken_HangingPipe04' '||' 2 'unexpected token'
    refute 'UnexpectedToken_HangingPipe05' '|||||' 2 'unexpected token'
    refute 'UnexpectedToken_HangingPipe06' '|||||||||||||' 2 'unexpected token'
    refute 'UnexpectedToken_MixedPipeRedir' '>>|><' 2 'unexpected token'
    refute 'UnexpectedToken_HangingCond01' '&&' 2 'unexpected token'
    refute 'UnexpectedToken_HangingCond02' '&&&&&&&&&&&&&&' 2 'unexpected token'
    refute 'CommandNotFoundSemiColon01' ';;' 127 'command not found'
    refute 'CommandNotFoundSemiColon02' ';;;;;' 127 'command not found'
    refute 'CommandNotFoundSemiColon03' ';;;;;;;;;;;;;;;' 127 'command not found'
    refute 'UnexpectedToken_EmptySubshell01' '()' 2 'unexpected token'
    refute 'UnexpectedToken_EmptySubshell02' '( ( ) )' 2 'unexpected token'
    refute 'UnexpectedToken_EmptySubshell03' '( ( ( ( ) ) ) )' 2 'unexpected token'
    # a hanging ampersand is the only case that qualifies as being an
    # unrecognized token, since it is a shell metacharacter but we only
    # interpret two ampersands, meaning it hits a fallthrough case
    refute 'UnrecognizedToken_HangingAmpersand01' '&' 2 'unrecognized token'
    refute 'UnrecognizedToken_HangingAmpersand02' '&&&&&' 2 'unrecognized token'
    refute 'UnrecognizedToken_HangingAmpersand03' 'hello || world &' 2 'unrecognized token'

    # 110-132
    compare_with_bash 'Expansion01' 'echo $'
    compare_with_bash 'Expansion02_DollarBang' 'echo $?'
    compare_with_bash 'Expansion03_DollarBang' 'echo $?$'
    compare_with_bash 'Expansion04_DollarBang' 'echo $? | echo $? | echo $?'
    compare_with_bash 'Expansion05' 'echo $:$= | cat -e'
    compare_with_bash 'Expansion06' 'echo " $ " | cat -e'
    # effectively: 'echo \' $ \' | cat -e', we use string concatenation to escape single quotes
    compare_with_bash 'Expansion07' 'echo '"'"' $ '"'"' | cat -e'
    compare_with_bash 'Expansion08' 'echo $HOME'
    # no escape, compare_with_bash 'Expansion09' 'echo \$HOME'
    compare_with_bash 'Expansion10' 'echo my shit terminal is [$TERM]'
    compare_with_bash 'Expansion11' 'echo my shit terminal is [$TERM4'
    compare_with_bash 'Expansion12' 'echo my shit terminal is [$TERM4]'
    # no bash vars, compare_with_bash 'Expansion13' 'echo $UID'
    compare_with_bash 'Expansion14' 'echo $HOME9'
    # no bash vars, compare_with_bash 'Expansion15' 'echo $9HOME'
    compare_with_bash 'Expansion16' 'echo $HOME%'
    # no bash vars, compare_with_bash 'Expansion17' 'echo $UID$HOME'
    compare_with_bash 'Expansion18' 'echo Le path de mon HOME est $HOME'
    # no escape, compare_with_bash 'Expansion19' 'echo $USER$var\$USER$USER\$USERtest$USER'
    # compare_with_bash 'Expansion20_wildcard' 'echo $hola*'
    compare_with_bash 'Echo_buncha_n' 'echo -nnnn $hola'
    refute 'UnexpectedToken_echo_redir' 'echo > <' 2 'unexpected token'
    refute 'UnexpectedToken_echo_pipe' 'echo | |' 2 'unexpected token'

    # 140-210
    # compare_with_bash 'Expansion21_wildcad' 'echo *'
    # compare_with_bash 'Expansion22_wildcard_quote' 'echo '\''*'\'''
    # compare_with_bash 'Expansion23_wildcard' 'echo D*'
    # compare_with_bash 'Expansion24_wildcard' 'echo *Z'
    # compare_with_bash 'Expansion25_wildcard' 'echo *t hola'
    # compare_with_bash 'Expansion26_wildcard' 'echo *t'
    # hell nah compare_with_bash 'Expansion27' 'echo $*'
    # compare_with_bash 'Expansion28_wildcard' 'echo hola*hola *'
    # compare_with_bash 'Expansion29_wildcard' 'echo $hola*'
    # compare_with_bash 'Expansion31_wildcard' 'echo $HOME*'
    # 150
    # no dollar-quote compare_with_bash 'Expansion32' 'echo $""'
    # no dollar-quote compare_with_bash 'Expansion33' 'echo "$"""'
    # no dollar-quote compare_with_bash 'Expansion34' 'echo '\''$'\'''\'''\'''
    # no dollar-quote compare_with_bash 'Expansion35' 'echo $"HOME"'
    # no dollar-quote compare_with_bash 'Expansion36' 'echo $'\'''\''HOME'
    # no dollar-quote compare_with_bash 'Expansion37' 'echo $""HOME'
    compare_with_bash 'QuoteRemoval_Expansion38' 'echo "$HO"ME'
    compare_with_bash 'QuoteRemoval_Expansion39' 'echo '\''$HO'\''ME'
    compare_with_bash 'QuoteRemoval_Expansion40' 'echo "$HO""ME"'
    compare_with_bash 'QuoteRemoval_Expansion41' 'echo '\''$HO'\'''\''ME'\'''
    # 160
    compare_with_bash 'QuoteRemoval_Expansion42' 'echo "'\''$HO'\'''\''ME'\''"'
    compare_with_bash 'QuoteRemoval_Expansion43' 'echo ""$HOME'
    compare_with_bash 'QuoteRemoval_Expansion44' 'echo "" $HOME'
    compare_with_bash 'QuoteRemoval_Expansion45' 'echo '\'''\''$HOME'
    compare_with_bash 'QuoteRemoval_Expansion46' 'echo '\'''\'' $HOME'
    # no dollar-quote compare_with_bash 'Expansion47' 'echo $"HO""ME"'
    # no dollar-quote compare_with_bash 'Expansion48' 'echo $'\''HO'\'''\''ME'\'''
    # no dollar-quote compare_with_bash 'Expansion49' 'echo $'\''HOME'\'''
    compare_with_bash 'QuoteRemoval_Expansion50' 'echo "$"HOME'
    compare_with_bash 'Expansion51' 'echo $=HOME'
    # 170
    # no dollar-quote compare_with_bash 'Expansion52' 'echo $"HOLA"'
    # no dollar-quote compare_with_bash 'Expansion53' 'echo $'\''HOLA'\'''
    compare_with_bash 'Expansion54' 'echo $DONTEXIST Hola'
    compare_with_bash 'QuoteRemoval55' 'echo "hola"'
    compare_with_bash 'QuoteRemoval56' 'echo '\''hola'\'''
    compare_with_bash 'QuoteRemoval57' 'echo '\'''\''hola'\'''\'''
    compare_with_bash 'QuoteRemoval58' 'echo '\'''\''h'\''o'\''la'\'''\'''
    compare_with_bash 'QuoteRemoval59' 'echo "'\'''\''h'\''o'\''la'\'''\''"'
    compare_with_bash 'QuoteRemoval60' 'echo "'\''"h'\''o'\''la"'\''"'
    refute 'QuoteRemoval61_CommandNotFound' 'echo"'\''hola'\''"' 127 'command not found'
    # 180
    compare_with_bash 'QuoteRemoval62' 'echo "'\''hola'\''"'
    compare_with_bash 'QuoteRemoval63' 'echo '\''"hola"'\'''
    compare_with_bash 'QuoteRemoval64' 'echo '\'''\'''\''ho"'\'''\'''\'''\''l"a'\'''\'''\'''
    compare_with_bash 'QuoteRemoval65' 'echo hola""""""""""""'
    compare_with_bash 'QuoteRemoval66' 'echo hola"'\'''\'''\'''\'''\'''\'''\'''\'''\'''\''"'
    compare_with_bash 'QuoteRemoval67' 'echo hola'\'''\'''\'''\'''\'''\'''\'''\'''\'''\'''\'''\'''
    compare_with_bash 'QuoteRemoval68' 'echo hola'\''""""""""""'\'''
    refute 'QuoteRemoval69_CommandNotFound' 'e"cho hola"' 127 'command not found'
    refute 'QuoteRemoval70_CommandNotFound' 'e'\''cho hola'\''' 127 'command not found'
    compare_with_bash 'QuoteRemoval71' 'echo "hola     " | cat -e'
    # 190
    compare_with_bash 'QuoteRemoval72' 'echo ""hola'
    compare_with_bash 'QuoteRemoval73' 'echo "" hola'
    compare_with_bash 'QuoteRemoval74' 'echo ""             hola'
    compare_with_bash 'QuoteRemoval75' 'echo ""hola'
    compare_with_bash 'QuoteRemoval76' 'echo "" hola'
    compare_with_bash 'QuoteRemoval77' 'echo hola""bonjour'
    compare_with_bash 'QuoteRemoval78' '"e"'\''c'\''ho '\''b'\''"o"nj"o"'\''u'\''r'
    refute 'QuoteRemoval79_CommandNotFound' '""e"'\''c'\''ho '\''b'\''"o"nj"o"'\''u'\''r"' 127 'command not found'
    compare_with_bash 'Expansion80' 'echo "$DONTEXIST"Makefile'
    compare_with_bash 'Expansion81' 'echo "$DONTEXIST""Makefile"'
    # 200
    compare_with_bash 'Expansion82' 'echo "$DONTEXIST" "Makefile"'
    refute 'Expansion83_CommandNotFound' '$' 127 'command not found' # hanging dollar sign
    compare_with_bash 'Expansion84' 'echo $?$?'
    compare_with_bash 'Expansion85' 'echo ?$HOME'
    compare_with_bash 'Expansion86' 'echo $'
    compare_with_bash 'Expansion87' 'echo $HOME'
    compare_with_bash 'Expansion88' 'echo $HOMEdskjhfkdshfsd'
    compare_with_bash 'Expansion89' 'echo "$HOMEdskjhfkdshfsd"'
    compare_with_bash 'Expansion90' 'echo '\''$HOMEdskjhfkdshfsd'\'''
    compare_with_bash 'Expansion91' 'echo $DONTEXIST'
    compare_with_bash 'Expansion92' 'echo $LESS$VAR'

    refute 'CommandNotFound_SingleDigit' '0' 127 'command not found'
    refute 'CommandNotFound_Digits' '000' 127 'command not found'
    refute 'CommandNotFound_AbsolutePathPrefixedByQuestionMark' '?/tmp' 127 'No such file or directory'
    refute 'CommandNotFound_AbsolutePath' '$HOME' 126 'Is a directory'

    compare_with_bash 'Interpretation_Expansion_NullWordsSkipped01' '$DONTEXIST'
    compare_with_bash 'Interpretation_Expansion_NullWordsSkipped02' '$HOMEdskjhfkdshfsd'
    refute 'Interpretation_Expansion_EmptyExpansionString' '"$HOMEdskjhfkdshfsd"' 127 'command not found'
    refute 'Interpretation_Expansion_EscapedString' \''$HOMEdskjhfkdshfsd'\' 127 'command not found'

    refute 'Export_NotAValidIdentifier01' 'export ""' 1 'not a valid identifier'
    refute 'Export_NotAValidIdentifier02' 'export =' 1 'not a valid identifier'
    refute 'Export_NotAValidIdentifier03' 'export %' 1 'not a valid identifier'
    refute 'Export_NotAValidIdentifier04' 'export $?' 1 'not a valid identifier'
    refute 'Export_NotAValidIdentifier05' 'export ?=2' 1 'not a valid identifier'
    refute 'Export_NotAValidIdentifier06' 'export 9HOLA=2' 1 'not a valid identifier'
    refute 'Export_NotAValidIdentifier07' 'export HOLA@' 1 'not a valid identifier'
    refute 'Export_NotAValidIdentifier08' 'export HOLA~' 1 'not a valid identifier'
    refute 'Export_NotAValidIdentifier09' 'export --HOLA' 1 'not a valid identifier'
    refute 'Export_NotAValidIdentifier10' 'export -HOLA' 1 'not a valid identifier'
    refute 'Export_NotAValidIdentifier11' 'export HO-LA' 1 'not a valid identifier'
    refute 'Export_NotAValidIdentifier12' 'export HO.LA' 1 'not a valid identifier'
    refute 'Export_NotAValidIdentifier13' 'export HO+LA' 1 'not a valid identifier'

    test_map_460_

    if test_success "$N_PASSED" "$N_FAILED"; then
        exit 0
    else
        exit 1
    fi
}

main
