import subprocess
import os


class CommandRunner:
    def __init__(self, command: str):
        self.command = command
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

cmd = CommandRunner(command)
cmd.input = input
cmd.env["HOME"] = "420"
res = cmd.run()

print(res)
print("output:")
print(res.stdout)
