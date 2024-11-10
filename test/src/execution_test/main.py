import subprocess


class CommandRunner:
    def __init__(self, command: str):
        self.command = command

    def run(self) -> subprocess.CompletedProcess:
        res = subprocess.run(
            self.command,
            capture_output=True,
            shell=True,
            text=True,
        )
        return res


cmd = CommandRunner("ls -la")
res = cmd.run()
print(res)
