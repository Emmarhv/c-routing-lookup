# IP Routing Table Lookup (C)

A low-level C implementation of an IP routing table lookup algorithm. This project simulates how network switches and routers efficiently match incoming packet IP addresses against a routing table using the **Longest Prefix Match (LPM)** algorithm to determine the correct output interface.

## How It Works

The program reads a text file containing routing table entries (IP prefixes and their corresponding output interfaces) and builds an internal **Trie (Prefix Tree)** data structure in memory. 

1. **Tree Construction:** Each bit of the IP prefix dictates the left/right traversal in the tree, allowing for highly efficient O(N) storage and lookup (where N is the 32-bit length of an IPv4 address).
2. **Tree Compression:** The algorithm includes a compression phase that eliminates redundant nodes to optimize memory usage.
3. **Lookup Phase:** When a batch of test IP addresses is fed into the system, it traverses the compressed Trie bit-by-bit to find the longest matching prefix and routes the packet to its corresponding output interface.
4. **Performance Profiling:** The code uses `clock_gettime` to precisely measure search times and node access counts, providing statistics on the efficiency of the LPM algorithm.

This project demonstrates strong capabilities in low-level networking concepts, dynamic memory management, pointers, and data structure optimization in C.

## Tech Stack
- **Language:** C (Standard C99)
- **Concepts:** Longest Prefix Match (LPM), Prefix Trees (Tries), Memory Compression, IP Routing
- **Build System:** Makefile

## How to Run

1. Open your terminal in the root directory.
2. Compile the code using the provided `Makefile`:
   ```bash
   make all
   ```
3. Run the executable with a routing table and a set of test IP addresses:
   ```bash
   ./my_route_lookup routing_table.txt prueba1.txt
   ```
4. To clean the compiled files:
   ```bash
   make clean
   ```
