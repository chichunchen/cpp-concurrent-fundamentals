# Experiments
This directory now puts the dump from valgrind for checking if there is memory leak in `ds/` directory.
In `hp_msqueue_mem.txt` and `no_gc_msqueue_mem.txt`, you might notice that there are tons of `Conditional jump or move depends on uninitialised value(s)` 
message and it might because the `ptr` structure in msqueue is heavily manipulated in an atomic way. More details in this [link](https://stackoverflow.com/questions/2612447/pinpointing-conditional-jump-or-move-depends-on-uninitialized-values-valgrin).

