import http.server
import json
import os
import shutil
import socket
import subprocess
import sys
import threading
import time
import webbrowser
from pathlib import Path

HERE = Path(__file__).resolve().parent
SOURCE = HERE / "visualizer.c"
EXE = HERE / ("visualizer.exe" if os.name == "nt" else "visualizer")
TRACE = HERE / "trace.json"


class Server(http.server.ThreadingHTTPServer):
    daemon_threads = True

    def handle_error(self, request, client_address):
        error = sys.exc_info()[1]
        if isinstance(error, (BrokenPipeError, ConnectionResetError, ConnectionAbortedError)):
            return
        super().handle_error(request, client_address)


class Handler(http.server.SimpleHTTPRequestHandler):
    protocol_version = "HTTP/1.1"

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(HERE), **kwargs)

    def end_headers(self):
        self.send_header("Cache-Control", "no-store, no-cache, must-revalidate")
        self.send_header("Pragma", "no-cache")
        super().end_headers()

    def do_GET(self):
        if self.path.split("?", 1)[0] == "/events":
            self.stream_trace()
        else:
            super().do_GET()

    def copyfile(self, source, outputfile):
        try:
            super().copyfile(source, outputfile)
        except (BrokenPipeError, ConnectionResetError, ConnectionAbortedError):
            pass

    def stream_trace(self):
        self.send_response(200)
        self.send_header("Content-Type", "text/event-stream; charset=utf-8")
        self.send_header("Cache-Control", "no-store")
        self.send_header("X-Accel-Buffering", "no")
        self.end_headers()

        last_signature = None
        last_ping = time.monotonic()

        try:
            while True:
                payload = None

                try:
                    info = TRACE.stat()
                    signature = (info.st_mtime_ns, info.st_size)

                    if signature != last_signature:
                        text = TRACE.read_text(encoding="utf-8").strip()

                        if text:
                            json.loads(text)
                            payload = text
                            last_signature = signature
                except FileNotFoundError:
                    last_signature = None
                except (OSError, ValueError):
                    pass

                if payload is not None:
                    clean = payload.replace("\r", "").replace("\n", "")
                    self.wfile.write(("data: " + clean + "\n\n").encode("utf-8"))
                    self.wfile.flush()
                    last_ping = time.monotonic()
                elif time.monotonic() - last_ping > 10:
                    self.wfile.write(b": ping\n\n")
                    self.wfile.flush()
                    last_ping = time.monotonic()

                time.sleep(0.25)
        except (BrokenPipeError, ConnectionResetError, ConnectionAbortedError, OSError):
            pass
        finally:
            self.close_connection = True

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
    server = Server(("127.0.0.1", port), Handler)
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
