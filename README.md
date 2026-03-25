# csvf
CSVF is a file format similar to CSV that you can view in a terminal.

[view specification](https://github.com/matthew-james-brewer/csvf/blob/master/csvf-0.0.1.md)

![screenshot](https://github.com/matthew-james-brewer/csvf/blob/master/screenshot.png?raw=true)

## using the tools

Before you can use the tools, you need to download the release for your OS and architecture: [Latest Release](https://github.com/matthew-james-brewer/csvf/releases/latest). Then you can skip to the 'after C install' section.

If you want to compile from source or use the js/java editions, you must clone in and switch to the created directory (`git clone https://github.com/matthew-james-brewer/csvf.git; cd csvf`).

### c

If you want to compile the C tools, you must go into the c directory (`cd c`) and run cmake (`mkdir build; cmake -B build; cmake --build build; cmake --install build`). It creates 5 files:

 * lib/libcsvf.(so|dll|dylib)
 * include/libcsvf.h
 * bin/csv2csvf(.exe)
 * bin/dispcsvf(.exe)
 * bin/csview

#### after C install

You can now convert csv files to csvf files (`csv2csvf x.csv x.csvf`), display csvf files (`dispcsvf x.csvf`), and jump straight to viewing csv files (`csview x.csv`).
You can also create C programs that use libcsvf. See the *-cmd.c files for demonstration of use. Compile with `-lcsvf` and put `#include <libcsvf.h>` at the top of your file.

### java

To use the Java tools, you must go into the java directory (`cd java`) and run `javac DispTable.java`, which generates DispTable.class.

Then, you can display csvf files (`java DispTable x.csvf` in the same directory as DispTable.class).

### js

To use the Javascript tools, you must create something that uses them. Some examples are provided in the `examples` folder.

If you're on iOS, you can install `js/csvf1.0.0.sbundle` using [Sunbundler](https://github.com/sbundle/sbundle-repo). Then, in ShortTerm, you can run `dispcsvf.sh x.csvf`.

## demo output

```
┌──────────┬──────┬─────────────────────────┐
│expression│result│note                     │
╞══════════╪══════╪═════════════════════════╡
│5+7       │12    │it's simple math         │
├──────────┼──────┼─────────────────────────┤
│41*62     │2542  │multiplication isn't hard│
├──────────┼──────┼─────────────────────────┤
│6480/60   │108   │just use long division   │
└──────────┴──────┴─────────────────────────┘
```
