import subprocess
import os

MINISHELL = 0
BASH = 1


def unreachable():
    pass


class CommandRunner:
    def __init__(self, command: str, shell):
        if shell == MINISHELL:
            self.command = f'~/code/42/cc/minishell/minishell -c "{command}"'
        elif shell == BASH:
            self.command = command
        else:
            unreachable()
        self.env = os.environ
        self.input = None

    def run(self) -> subprocess.CompletedProcess:
        res = subprocess.run(
            self.command,
            input=self.input,
            capture_output=True,
            shell=True,
            text=True,
            env=self.env,
        )
        return res


command = "echo $HOME"
input = None

# command = "grep f"; input = "one\ntwo\nthree\nfour\nfive\nsix\n"

cmd = CommandRunner(command, MINISHELL)
cmd.input = input
# cmd.env["HOME"] = "420"
res = cmd.run()

print(res)
print("output:")
print(res.stdout)
