
# ⚡ High-Performance In-Memory Key-Value Store

A Redis-inspired, high-performance in-memory database built from scratch using **C++14**.

Designed to demonstrate advanced Data Structures, Algorithms, Operating Systems, Computer Networks, and memory management through a compact database engine.

## 🚀 Key Features

- **Non-blocking TCP Server:** Single-threaded event loop using `poll()` to handle multiple client connections.
- **Custom Hash Table:** Average O(1) key-value operations with incremental rehashing.
- **AVL Tree:** Self-balancing AVL tree powering sorted-set operations.
- **TTL Management:** Min-heap-based key expiration with lazy and active expiration.
- **Memory Reclamation:** Background thread pool for deferred destruction of large sorted sets.
- **Custom Binary Protocol:** Length-prefixed request/response protocol with pipelining support.
- **Concurrent Connections:** Handles multiple connected clients without blocking on individual socket operations.

## 🏗️ System Architecture

```text
             CLIENT APPLICATIONS
                      |
                      | TCP
                      v
             NON-BLOCKING SERVER
                      |
                      v
                EVENT LOOP
                 (poll)
                      |
                      v
              COMMAND PARSER
                      |
                      v
             COMMAND EXECUTION
                      |
          +-----------+-----------+
          |           |           |
          v           v           v
      HASH TABLE   AVL TREE    TTL HEAP
          |           |           |
          v           v           v
      KEY-VALUE    SORTED SET   EXPIRATION
       STORAGE      STORAGE     MANAGER
          |
          v
    MEMORY RECLAIMER
     (Worker Pool)
```

## 🧠 Core Data Structures & Complexity

| Component | Data Structure | Complexity |
|---|---|---|
| Key-value storage | Custom Hash Table | Average O(1) |
| Hash resizing | Incremental Rehashing | Amortized O(1) per migrated entry |
| Sorted sets | AVL Tree | O(log N) |
| Rank queries | Subtree Size Augmentation | O(log N) |
| TTL scheduling | Min-Heap | O(log N) insertion |
| Expiration lookup | Min-Heap | O(1) peek |
| Memory cleanup | Background Worker Pool | Deferred processing |

### 1. Incremental Rehashing

Traditional hash-table resizing can cause noticeable pauses when a large number of entries must be migrated.

This project implements incremental rehashing:

- Maintains old and new hash tables during resizing.
- Migrates a bounded amount of work per database operation.
- Allows lookups and updates while migration is in progress.
- Avoids a single large resize operation on the request path.

### 2. AVL-Tree-Based Sorted Sets

Each AVL node stores:

- Member name
- Numeric score
- Node height
- Subtree size
- Left and right child pointers

Supported operations include insertion, deletion, score lookup, and rank-based range queries.

### 3. TTL-Based Expiration

A min-heap schedules key expiration.

- `PEXPIRE` assigns an expiration time.
- `PTTL` returns remaining lifetime.
- `PERSIST` removes expiration.
- Expired keys are removed through lazy and active expiration.

### 4. Deferred Memory Reclamation

Large detached sorted sets can be scheduled for background destruction.

This reduces expensive object destruction on the main request-processing path.

## 📋 Supported Commands

The database currently supports 16 commands.

### Key-Value Operations

| Command | Description |
|---|---|
| `PING` | Check server connectivity |
| `SET key value` | Insert or update a key |
| `GET key` | Retrieve a value |
| `DEL key` | Delete a key |
| `EXISTS key` | Check key existence |
| `DBSIZE` | Return the number of keys |
| `INCR key` | Increment an integer value |
| `KEYS` | Return all keys |

### TTL Operations

| Command | Description |
|---|---|
| `PEXPIRE key milliseconds` | Set expiration |
| `PTTL key` | Get remaining TTL |
| `PERSIST key` | Remove expiration |

### Sorted Set Operations

| Command | Description |
|---|---|
| `ZADD key score member` | Add or update a member |
| `ZREM key member` | Remove a member |
| `ZSCORE key member` | Get member score |
| `ZCARD key` | Count sorted-set members |
| `ZQUERY key score member offset limit` | Query sorted-set elements |

