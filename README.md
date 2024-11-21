# C Data Structures

## Contents

This library implements the following essential data structures with basic operations:
- **String**: char array implementation
- **List**: dynamic array implementation
- **Dict**: hash-table implementation
- **Set**: hash-table implementation
- **Heap**: Fibonnaci heap implementation

List, Dict, Set, and Heap also contain a wrapper data structure for concurrency support using the `<pthreads.h>` library.

## How to build

The following instructions were prepared for [OpenBSD 7.3](https://www.openbsd.org/73.html).
```
$ cd data_structures/
$ make
```

This will yield a `data_structures.a` artifact you can include in your project.

## How to test

After building the library, run tests under `test/`.

## How to use

`example_app/` contains an example program that uses the data structures library that you can try out:
```
$ cd example_app/
$ make
$ ./example_app
list: [1,2,3,4,5]
```
