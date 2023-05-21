# CAFF parser

*CIFF and CAFF files to webp converter*

The parser can be run and compiled with the helper script:
```bash
./run.sh [parser arguments]
```

## Compilation

You need to have a version of g++ installed that is capable
of compiling to CXX17.

From the location of this README.md file run:
```bash
make
```
This will create a binary named "parser" at this location.

To clean up the compiled binary as well as other artifacts, run:

```bash
make clean
```

## Usage

The converter validates the input file and if all checks pass, a webp image 
is created with the same name at the same location as the input file.

To run the binary:
```bash
./parser <-caff|-ciff> [path-to-file]
```

## Testing

As part of secure software development, a minimal fuzzer was implemented in python.
To run it you'll need python and the packages in requirements.txt installed.
This can be done by running pip (in the test directory):

```bash
pip install -r requirements.txt
```

To try it out, simply run the following command from the test directory:

```bash
python fuzzer.py
```

This wil run the program with the default arguments, the number of rounds and the
number of errors injected to the file are configurable via the cli interface.

To get familiar with the cli interface, run:

```
python fuzzer.py -h
```

During execution "1.caff" file is copied, errors are injected to the copy, the errors
and their locations are logged and the parser is invoked. After the parser's
execution has finished, its' return value is logged and the behavor's correctness
can be verified manually.

**During development this was done 20x to ensure a secure parser is created**
