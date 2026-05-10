# Custom Network Monitor

A high-performance user-space network monitoring tool written in C using Linux raw sockets.

This project captures live Ethernet frames, parses IPv4/TCP/UDP packets, tracks bidirectional flows using a custom hash table implementation, and aggregates real-time traffic statistics.

---

# Features

- Raw packet capture using Linux AF_PACKET sockets
- Ethernet frame parsing
- IPv4 packet parsing
- TCP and UDP header parsing
- Real-time flow aggregation
- Bidirectional flow normalization
- Custom hash table with separate chaining
- Periodic telemetry output
- Byte and packet statistics per flow
- Modular system-level architecture

---

# Architecture

```text
[Packet Source]
       ↓
[Capture Layer]
       ↓
[Parser Layer]
       ↓
[Flow Normalization]
       ↓
[Hash-Based Flow Tracker]
       ↓
[Aggregator]
       ↓
[Telemetry Output]
```

---

# Technologies Used

- C
- Linux Raw Sockets
- POSIX Networking APIs
- IPv4/TCP/UDP Protocol Parsing
- Hash Tables
- Linux Packet Interfaces

---

# Why This Project?

This project was built to deepen understanding of:

- low-level Linux networking
- packet parsing internals
- telemetry pipelines
- flow aggregation systems
- memory-efficient data structures
- systems programming architecture

The design intentionally avoids external packet-capture frameworks in order to work directly with Linux networking primitives.

---

# Flow Tracking

Each flow is identified using the 5-tuple:

```c
(src_ip, src_port, dst_ip, dst_port, protocol)
```

Bidirectional traffic is normalized into a canonical flow representation to prevent duplicate flow entries.

---

# Build

```bash
gcc *.c -o "monitor_name"
```

---

# Run

Raw sockets require elevated privileges.

Option 1:
```bash
sudo ./monitor
```

Option 2 (recommended):
```bash
sudo setcap cap_net_raw+ep ./monitor
./monitor
```

---

# Example Output

```text
==== Flow Table ====

192.168.1.10:50000 → 142.250.x.x:443 | TCP | Bytes: 12000 | Packets: 30
192.168.1.10:5353  → 224.0.0.251:5353 | UDP | Bytes: 800 | Packets: 10
```

---

# Future Improvements

- Top talkers sorting
- CSV export
- CLI protocol filters
- IPv6 support
- Flow expiration/LRU eviction
- Multi-threaded packet processing
- ncurses-based dashboard
- PCAP file export

---

# Learning Outcomes

This project demonstrates:

- systems programming
- network protocol parsing
- Linux internals
- custom data structures
- real-time telemetry aggregation
- memory management
- performance-oriented software design

---

# License

MIT License