# XoneKey WSL Build & Test Guide

This guide describes how to build and test the XoneKey core engine on Ubuntu WSL (Windows Subsystem for Linux).

## Prerequisites

Ensure you have the following packages installed on your Ubuntu WSL:

```bash
sudo apt update
sudo apt install build-essential g++ make
```

## Build Instructions

1.  Open your WSL terminal.
2.  Navigate to the project directory.
3.  Go to the engine directory:
    ```bash
    cd Sources/XoneKey/engine
    ```
4.  Build the test application:
    ```bash
    make
    ```

## Running the Test

Run the generated test executable:

```bash
./test_engine
```

The test will verify:
- Engine initialization
- Character code lookups
- Vietnamese typing logic (Telex/VNI)
- Engine safety features (bounds checking)

## Project Structure for Linux

-   `Engine.cpp / Engine.h`: Core engine logic.
-   `Vietnamese.cpp / Vietnamese.h`: Vietnamese typing rules.
-   `DataType.h`: Cross-platform data types and platform selection.
-   `platforms/linux.h`: Linux-specific key codes.
-   `EngineSafety.h`: Bounds checking and safety layer.
