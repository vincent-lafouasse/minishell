#!/bin/bash

EXEC_TEST_ROOT="$(
    cd -- "$(dirname "$0")" >/dev/null 2>&1 || exit
    pwd -P
)"
MINISHELL_ROOT="${EXEC_TEST_ROOT}/../../.."
MINISHELL="${MINISHELL_ROOT}/minishell"

BUILD="${MINISHELL_ROOT}/build/exec_test"
INFILE_DIR="${MINISHELL_ROOT}/aux/infiles"

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
    echo -e "${YELLOW}Testing $test_name$NC"

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
        echo -e "    ${YELLOW}For command:$NC $command"
        echo -e "${YELLOW}expected${NC}"
        for file in "$bash_output/"*; do
            echo -ne "$PURPLE"
            basename "$file"
            echo -ne "$NC"
            cat "$file"
        done
        echo -e "${YELLOW}was${NC}"
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

    local build_dir="${BUILD}/${test_name}"
    mkdir -p "$build_dir"
    echo -e "${YELLOW}Testing $test_name$NC"

    local command="${command//INFILE_DIR/${INFILE_DIR}}"
    local command="${command//OUTFILE_DIR/${build_dir}}"

    $MINISHELL -c "$minishell_command" >"${build_dir}/stdout" 2>"${build_dir}/stderr"
    local actual_status=$?

    local had_error=0

    if [ "$actual_status" -ne "$status" ]; then
        echo -e "    ${YELLOW}For command:$NC $command"
        echo -e "    ${RED}Expected status $status was $actual_status$NC"
        had_error=1
    fi

    if ! grep --quiet --ignore-case "$partial_stderr" "${build_dir}/stderr"; then
        if [ "$had_error" -eq 0 ]; then
            echo -e "    ${YELLOW}For command:$NC $command"
        fi
        echo -e "    ${RED}Stderr doesnt contain pattern '$partial_stderr'$NC"
        had_error=1
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

