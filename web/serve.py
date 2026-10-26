import http.server
import os
import shutil
import socket
import subprocess
import sys
import threading
import webbrowser
from pathlib import Path

HERE = Path(__file__).resolve().parent
SOURCE = HERE / "visualizer.c"
EXE = HERE / ("visualizer.exe" if os.name == "nt" else "visualizer")


class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(HERE), **kwargs)

    def end_headers(self):
        self.send_header("Cache-Control", "no-store, no-cache, must-revalidate")
        self.send_header("Pragma", "no-cache")
        super().end_headers()

    def log_message(self, fmt, *args):
        pass


def pick_port(start=8000):
    for port in range(start, start + 30):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            try:
                sock.bind(("127.0.0.1", port))
            except OSError:
                continue
            return port
    raise RuntimeError("Could not find a free port between 8000 and 8029.")


def build():
    if not SOURCE.exists():
        print("visualizer.c is missing from this folder.")
        return False

    if EXE.exists() and EXE.stat().st_mtime >= SOURCE.stat().st_mtime:
        return True

    gcc = shutil.which("gcc")
    if gcc is None:
        if EXE.exists():
            print("gcc was not found, using the existing visualizer executable.")
            return True
        print("gcc was not found. Install a C compiler or build visualizer.c yourself.")
        return False

    print("Compiling visualizer.c ...")
    result = subprocess.run(
        [gcc, "-std=c99", "-Wall", "-Wextra", str(SOURCE), "-o", str(EXE)]
    )
    if result.returncode != 0:
        print("Compilation failed.")
        return False

    print("Build OK.")
    return True


def main():
    if not build():
        return 1

    port = pick_port()
    server = http.server.ThreadingHTTPServer(("127.0.0.1", port), Handler)
    threading.Thread(target=server.serve_forever, daemon=True).start()

    url = "http://127.0.0.1:%d/viewer.html" % port
    print()
    print("Viewer:  " + url)
    print("Use the C menu in this terminal. The page updates by itself.")
    print("Press Ctrl+C in this window to quit the viewer.")
    print()

    threading.Timer(0.7, lambda: webbrowser.open(url)).start()

    try:
        subprocess.call([str(EXE)], cwd=str(HERE))
    except KeyboardInterrupt:
        pass
    except OSError as error:
        print("Could not run the visualizer: %s" % error)
        server.shutdown()
        return 1

    print()
    print("Visualizer closed. The last state is still on the page.")
    try:
        input("Press Enter to stop the local server...")
    except (KeyboardInterrupt, EOFError):
        pass

    server.shutdown()
    return 0


if __name__ == "__main__":
    sys.exit(main())
