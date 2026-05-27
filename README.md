# IP Routing Lookup (C / Networking)

A low-level networking project that implements a highly efficient Longest Prefix Match (LPM) algorithm for IP routing tables using a Trie data structure.

## 🎯 Objective
In core internet routers, IP lookups must happen in nanoseconds. The objective of this project was to replace slow, linear routing table searches with an optimized data structure capable of performing Longest Prefix Match (LPM) lookups extremely fast, operating close to the hardware level in C.

## 🚀 What is Achieved
- **Trie Data Structure:** Implemented a binary Trie (prefix tree) from scratch in C to store network prefixes and their corresponding next-hop output ports.
- **Memory Management:** Carefully managed dynamic memory allocation (`malloc`/`free`) for tree nodes to prevent memory leaks during routing table updates.
- **Path Compression (Optional):** Optimized the basic Trie into a compressed structure to reduce depth and speed up memory access times.
- **Performance Benchmarking:** Verified that the custom Trie implementation drastically outperforms naive linear search methods when processing thousands of simulated IP packets.

## 🛠️ Tools & Technologies
- **Language:** C
- **Domain:** Telecommunications, Network Routing, Low-Level Algorithms
- **Concepts:** Longest Prefix Match (LPM), Binary Tries, Dynamic Memory Allocation, Bitwise Operations.

## 📖 Usage Guide

### Prerequisites
You need a C compiler (like `gcc`) and a `make` utility installed on your system (Linux/macOS recommended, or MinGW/WSL on Windows).

### Compilation and Execution
1. Clone the repository.
2. Navigate to the source folder and compile the project using the provided Makefile:
   ```bash
   make
   ```
3. Run the compiled executable, typically passing a routing table file and a file containing IP addresses to look up:
   ```bash
   ./my_route_lookup routing_table.txt ip_packets.txt
   ```
*(Refer to the source code comments for the exact expected arguments and table formats).*
