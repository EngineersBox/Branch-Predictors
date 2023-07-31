import csv, subprocess, logging, itertools, re, os
from io import TextIOWrapper
from enum import Enum, IntEnum
from typing import Any, Dict, List, Tuple, Union
from collections import namedtuple, ChainMap

SIFT_TRACES_DIRECTORY = "sift-traces"
SIFT_FILE_EXTENSION = ".sift"

def getTraceNames() -> List[str]:
    return list(map(lambda file: f"{SIFT_TRACES_DIRECTORY}/{file.strip(SIFT_FILE_EXTENSION)}", os.listdir(SIFT_TRACES_DIRECTORY)))

TRACES = getTraceNames()

def executeProcess(command: Union[List[str], str]) -> subprocess.CompletedProcess:
    return subprocess.run(
        command.split(" ") if type(command) == str else command,
        capture_output=True,
        text=True
    )

def runTrace(predictor: str, trace: str):
    command = [
        "./run-sniper",
        "-c", "gainestown",
        "-c", "rob",
        "-c", predictor,
        "-d", "result_directory",
        f"--traces={trace}"
    ]
    result = executeProcess(command)
    if (result.returncode != 0):
        raise RuntimeError(f"Trace failed to execute: {result.stderr}")

BRANCH_PREDICTOR_HIT_COUNT_REGEX = re.compile("branch_predictor\.num\-correct\s=\s[0-9]+")
BRANCH_PREDICTOR_MISS_COUNT_REGEX = re.compile("branch_predictor\.num\-incorrect\s=\s[0-9]+")

def filterBranchResults(stdout: str) -> Tuple[str, str]:
    hits = BRANCH_PREDICTOR_HIT_COUNT_REGEX.findall(stdout)[0]
    misses = BRANCH_PREDICTOR_MISS_COUNT_REGEX.findall(stdout)[0]
    return hits, misses

def getResults() -> str:
    command = [
        "./tools/dumpstats.py",
        "-d", "~/comp3710-uarch-assignment-2/result_directory"
    ]
    result = executeProcess(command)
    if (result.returncode != 0):
        raise RuntimeError(f"Unable to retrieve stats for trace results: {result.stderr}")
    return filterBranchResults(result.stdout)

PREDICTORS = [
    "one_bit",
    "pentium_m",
    "gshare",
    # "bi_mode",
    # "perceptron"
]print(TRACES)


def main():
    results = {}
    for predictor in PREDICTORS:
        for trace in TRACES:
            runTrace(predictor, trace)
            result = getResults()
            if (predictor not in results):
                results[predictor] = {}
            results[predictor][trace] = result
    print(results)

if __name__ == "__main__":
    main()