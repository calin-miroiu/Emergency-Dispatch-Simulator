# Emergency Dispatch System Simulator

## What it does
Implements a simulation of an emergency call dispatch system utilizing custom C data structures including doubly linked lists, priority queues, and stacks. The program reads operational commands to manage incidents, dispatch available intervention units, and track intervention histories. 

The core architecture features:
- **Modular design:** Separated into `main.c` for I/O parsing, `dispatch_system.c` for function implementations, and `dispatch_system.h` for definitions.
- **System structure:** Manages an array of intervention units alongside sentinel-based doubly linked lists for active incidents and interventions.
- **Queuing logic:** Distinct priority queues (high, medium, low) for incident triage and a standard queue for available unit allocation.
- **Memory safety:** Explicit memory management routines iteratively traverse and free all dynamically allocated nodes, sentinels, strings, arrays, and structural wrappers to prevent memory leaks upon termination.

## Core Operations & Logic
- **`ADD_INCIDENT`**: Parses input data (handling string manipulation via `strtok` and `memmove`), allocates a new incident node, and appends it to the system list and the appropriate priority queue.
- **`DISPATCH`**: Verifies unit availability and queue states. Extracts the incident from the highest-priority queue, dequeues an available unit, links them into an intervention record, and pushes this record onto the history stack.
- **`UNDO_LAST_DISPATCH`**: Pops records from the history stack until an active intervention is found. Restores the unit's availability and uses a custom `enqueue_front` function to return the incident to the very front of its original priority queue, ensuring it gets dispatched next.
- **`SOLVED_INCIDENT`**: Traverses the incident list to update statuses to "solved" and reclaims the assigned unit, placing it back into the available unit queue.

## Usage
A `Makefile` is provided for automated compilation.

To compile the project with `-Wall`, `-Wextra`, and `-g` flags:
```bash
make build
```

Execute the binary:
```
./dispatch_sim
```
Note: The program reads input operations (e.g., ADD_INCIDENT, DISPATCH, SOLVED_INCIDENT) from a local file named dispatch.in and writes execution results to dispatch.out.

To remove the compiled binary and output files:
```
make clean
```

## Requirements:
  -GCC (GNU Compiler Collection)
  
  -GNU Make
