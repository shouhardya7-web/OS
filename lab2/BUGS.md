# BUGS FOUND

## 1. Off-by-One Bug

File: buggy_sum.c

Bug:
The loop used <= instead of <, causing access outside array bounds.

How Found:
Used GDB with breakpoints and print commands.

Fix:
Changed:
i <= n

To:
i < n


## 2. Memory Leak

File: leak.c

Bug:
malloc() allocated memory but free() was missing.

How Found:
Used valgrind --leak-check=full

Fix:
Added:
free(buf);


## 3. Use-After-Free Bug

File: uaf.c

Bug:
Program accessed pointer after free().

How Found:
Used valgrind which reported invalid read.

Fix:
Moved free(p) after printf() and set pointer to NULL.