## 💻 Tech Stack

- Language: C++14
- Networking: TCP Sockets
- I/O Multiplexing: `poll()`
- Data Structures: Hash Table, AVL Tree, Min-Heap
- Concurrency: POSIX Threads
- Build System: CMake / Make
- Testing: Python Integration Tests

## ⚙️ Installation & Setup

### Requirements

- macOS or Linux
- C++14-compatible compiler
- CMake 3.12+
- Python 3 (for integration tests)
- POSIX threads

No external Redis server or third-party C++ Redis client is required.

### Clone the Repository

```bash
git clone https://github.com/viv007-cyber/Project-Redis.git

cd Project-Redis
```

### Build Using CMake

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

cmake --build build -j
```

### Alternative: Build Using Make

```bash
make
```

## ▶️ Running the Server

Start the server on port 6379:

```bash
./build/kvstore_server 6379
```

If using Make:

```bash
./kvstore_server 6379
```

The server accepts an optional port argument. The default port is 6379.

## 🧪 Testing

Run the integration tests using CMake:

```bash
ctest --test-dir build --output-on-failure
```

Or using Make:

```bash
make test
```

The integration tests cover:

- Basic key-value operations
- Integer increments
- TTL expiration
- Sorted-set operations
- Multiple client connections
- Hash-table growth and rehashing

## 📊 Benchmarking

Start the server in Terminal 1:

```bash
./build/kvstore_server 6379
```

Run the benchmark in Terminal 2:

```bash
./build/kvstore_benchmark 6379
```

The benchmark executes 100,000 sequential SET requests over one TCP connection.

Record the following when reporting results:

- CPU and RAM configuration
- Compiler and compiler version
- Build configuration
- Number of requests
- Throughput in operations per second

Benchmark results are machine-dependent and should not be treated as a direct Redis comparison.

## 📁 Project Structure

```text
Project-Redis/
│
├── 14/
│   ├── 14_server.cpp
│   ├── hashtable.cpp
│   ├── hashtable.h
│   ├── avl.cpp
│   ├── avl.h
│   ├── zset.cpp
│   ├── zset.h
│   ├── heap.cpp
│   ├── heap.h
│   ├── thread_pool.cpp
│   ├── thread_pool.h
│   ├── common.h
│   ├── list.h
│   └── benchmark.cpp
│
├── tests/
│   └── integration_test.py
│
├── CMakeLists.txt
├── Makefile
├── Dockerfile
└── README.md
```

## 🎯 Engineering Concepts Demonstrated

This project demonstrates practical implementation of:

**Data Structures & Algorithms**
- Custom hash tables
- Incremental rehashing
- AVL rotations and balancing
- Min-heaps
- Rank-based tree traversal

**Operating Systems**
- Non-blocking I/O
- Event-driven architecture
- POSIX threads
- Deferred memory reclamation
- Resource management

**Computer Networks**
- TCP client-server communication
- Socket programming
- Binary serialization
- Request pipelining
- Multiple client connections

**System Design**
- In-memory storage architecture
- Command execution engine
- TTL scheduling
- Background resource cleanup

## ⚠️ Current Limitations

This is an educational systems-programming project, not a production replacement for Redis.

- No AOF/RDB persistence or crash recovery.
- No replication or clustering.
- No authentication or TLS.
- Custom protocol; not RESP-compatible.
- Command execution is single-threaded.
- `KEYS` scans the complete keyspace.
- Benchmark currently uses one sequential client connection.

## 🔮 Future Improvements

- RESP protocol compatibility.
- Snapshot-based persistence.
- Append-only file logging.
- LRU/LFU eviction policies.
- Improved multi-client load testing.
- Latency measurement (p50, p95, p99).
- Memory profiling and allocator optimization.

## 🎓 Learning Outcomes

The primary objective is to understand how high-performance database systems work internally by implementing their core mechanisms rather than relying on existing database libraries.

**Built with C++14 | Data Structures | Algorithms | Operating Systems | Computer Networks**

