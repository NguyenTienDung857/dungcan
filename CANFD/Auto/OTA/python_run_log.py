import atexit
import os
import sys
from datetime import datetime


_log_file = None
_log_path = None
_original_stdout = None
_original_stderr = None


class TeeStream:
    def __init__(self, original_stream, file_stream):
        self._original_stream = original_stream
        self._file_stream = file_stream

    def write(self, data):
        if self._original_stream:
            self._original_stream.write(data)
        self._file_stream.write(data)
        self.flush()

    def flush(self):
        if self._original_stream:
            self._original_stream.flush()
        self._file_stream.flush()

    def isatty(self):
        if self._original_stream and hasattr(self._original_stream, "isatty"):
            return self._original_stream.isatty()
        return False

    def __getattr__(self, name):
        return getattr(self._original_stream, name)


def _build_log_path(log_dir, script_name):
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    base_name = f"{script_name}_{timestamp}.log"
    log_path = os.path.join(log_dir, base_name)

    suffix = 1
    while os.path.exists(log_path):
        log_path = os.path.join(log_dir, f"{script_name}_{timestamp}_{suffix}.log")
        suffix += 1

    return log_path


def start_python_log(script_dir, script_name):
    global _log_file, _log_path, _original_stdout, _original_stderr

    if _log_file:
        return _log_path

    log_dir = os.path.join(script_dir, "Log_python")
    os.makedirs(log_dir, exist_ok=True)

    _log_path = _build_log_path(log_dir, script_name)
    _log_file = open(_log_path, "a", encoding="utf-8", errors="replace", buffering=1)
    _original_stdout = sys.stdout
    _original_stderr = sys.stderr
    sys.stdout = TeeStream(_original_stdout, _log_file)
    sys.stderr = TeeStream(_original_stderr, _log_file)
    atexit.register(close_python_log)

    print(f"Python run log started: {_log_path}")
    return _log_path


def close_python_log():
    global _log_file, _original_stdout, _original_stderr

    if not _log_file:
        return

    try:
        sys.stdout = _original_stdout
        sys.stderr = _original_stderr
        _log_file.close()
    finally:
        _log_file = None
