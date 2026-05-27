# IP Routing Table Lookup (C)

A low-level C implementation of an IP routing table lookup algorithm. This project simulates how network switches and routers efficiently match incoming packet IP addresses against a routing table using the **Longest Prefix Match (LPM)** algorithm to determine the correct output interface.

## ⚙️ How It Works

The program reads a text file containing routing table entries (IP prefixes and their corresponding output interfaces) and builds an internal data structure in memory. When a batch of test IP addresses is fed into the system, it traverses the memory structure to find the longest matching prefix and routes the packet accordingly.

This demonstrates fundamental low-level networking concepts and memory management in C.

## 🛠️ Tech Stack

- **Language:** C
- **Concepts:** Longest Prefix Match (LPM), IP Routing, Networking, Memory Management
- **Build System:** Makefile

## 🚀 How to Run

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
