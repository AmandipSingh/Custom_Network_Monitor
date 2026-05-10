# Custom Network Monitor Architecture

## Overview

The Custom Network Monitor is a user-space telemetry engine built using Linux raw sockets.

The system captures Ethernet frames directly from the network interface, parses protocol headers, aggregates bidirectional flows, and generates real-time traffic statistics.

---

# Pipeline

```text
[Raw Socket Capture]
          ↓
[Ethernet Parsing]
          ↓
[IPv4 Parsing]
          ↓
[TCP/UDP Parsing]
          ↓
[Flow Normalization]
          ↓
[Hash-Based Aggregation]
          ↓
[Telemetry Output]
```

---

# Core Components

## Capture Layer
Responsible for:
- raw socket creation
- packet ingestion
- frame reception

Files:
- capture.c
- capture.h

---

## Parser Layer
Responsible for:
- Ethernet parsing
- IPv4 parsing
- TCP/UDP extraction

Files:
- parser.c
- parser.h

---

## Flow Tracker
Responsible for:
- flow hashing
- bidirectional normalization
- aggregation
- statistics tracking

Files:
- flow_tracker.c
- flow_tracker.h

---

## Utility Layer
Responsible for:
- protocol formatting
- IP formatting

Files:
- utils.c
- utils.h

---

# Flow Definition

A network flow is identified using:

```c
(src_ip, src_port, dst_ip, dst_port, protocol)
```

Bidirectional traffic is normalized into a canonical representation before aggregation.

---

# Data Structure Design

The system uses:
- hash table
- separate chaining
- linked-list collision handling

This provides efficient O(1) average flow lookup.

---

# Future Improvements

- CSV export
- top talker sorting
- IPv6 support
- ncurses dashboard
- multithreaded packet capture
- flow eviction/LRU cleanup
- eBPF integration