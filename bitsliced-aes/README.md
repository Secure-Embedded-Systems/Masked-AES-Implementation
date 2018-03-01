Bitsliced AES
-------------

Bitslicing is a technique to compute steps in an algorithm 1 bit at a time.  Each bit in a processor word would be a part
of a different data stream for that particular algorithm.  It is attractive because then it can run many different streams 
in parallel (depending on the word length).  E.g. a 32 bit word length can compute 32 different streams in parallel.

Some encryption algorithms allow blocks to be operated on in parallel like AES CTR.  So if there is enough input, say 32 blocks,
then a 32 bit processor can achieve full utilitization by filling all bits in word and enciphering all blocks in parallel.


Research
--------


Bitslicing is not a new idea.  It was first done on DES by Eli Biham in 1997 as described by his paper "A Fast New DES Implementation in Software".
Since then many implementations have been done on AES and other encryption algorithms.  However none are either pubicly available or easily portable.


This bitsliced implementation
-----------------------------

This work exists for education and research.  This repository exists as a reference for people working on bitslicing.
It is written entirely in C.

Performance measurements done for AES-CTR on a 64 bit 4 GHz Intel 4790 and compiled with GCC 4.8.4.

|                      |footprint | throughput |
|----------------------|----------|------------|
|Performance optimized |  12,150 bytes  | 51 cycles/byte |
|Footprint optimized   | 8,526 bytes | 81 cycles/byte |

Performance could be improved by about 5-10x by writting in assembly and ensuring
more operations stay in registers rather then spill to memory.


Compiling
----------

Compile the benchmarking program by running:

```bash
make
```

Benchmark program requires OpenSSL.


Compile the test program by running:

```bash
make test
```

Change to the word length of your processor by editing the `WORD_SIZE` macro in bs.h.  Optimize for
footprint by using `-O2` instead of `-O3` in the Makefile and also deleting the `-DUNROLL_TRANSPOSE` 
flag.


