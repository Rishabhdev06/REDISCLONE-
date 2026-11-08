
# High-Performance In-Memory Datastore (C++ Redis Clone)

A lightweight, high-performance in-memory database built entirely from scratch in **C++14**. This project implements a custom non-blocking TCP server, a proprietary binary serialization protocol, and advanced data structures to achieve production-grade throughput with minimal latency.

---

## Performance Benchmark

Achieved **57,600+ Operations Per Second (OPS)** for concurrent `SET`/`GET` operations on a **single-threaded architecture** running on **Apple Silicon (bare-metal)**.

> **Note:** Containerized deployment on macOS introduces a network proxy layer that slightly impacts throughput. Bare-metal execution reflects the true performance of the underlying C++ event-driven architecture.

---

## Features

- High-performance in-memory key-value datastore
- Non-blocking TCP server using `poll()`
- Custom binary serialization protocol
- Custom hash table with **O(1)** progressive rehashing
- AVL Tree implementation for Sorted Sets (`ZSET`)
- TTL support using a Min-Heap
- Asynchronous memory cleanup using a background thread pool
- Optimized with `-O3` compiler optimizations
- Docker support for instant deployment

---

# Quick Start (Docker)

Run the datastore instantly without installing a C++ compiler.

```bash
docker run -d -p 6379:6379 yashgupta/c-redis-datastore:v1.0
```

The server will start listening on:

```
localhost:6379
```

---

# System Architecture

The datastore is designed from scratch with performance as the primary objective, avoiding high-level abstractions wherever possible.

## Non-blocking Event Loop

- Built using POSIX `poll()`
- Handles thousands of concurrent TCP connections
- Eliminates expensive OS thread context switching
- Single-threaded networking architecture

---

## Custom Hash Table

Implemented entirely from scratch using chaining.

### Progressive Rehashing

Instead of rehashing the entire table at once (which causes latency spikes), the server migrates a fixed number of buckets (e.g., **128**) during each event loop iteration.

Benefits:

- Constant latency
- No server freezing
- O(1) amortized resizing

---

## AVL Trees (Sorted Sets)

Sorted Sets (`ZSET`) are backed by a custom AVL Tree implementation.

Provides:

- O(log N) insertion
- O(log N) deletion
- O(log N) lookup
- Efficient range queries

---

## Min Heap (TTL Management)

Expiration times are maintained using a custom Min-Heap.

Advantages:

- O(1) access to the next key to expire
- Efficient TTL cleanup
- Low memory overhead

---

## Asynchronous Deallocation

Destroying large AVL trees can block the main event loop.

To avoid latency spikes, a dedicated thread pool performs background memory cleanup.

Benefits:

- Non-blocking deletes
- Smooth request latency
- Faster server responsiveness

---

# 🛠️ Build From Source

## Prerequisites

- C++14 compatible compiler (`clang` or `g++`)
- macOS or Linux
- POSIX-compatible environment

---

## Clone Repository

```bash
git clone https://github.com/yashgupta/c-redis-datastore.git

cd c-redis-datastore/redis/14
```

---

## Compile

```bash
clang++ -O3 -std=c++14 -pthread \
14_server.cpp \
hashtable.cpp \
zset.cpp \
avl.cpp \
heap.cpp \
thread_pool.cpp \
-o server
```

---

## Run

```bash
./server
```

The datastore will start listening on port **6379**.

---

# Benchmarking

Since this project uses a **custom binary protocol**, standard tools like `redis-benchmark` cannot be used.

A dedicated benchmark tool is included.

## Compile

```bash
clang++ -O3 -std=c++14 benchmark.cpp -o custom_bench
```

## Run

```bash
./custom_bench
```

Make sure the server is already running before executing the benchmark.

---

# Benchmark Results

| Metric | Value |
|----------|-------|
| Architecture | Single-threaded |
| Protocol | Custom Binary |
| Event Loop | `poll()` |
| Operations | Concurrent `SET` / `GET` |
| Throughput | **57,600+ OPS** |
| Platform | Apple Silicon (Bare Metal) |

---

# 🏗️ Project Structure

```
redis/
└── 14/
    ├── 14_server.cpp
    ├── benchmark.cpp
    ├── hashtable.cpp
    ├── heap.cpp
    ├── avl.cpp
    ├── zset.cpp
    ├── thread_pool.cpp
    └── ...
```

---

# Future Improvements

- RESP protocol compatibility
- Persistence (AOF/RDB)
- Replication
- Multi-threaded networking
- Cluster mode
- Pub/Sub support
- Transactions
- Lua scripting
- LRU/LFU eviction policies

---

# 📄 License

This project is released under the **MIT License**.

---

# 👨‍💻 Author

** Vivaan Jain and Yash Gupta**

````
