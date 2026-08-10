#!/usr/bin/env python3
import argparse
import glob
import os
import re
import subprocess
import sys
import tempfile

from concurrent.futures import ThreadPoolExecutor, as_completed
from typing import Callable, Literal

FORMAT_OPTS = "-style=file"
TIDY_OPTS = "-p . --quiet"
MIN_CLANG_VER = 17


def get_clang_version(exe: str) -> int | None:
    try:
        out = subprocess.run(
            [exe, "--version"], check=False, capture_output=True, text=True
        )
        m = re.search(r"version \d+\.\d+\.\d+", out.stdout)
        if m is None:
            sys.stderr.write(f"error: could not determine {exe} version\n")
            return None
        major_ver = m.group().split()[1].split(".")[0]
        return int(major_ver)
    except FileNotFoundError:
        sys.stderr.write(f"error: could not find {exe} binary\n")
        return None


def run(exe: str, args: list[str]) -> tuple[str, str, str, int]:
    out = subprocess.run([exe, *args], check=False, capture_output=True, text=True)
    return args[-1], out.stdout, out.stderr, out.returncode


def get_fixes_file(tmp_dir: str) -> str:
    fd, path = tempfile.mkstemp(suffix=".yaml", dir=tmp_dir)
    os.close(fd)
    return path


def get_clang_tidy_args(
    file: str, tmp_dir: str, use_colour: bool, check: bool
) -> list[str]:
    args = [*TIDY_OPTS.split()]

    if check:
        args.append("--warnings-as-errors=*")
    else:
        args.append(f"-export-fixes={get_fixes_file(tmp_dir)}")

    if use_colour:
        args.append("--use-color")

    args.append(file)

    return args


def get_clang_format_args(file: str, check: bool) -> list[str]:
    if check:
        return [*FORMAT_OPTS.split(), "--dry-run", "-Werror", file]

    return [*FORMAT_OPTS.split(), "-i", file]


NOISY_TIDY_OUTPUT = re.compile(r"\[\d+\/\d+\] \(\d+\/\d+\) Processing file .*")
DIAG_BLOCK_START = re.compile(r"\S+:\d+:\d+: (error|warning|note):")


def print_diag_dedup(stdout: str, seen: set[str]) -> None:
    block: list[str] = []

    def print_block() -> None:
        if block:
            diag = "\n".join(block)
            if diag not in seen:
                seen.add(diag)
                print(diag)

    for line in stdout.splitlines():
        if re.match(DIAG_BLOCK_START, line) and block:
            print_block()
            block.clear()
        block.append(line)
    print_block()


def run_step(
    exe: str,
    args_cb: Callable[[str], list[str]],
    files: list[str],
    label: str,
    num_workers: int,
) -> Literal[0] | Literal[1]:
    ret = 0
    with ThreadPoolExecutor(num_workers) as e:
        print(f"{label} {len(files)} files with {num_workers} jobs...")
        count = 1
        futures = {e.submit(run, exe, args_cb(file)) for file in files}
        seen = set()
        for future in as_completed(futures):
            file, stdout, stderr, code = future.result()
            is_ok = True
            if stdout.strip():
                print_diag_dedup(stdout.strip(), seen)
                is_ok = False
            if code != 0:
                for line in stderr.splitlines():
                    if not re.search(NOISY_TIDY_OUTPUT, line):
                        print(line)
                ret = 1
                is_ok = False

            if is_ok:
                print(f"[{count}/{len(files)}] OK {file}")
            else:
                print(f"[{count}/{len(files)}] WARN {file}")
            count += 1
        return ret


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Format files using clang-format and clang-tidy"
    )
    parser.add_argument(
        "-j", "--jobs", type=int, help="Number of concurrent jobs to run", default=0
    )
    parser.add_argument(
        "--format-executable",
        metavar="PATH",
        help="Name or path to clang-format executable",
        default="clang-format",
    )
    parser.add_argument(
        "--tidy-executable",
        metavar="PATH",
        help="Name or path to clang-tidy executable",
        default="clang-tidy",
    )
    parser.add_argument(
        "--replacements-executable",
        metavar="PATH",
        help="Name or path to clang-apply-replacements executable",
        default="clang-apply-replacements",
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="Verify if files pass checks without modifying them",
        default=False,
    )
    parser.add_argument(
        "--use-colour",
        action="store_true",
        help="Show clang-tidy diagnostics in colour",
        default=False,
    )
    parser.add_argument(
        "--use-color",
        action="store_true",
        help="Show clang-tidy diagnostics in colour (spelled incorrectly)",
        default=False,
    )

    args = parser.parse_args()
    clang_format = args.format_executable
    clang_tidy = args.tidy_executable
    clang_apply_replacements = args.replacements_executable

    clang_format_ver = get_clang_version(clang_format)
    if clang_format_ver is None:
        return 1
    if clang_format_ver < MIN_CLANG_VER:
        sys.stderr.write(
            f"clang-format version: {clang_format_ver}, is less than minimum version: {MIN_CLANG_VER}"
        )
        return 1

    clang_tidy_ver = get_clang_version(clang_tidy)
    if clang_tidy_ver is None:
        return 1
    if clang_tidy_ver < MIN_CLANG_VER:
        sys.stderr.write(
            f"clang-tidy version: {clang_tidy_ver}, is less than minimum version: {MIN_CLANG_VER}"
        )
        return 1

    clang_replacements_ver = get_clang_version(clang_apply_replacements)
    if clang_replacements_ver is None:
        return 1
    if clang_replacements_ver < MIN_CLANG_VER:
        sys.stderr.write(
            f"clang-apply-replacements version: {clang_replacements_ver}, is less than minimum version: {MIN_CLANG_VER}"
        )
        return 1

    tidy_files = glob.glob("src/**/*.c", recursive=True)
    format_files = glob.glob("src/**/*.[ch]", recursive=True)

    num_workers = args.jobs if args.jobs > 0 else min(32, (os.cpu_count() or 1) + 4)
    ret = 0
    with tempfile.TemporaryDirectory() as tmp:
        ret |= run_step(
            clang_tidy,
            lambda file: get_clang_tidy_args(
                file, tmp, args.use_colour or args.use_color, args.check
            ),
            tidy_files,
            "Tidying",
            num_workers,
        )
        if not args.check:
            print("Applying replacements")
            _, _, _, rc = run(clang_apply_replacements, [tmp])
            ret |= rc

    ret |= run_step(
        clang_format,
        lambda file: get_clang_format_args(file, args.check),
        format_files,
        "Formatting",
        num_workers,
    )

    print("Done.")
    return ret


if __name__ == "__main__":
    sys.exit(main())
