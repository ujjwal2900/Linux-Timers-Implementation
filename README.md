# Linux Timers Implementation

This project provides implementations and examples of various timer mechanisms used in Linux systems, including POSIX timers and timing wheel timers. It demonstrates how timers can be used for event scheduling, timeouts, and periodic tasks, with a practical example in a routing table context.

## Overview

Timers are fundamental components in operating systems like Linux for managing time-based events. They allow applications to schedule tasks to run at specific times or after certain intervals. This project explores different timer implementations:

1. **POSIX Timers**: Standard timer API using `timerfd` for user-space applications
2. **Timing Wheel Timers**: An efficient data structure for managing large numbers of timers
3. **Routing Table Example**: Demonstrates timer usage for entry expiration in a network routing table

## Components

### Core Timer Library (`timerlib.c`, `timerlib.h`)
- Wrapper around POSIX timers using `timerfd`
- Supports one-shot and periodic timers
- Features exponential backoff and state management
- Thread-safe timer operations

### Wheel Timer Implementation (`WheelTimers/`)
- Efficient timing wheel data structure
- O(1) timer registration and expiration
- Supports recurring and one-time events
- Thread-based timer processing

### Routing Table Example (`assignment/`)
- Demonstrates practical timer usage
- Implements routing table with automatic entry expiration
- Uses timers to clean up stale routes

## Usage of Timers in Linux

### General Purpose
Timers in Linux are used for:
- **Event Scheduling**: Delaying execution of tasks
- **Timeouts**: Preventing indefinite waits in I/O operations
- **Periodic Tasks**: Running maintenance or monitoring functions
- **Resource Management**: Cleaning up unused resources after timeouts

### Kernel-Level Timers
- Used by the scheduler for time slices
- Network stack timeouts (TCP retransmissions, connection timeouts)
- Device driver timeouts
- Filesystem operations

### User-Space Timers
- Application-level timeouts
- Periodic data collection
- GUI event handling
- Real-time applications

## Timing Wheel Timers

Timing wheels are an optimization for systems that need to manage thousands of timers efficiently. Instead of scanning all timers on each tick (O(n) complexity), timing wheels use a circular buffer divided into slots.

### How It Works
1. **Wheel Structure**: Fixed number of slots representing time intervals
2. **Timer Registration**: Timers are placed in slots based on their expiration time
3. **Clock Tick**: The wheel rotates, processing expired timers in O(1) time
4. **Efficiency**: Reduces timer management overhead for large-scale applications

### Advantages
- Constant-time operations for most timer operations
- Scales well with large numbers of timers
- Used in high-performance systems like network servers and databases

## Building and Running

### Prerequisites
- Linux system with POSIX timer support
- GCC compiler
- pthread library

### Compilation
```bash
# For wheel timer examples
cd WheelTimers
./compile.sh

# For routing table example
cd assignment
./compile.sh

# For basic timer examples
gcc -o timer_example timerExample.cpp timerlib.c -lpthread
```

### Running Examples
```bash
# Wheel timer test
./wheelTimerTest

# Timer example
./timer_example

# Routing table demo
./rt_entry_expiration
```


