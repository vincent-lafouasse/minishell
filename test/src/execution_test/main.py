import unittest
import subprocess
import os


class CommandRunner:
    MINISHELL = 0
    BASH = 1

    def __init__(self, command: str, input: str, env):
        self.command = command
        self.env = env
        self.input = input

    def run(self, shell) -> subprocess.CompletedProcess:
        if shell == self.MINISHELL:
            command = f"~/code/42/cc/minishell/minishell -c '{self.command}'"
        elif shell == self.BASH:
            command = self.command
        res = subprocess.run(
            command,
            input=self.input,
            capture_output=True,
            shell=True,
            text=True,
            env=self.env,
        )
        return res


def compare(command, input, env):
    runner = CommandRunner(command, input, env)
    res_bash = runner.run(CommandRunner.BASH)
    res_minishell = runner.run(CommandRunner.MINISHELL)
    assert res_bash.returncode == res_minishell.returncode
    assert res_bash.stdout == res_minishell.stdout
    assert res_bash.stderr == res_minishell.stderr


class TestDemonstration:
    def test_ls(self):
        command = "ls -la"
        input = None
        env = os.environ
        compare(command, input, env)

    def test_receive_stdin(self):
        command = "cat -e"
        input = "i'm writing in stdin"
        env = os.environ
        compare(command, input, env)

    def test_mutate_env(self):
        command = "echo $COOL_NUMBER"
        input = None
        env = os.environ
        env["COOL_NUMBER"] = "420"
        compare(command, input, env)

    def test_heredoc(self):
        # should not pass ???????
        command = "cat << EOF"
        input = "one\ntwo\nEOF\n"
        env = os.environ
        compare(command, input, env)


if __name__ == "__main__":
    unittest.main(verbosity=2)
