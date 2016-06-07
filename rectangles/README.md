Copyright (c) 2016 Gian Luigi Romita (romitagl@gmail.com)

## Introduction
This program parses an input JSON file containing rectangles coordinates and returns the overlaps.

* Sample input JSON file:
{
"rects": [
        {"x": 100, "y": 100, "w": 250, "h": 80 },
        {"x": 120, "y": 200, "w": 250, "h": 150 },
        {"x": 140, "y": 160, "w": 250, "h": 100 },
        {"x": 160, "y": 140, "w": 350, "h": 190 }
        ]
}

* Sample output:
Input:
1: Rectangle at (100,100), w=250, h=80.
2: Rectangle at (120,200), w=250, h=150.
3: Rectangle at (140,160), w=250, h=100.
4: Rectangle at (160,140), w=350, h=190.
Overlaps:
Between rectangle 1 and 3 at: (140,160), w=210, h=20.
Between rectangle 2 and 3 at: (140,200), w=230, h=60.
Between rectangle 1 and 4 at: (160,140), w=190, h=40.
Between rectangle 1 and 3 and 4 at: (160,160), w=190, h=20.
Between rectangle 3 and 4 at: (160,160), w=230, h=100.
Between rectangle 2 and 4 at: (160,200), w=210, h=130.
Between rectangle 2 and 3 and 4 at: (160,200), w=210, h=60.

## Compatibility:
The technologies used are only C++ (c++0x) with STL.
Some compiler-platform combinations which have been tested are shown as follows.
* GCC 4.4.7 20120313 - Red Hat 4.4.7-4 
* Apple LLVM version 7.3.0 (clang-703.0.29) - OS X 10.11

## Build
* Linux/Mac: just type 'make' to call the Makefile

## Run
* Linux/Mac: just run "./rect_parser json_input_files/rectangles.json"

## Build Test
* Linux/Mac: just type 'make test' to call the Makefile

## Run Test
- this test executable implements a first set of robustness and correcteness tests
- specific unit tests should be implemented using Cppunit or GoogleTest
* Linux/Mac: just run "./rect_parser_test"





