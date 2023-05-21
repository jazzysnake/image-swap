import os
import shutil
import random
import argparse
import tqdm
import subprocess

TEST_FILE_PATH = "test.caff"
# range of bytes to modify (1.caff has 162 bytes of preamble)
PREAMBLE_BYTES_RANGE = (0, 162)

def inject_errors(file_path: str, num_errors: int) -> None:
    errors = generate_errors(num_errors)
    logInjectedErrors(errors)
    modify_binary_file(TEST_FILE_PATH, errors)

def generate_errors(num_errors:int) -> dict:
    """
    creates a dict of offset and value pairs that will be changed
    in the test file.
    """
    return {random.randint(
            PREAMBLE_BYTES_RANGE[0],
            PREAMBLE_BYTES_RANGE[1]
            ): random.randint(0, 255)
            for i in range(num_errors)}

def modify_binary_file(file_path: str, offsets: dict) -> None:
    """
    modifies the given file at the given offset-value pairs
    """
    with open(file_path, "r+b") as file:
        for offset, value in offsets.items():
            file.seek(offset)
            file.write(value.to_bytes(1, byteorder="little"))

def logInjectedErrors(errors:dict) -> None:
    print("Offsets and Modified values:")
    for offset, value in errors.items():
        print(f"Offset: {offset}, Value: {value}")

def perform_testing(
        binary_path: str,
        file_path: str,
        num_rounds: int,
        num_errors: int
        ) -> None:
    for round in tqdm.tqdm(range(num_rounds), desc="Error injection round"):
        shutil.copyfile(file_path, TEST_FILE_PATH)
        inject_errors(TEST_FILE_PATH , num_errors)
        run_binary(binary_path)
        os.remove(TEST_FILE_PATH)
        os.remove(TEST_FILE_PATH.rstrip('caff')+'webp')

def run_binary(binary_path: str) -> None:
    result = subprocess.run([binary_path, "-caff", TEST_FILE_PATH])
    if result.returncode == 0:
        print("Parser did not fail")
    else:
        print("Parser failed to parse")


def main() -> None:
    parser = argparse.ArgumentParser(description="CAFF Testing Script")
    parser.add_argument(
            "--parser",
            type=str,
            help="Path to the parser binary",
            default="../parser"
            )
    parser.add_argument(
            "--file",
            type=str,
            help="Path to the valid 1.caff file",
            default="../caff_files/1.caff"
            )
    parser.add_argument(
            "--rounds",
            type=int,
            help="Number of testing rounds",
            default=1
            )
    parser.add_argument(
            "--errors",
            type=int,
            help="Number of errors to inject in each round",
            default=1
            )
    args = parser.parse_args()
    perform_testing(args.parser, args.file, args.rounds, args.errors)

if __name__ == "__main__":
    main()
