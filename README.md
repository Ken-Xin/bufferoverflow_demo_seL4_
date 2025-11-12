# seL4 Microkernel Security Demonstration Project

A comprehensive demonstration of the seL4 microkernel's security features, including IPC communication, component isolation, fault tolerance, and buffer overflow protection with capability-based isolation.

##  Table of Contents

- [Project Overview](#project-overview)
- [Key Features](#key-features)
- [System Requirements](#system-requirements)
- [Installation](#installation)
- [Project Tasks](#project-tasks)
- [Building and Running](#building-and-running)
- [Project Structure](#project-structure)
- [Key Concepts](#key-concepts)
- [Comparison with Linux](#comparison-with-linux)
- [Troubleshooting](#troubleshooting)

## Project Overview

This project implements a PC/Mac-based seL4 microkernel system using Microkit on ARM64 architecture via QEMU (and optionally on Raspberry Pi 4). The system demonstrates:

- Inter-Process Communication (IPC) via endpoints and shared memory
- Capability-based isolation (VSpace and CSpace)
- Fault tolerance and containment
- Buffer overflow protection through mathematical proof-based isolation
- Performance comparison with monolithic Linux architecture

##  Key Features

### Task 2: IPC Client-Server Implementation
- Endpoint-based communication between client and server
- Shared memory (page mapping) demonstration
- Message passing with badges and reply objects

### Task 3: Component Isolation with Logger
- **VSpace Isolation**: Each Protection Domain has isolated virtual address space
- **CSpace Isolation**: Capability-based access control
- **Capability Mapping**: Least-privilege enforcement
- Third component (logger) with controlled access

### Task 4: Fault Tolerance Demonstration
- Intentional component crash demonstration
- Fault containment verification
- System integrity maintenance during failures
- Fault handler implementation

### Task 5: Buffer Overflow Security Analysis
- Buffer overflow attacks with traditional security disabled
- Comparison between seL4 and conventional OS protections
- Demonstration of seL4's mathematical proof-based security
- ASLR, stack canaries, and DEP disabled to show seL4's fundamental protection

##  System Requirements

### Hardware
- Mac (M1 Apple Silicon or Intel)
- 4GB+ RAM recommended
- (Optional) Raspberry Pi 4 for hardware deployment

### Software
- macOS 11.0 or later
- Homebrew package manager
- QEMU (ARM64 support)
- AArch64 cross-compiler toolchain
- seL4 Microkit SDK v2.0.1

##  Installation

### 1. Install Dependencies

Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

Install QEMU
brew install qemu

Install AArch64 toolchain
brew install aarch64-elf-gcc

Or alternatively:
brew tap messense/macos-cross-toolchains
brew install aarch64-unknown-linux-gnu


### 2. Download Microkit SDK

Create project directory
mkdir ~/seL4_project
cd ~/seL4_project

Download Microkit SDK (macOS ARM64 version for M1/M2/M3 Macs)
curl -L https://github.com/seL4/microkit/releases/download/2.0.1/microkit-sdk-2.0.1-macos-aarch64.tar.gz -o sdk.tar.gz

For Intel Macs, use:
curl -L https://github.com/seL4/microkit/releases/download/2.0.1/microkit-sdk-2.0.1-macos-x86-64.tar.gz -o sdk.tar.gz
Extract
tar xf sdk.tar.gz
mv microkit-sdk-2.0.1 microkit-sdk


### 3. Clone/Copy Project Files

Place all project files in the `~/seL4_project` directory:
- `Makefile`
- `util.h`
- `*.c` files (server.c, client.c, logger.c, etc.)
- `*.xml` files (system.xml, system_with_logger.xml, etc.)

##  Project Tasks

### Task 2: IPC Client-Server Implementation

make clean
make task2
make run


**Expected Output**: Client and server exchange messages through shared memory, demonstrating IPC functionality.

### Task 3: Component Isolation with Logger

make clean
make task3
make run


**Expected Output**: Three isolated components (client, server, logger) operate independently, demonstrating VSpace and CSpace isolation.

### Task 4: Fault Tolerance

make clean
make task4
make run


**Expected Output**: One component crashes intentionally, but others continue operating normally.

### Task 5: Buffer Overflow Security

make clean
make task5
make run


**Expected Output**: Buffer overflow occurs in vulnerable component, but isolation prevents propagation to other components.

### Exit QEMU

Press `Ctrl+A`, then press `X`

## Project Structure

seL4_project/
├── Makefile # Build automation
├── util.h # String/memory utility functions
│
├── server.c # Server component
├── client.c # Client component
├── logger.c # Logger component
├── fault_handler.c # Fault handling component
├── fault_test.c # Fault test component
├── safe_server.c # Secure server component
├── vulnerable_pd.c # Vulnerable component (for Task 5)
├── attacker.c # Attacker simulator (for Task 5)
│
├── system.xml # Task 2 system description
├── system_with_logger.xml # Task 3 system description
├── system_fault.xml # Task 4 system description
├── system_buffer_overflow.xml # Task 5 system description
│
├── microkit-sdk/ # Microkit SDK
└── README.md # This file


##  Key Concepts

### VSpace Isolation
Each Protection Domain has its own isolated virtual address space. Even if a component knows the virtual address of another component's memory, it cannot access it without explicit capability mapping.

### CSpace Isolation
Each Protection Domain has its own capability space, containing only the capabilities explicitly granted in the system description XML file.

### Capability Mapping
Access rights are controlled through capabilities, which are unforgeable tokens that grant specific permissions to specific resources.

### Mathematical Proof
seL4 is the first operating system kernel with a complete formal verification, mathematically proving that buffer overflows are impossible in the kernel.


##  Troubleshooting

### Build Errors

Check if SDK is properly installed
make check-sdk

Clean and rebuild
make clean
make task2


### QEMU Issues

Verify QEMU installation
qemu-system-aarch64 --version

Check if QEMU can access ARM CPU
qemu-system-aarch64 -cpu help | grep cortex-a53


### Linker Errors

Ensure the linker script is correctly specified in the Makefile:
LINKER_SCRIPT := $(MICROKIT_SDK)/board/$(BOARD)/$(CONFIG)/lib/microkit.ld


##  Additional Resources

- [seL4 Official Documentation](https://docs.sel4.systems/)
- [Microkit User Manual](https://docs.sel4.systems/projects/microkit/)
- [seL4 Formal Verification](https://sel4.systems/Info/FAQ/proof.pml)