main() {
    N_PASSED=0
    N_FAILED=0
    FAILED_TESTS=()
    setup

    compare_with_bash 'Simple_HelloWorld' 'echo hello world'
    compare_with_bash 'Simple_PrintWhitespace' 'echo "         " | cat -e'
    compare_with_bash 'Simple_pwdIntoFile' 'pwd > OUTFILE_DIR/pwd_log'
    compare_with_bash 'Simple_SearchFile' 'grep thou INFILE_DIR/shakespeare.txt'

    compare_with_bash 'Pipe_FindIncludes' 'grep -r include ./src | sort | uniq'

    compare_with_bash 'EnvStuff' 'export COOL_NUMBER=420 && echo $COOL_NUMBER'

    refute 'Refute_NonExistantCommand' 'man_i_sure_hope_this_command_doesnt_exist' 127 'command not found'
    refute 'Refute_UnexpectedToken' '>' 2 'unexpected token'
    refute 'Refute_IsADirectory' '/' 126 'is a directory'

    # minishell map

    # 25-27
    # minishell receives "n" somehow compare_with_bash "JustANewLine" "\n"
    compare_with_bash "BunchaSpace" "         "
    # same here, minishell receives tttt compare_with_bash "BunchaTabs" "\t\t\t\t\t"

    # 28-41
    refute 'UnexpectedToken_NoColon' ':' 2 'unexpected token'
    refute 'UnexpectedToken_NoBang' '!' 2 'unexpected token'
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
    refute 'UnexpectedToken_HangingCond02' '&&&&&' 2 'unexpected token'
    refute 'UnexpectedToken_HangingCond03' '&&&&&&&&&&&&&&' 2 'unexpected token'
    refute 'UnexpectedToken_NoSemiColon01' ';;' 2 'unexpected token'
    refute 'UnexpectedToken_NoSemiColon02' ';;;;;' 2 'unexpected token'
    refute 'UnexpectedToken_NoSemiColon03' ';;;;;;;;;;;;;;;' 2 'unexpected token'
    refute 'UnexpectedToken_EmptySubshell01' '()' 2 'unexpected token'
    refute 'UnexpectedToken_EmptySubshell02' '( ( ) )' 2 'unexpected token'
    refute 'UnexpectedToken_EmptySubshell03' '( ( ( ( ) ) ) )' 2 'unexpected token'

    # 110-132
    compare_with_bash 'Expansion01' 'echo $'
    compare_with_bash 'Expansion02_DollarBang' 'echo $?'
    compare_with_bash 'Expansion03_DollarBang' 'echo $?$'
    compare_with_bash 'Expansion04_DollarBang' 'echo $? | echo $? | echo $?'
    compare_with_bash 'Expansion05' 'echo $:$= | cat -e'
    compare_with_bash 'Expansion06' 'echo " $ " | cat -e'
    compare_with_bash 'Expansion07' 'echo ' $ ' | cat -e'
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
    compare_with_bash 'Expansion20_wildcard' 'echo $hola*'
    compare_with_bash 'Echo_buncha_n' 'echo -nnnn $hola'
    refute 'UnexpectedToken_echo_redir' 'echo > <' 2 'unexpected token'
    refute 'UnexpectedToken_echo_pipe' 'echo | |' 2 'unexpected token'

#    # 140-210
#    compare_with_bash 'Expansion21_wildcad' 'echo *'
#    compare_with_bash 'Expansion22_wildcard_quote' 'echo '\''*'\'''
#    compare_with_bash 'Expansion23_wildcard' 'echo D*'
#    compare_with_bash 'Expansion24_wildcard' 'echo *Z'
#    compare_with_bash 'Expansion25_wildcard' 'echo *t hola'
#    compare_with_bash 'Expansion26_wildcard' 'echo *t'
#    # hell nah compare_with_bash 'Expansion27' 'echo $*'
#    compare_with_bash 'Expansion28_wildcard' 'echo hola*hola *'
#    compare_with_bash 'Expansion29_wildcard' 'echo $hola*'
#    compare_with_bash 'Expansion31_wildcard' 'echo $HOME*'
#    # 150
#    compare_with_bash 'Expansion32' 'echo $""'
#    compare_with_bash 'Expansion33' 'echo "$"""'
#    compare_with_bash 'Expansion34' 'echo '\''$'\'''\'''\'''
#    compare_with_bash 'Expansion35' 'echo $"HOME"'
#    compare_with_bash 'Expansion36' 'echo $'\'''\''HOME'
#    compare_with_bash 'Expansion37' 'echo $""HOME'
#    compare_with_bash 'Expansion38' 'echo "$HO"ME'
#    compare_with_bash 'Expansion39' 'echo '\''$HO'\''ME'
#    compare_with_bash 'Expansion40' 'echo "$HO""ME"'
#    compare_with_bash 'Expansion41' 'echo '\''$HO'\'''\''ME'\'''
#    # 160
#    compare_with_bash 'Expansion42' 'echo "'\''$HO'\'''\''ME'\''"'
#    compare_with_bash 'Expansion43' 'echo ""$HOME'
#    compare_with_bash 'Expansion44' 'echo "" $HOME'
#    compare_with_bash 'Expansion45' 'echo '\'''\''$HOME'
#    compare_with_bash 'Expansion46' 'echo '\'''\'' $HOME'
#    compare_with_bash 'Expansion47' 'echo $"HO""ME"'
#    compare_with_bash 'Expansion48' 'echo $'\''HO'\'''\''ME'\'''
#    compare_with_bash 'Expansion49' 'echo $'\''HOME'\'''
#    compare_with_bash 'Expansion50' 'echo "$"HOME'
#    compare_with_bash 'Expansion51' 'echo $=HOME'
#    # 170
#    compare_with_bash 'Expansion52' 'echo $"HOLA"'
#    compare_with_bash 'Expansion53' 'echo $'\''HOLA'\'''
#    compare_with_bash 'Expansion54' 'echo $DONTEXIST Hola'
#    compare_with_bash 'Expansion55' 'echo "hola"'
#    compare_with_bash 'Expansion56' 'echo '\''hola'\'''
#    compare_with_bash 'Expansion57' 'echo '\'''\''hola'\'''\'''
#    compare_with_bash 'Expansion58' 'echo '\'''\''h'\''o'\''la'\'''\'''
#    compare_with_bash 'Expansion59' 'echo "'\'''\''h'\''o'\''la'\'''\''"'
#    compare_with_bash 'Expansion21' 'echo "'"h'o'la"'"'
#    compare_with_bash 'Expansion21' 'echo"'hola'"'
#    # 180
#    compare_with_bash 'Expansion21' 'echo "'hola'"'
#    compare_with_bash 'Expansion21' 'echo '"hola"''
#    compare_with_bash 'Expansion21' 'echo '''ho"''''l"a''''
#    compare_with_bash 'Expansion21' 'echo hola""""""""""""'
#    compare_with_bash 'Expansion21' 'echo hola"''''''''''"'
#    compare_with_bash 'Expansion21' 'echo hola'''''''''''''
#    compare_with_bash 'Expansion21' 'echo hola'""""""""""''
#    compare_with_bash 'Expansion21' 'e"cho hola"'
#    compare_with_bash 'Expansion21' 'e'cho hola''
#    compare_with_bash 'Expansion21' 'echo "hola     " | cat -e'
#    # 190
#    compare_with_bash 'Expansion21' 'echo ""hola'
#    compare_with_bash 'Expansion21' 'echo "" hola'
#    compare_with_bash 'Expansion21' 'echo ""             hola'
#    compare_with_bash 'Expansion21' 'echo ""hola'
#    compare_with_bash 'Expansion21' 'echo "" hola'
#    compare_with_bash 'Expansion21' 'echo hola""bonjour'
#    compare_with_bash 'Expansion21' '"e"'c'ho 'b'"o"nj"o"'u'r'
#    compare_with_bash 'Expansion21' '""e"'c'ho 'b'"o"nj"o"'u'r"'
#    compare_with_bash 'Expansion21' 'echo "$DONTEXIST"Makefile'
#    compare_with_bash 'Expansion21' 'echo "$DONTEXIST""Makefile"'
#    # 200
#    compare_with_bash 'Expansion21' 'echo "$DONTEXIST" "Makefile"'
#    compare_with_bash 'Expansion21' '$?'
#    compare_with_bash 'Expansion21' '$?$?'
#    compare_with_bash 'Expansion21' '?$HOME'
#    compare_with_bash 'Expansion21' '$'
#    compare_with_bash 'Expansion21' '$HOME'
#    compare_with_bash 'Expansion21' '$HOMEdskjhfkdshfsd'
#    compare_with_bash 'Expansion21' '"$HOMEdskjhfkdshfsd"'
#    compare_with_bash 'Expansion21' ''$HOMEdskjhfkdshfsd''
#    compare_with_bash 'Expansion21' '$DONTEXIST'
#    compare_with_bash 'Expansion21' '$LESS$VAR'

    if test_success "$N_PASSED" "$N_FAILED"; then
        exit 0
    else
        exit 1
    fi
}

main
