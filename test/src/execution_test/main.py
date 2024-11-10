import subprocess


class CommandRunner:
    def __init__(self, command: str):
        pass

    def run(self) -> subprocess.CompletedProcess:
        pass


subprocess.run("ls -la", shell=True)
