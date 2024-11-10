import unittest
import subprocess
import os


class CommandRunner:
    MINISHELL = 0
    BASH = 1

    def __init__(self, command: str, input: str):
        self.command = command
        self.env = os.environ
        self.input = input

    def run(self, shell) -> subprocess.CompletedProcess:
        if shell == self.MINISHELL:
            command = f'~/code/42/cc/minishell/minishell -c "{self.command}"'
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


class SimpleCommand(unittest.TestCase):
    def test_ls(self):
        command = "ls -la"
        input = None

        runner = CommandRunner(command, input)
        res_bash = runner.run(CommandRunner.BASH)
        res_minishell = runner.run(CommandRunner.MINISHELL)
        self.assertEqual(res_bash.returncode, res_minishell.returncode)
        self.assertEqual(res_bash.stdout, res_minishell.stdout)
        self.assertEqual(res_bash.stderr, res_minishell.stderr)


if __name__ == "__main__":
    unittest.main()
