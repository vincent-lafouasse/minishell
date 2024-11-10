import subprocess


class CommandRunner:
    def __init__(self, command: str):
        self.command = command
        self.input = None

    def with_input(self, input: str):
        self.input = input

    def run(self) -> subprocess.CompletedProcess:
        res = subprocess.run(
            self.command,
            input=self.input,
            capture_output=True,
            shell=True,
            text=True,
        )
        return res


command = "cat | cat | cat | tac"
input = "one\ntwo\nthree\nfour\nfive\nsix\n"
cmd = CommandRunner(command)
cmd.with_input(input)
res = cmd.run()
print(res)
print("output:")
print(res.stdout)
