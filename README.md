*This project has been created as part of the 42 curriculum by daogawa.*

# Codexion (dining-coders)

## Description

**Codexion** is a concurrency project written in C with POSIX threads. It is a
variation of the classic *Dining Philosophers* problem.

`N` coders sit around a circular table. Between each pair of neighbouring coders
lies one USB **dongle** (so there are `N` dongles). To **compile**, a coder must
hold both the dongle on their left and the one on their right. Each coder loops
through:

1. take two dongles → **compile**
2. release the dongles → **debug**
3. **refactor**
4. back to step 1

A coder who has not *started* a compile within `time_to_burnout` milliseconds
**burns out**, and the simulation stops. The simulation also stops when every
coder has compiled at least `number_of_compiles_required` times.

The goal is to share a limited resource between threads **without data races,
deadlocks, or starvation**, using only mutexes and condition variables.

### Overview of the implementation

- **One thread per coder** plus **one monitor thread**. The monitor checks every
  ~1 ms whether a coder has burned out or whether everyone has compiled enough.
- **Dongles** are not mutexes. Their state (`in_use`, `available_at`) is protected
  by a single `table_lock`. Coders wait on their own `pthread_cond_t` and are woken
  by neighbours when dongles are released.
- **Dongle cooldown**: after release, a dongle cannot be taken again until
  `dongle_cooldown` ms have passed. Waiting coders use `pthread_cond_timedwait`
  until the earliest cooldown ends.
- **Fair scheduling with a priority queue (min-heap)**: every coder asking for
  dongles is pushed into a shared wait heap. A coder may take its dongles only if
  no neighbour competing for the same dongle has a higher priority. Two policies
  are available:
  - `fifo` — first come, first served (key = arrival order)
  - `edf` — *Earliest Deadline First* (key = `last_compile_start + time_to_burnout`),
    so the coder closest to burning out goes first
- **Logging** goes through a `print_lock` so log lines never mix together.

### Project structure

```
.
├── Makefile
├── includes/codexion.h   # structs, enums, prototypes
└── src/
    ├── main.c            # entry point, argument count check
    ├── parse.c           # argument validation / parameter init
    ├── simulate.c        # thread creation and join
    ├── coders.c          # coder initialisation / destruction
    ├── dongle.c          # dongle initialisation
    ├── coder_routine.c   # coder loop: take dongles, compile, debug, refactor
    ├── heap.c            # min-heap used as the wait queue (FIFO / EDF)
    ├── monitor.c         # burnout and end condition detection
    ├── print.c           # thread-safe status output
    ├── sleep.c           # precise sleep that stops early
    ├── time.c            # time helpers (ms)
    └── clean.c           # freeing resources
```

## Instructions

### Requirements

- A C compiler (`gcc` / `cc`) and `make`
- A POSIX system with pthreads (Linux or macOS)

### Compilation

```sh
make          # builds the ./codexion binary
make clean    # removes object files
make fclean   # removes object files and the binary
make re       # full rebuild
make debug    # build with -g -O0 -fsanitize=address
```

### Execution

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| # | Argument | Meaning |
|---|----------|---------|
| 1 | `number_of_coders` | Number of coders (and dongles) |
| 2 | `time_to_burnout` | ms a coder can go without starting a compile before burning out |
| 3 | `time_to_compile` | ms spent compiling (holding 2 dongles) |
| 4 | `time_to_debug` | ms spent debugging |
| 5 | `time_to_refactor` | ms spent refactoring |
| 6 | `number_of_compiles_required` | the simulation ends when every coder has compiled this many times |
| 7 | `dongle_cooldown` | ms a released dongle stays unavailable |
| 8 | `scheduler` | `fifo` or `edf` |

All numeric arguments must be non-negative integers. Exactly 8 arguments are
needed. Otherwise the program prints an error and exits with status `1`.

### Examples

```sh
# 4 coders, nobody should burn out, ends after each coder compiles twice
./codexion 4 800 200 100 100 2 50 fifo

# Same setup with the Earliest-Deadline-First scheduler
./codexion 4 800 200 100 100 2 50 edf

# Too short burnout time: a coder burns out
./codexion 2 300 200 100 100 5 50 edf

# Only one coder: there is only one dongle, so they burn out
./codexion 1 800 200 100 100 2 50 fifo
```

### Output format

Each state change is printed as `<timestamp_ms> <coder_id> <action>`:

```
0 0 has taken a dongle
0 0 is compiling
200 0 is debugging
300 0 is refactoring
301 1 has burned out
```

### Checking for data races and leaks

```sh
make debug && ./codexion 4 800 200 100 100 2 50 fifo   # AddressSanitizer
# Linux:
valgrind --tool=helgrind ./codexion 4 800 200 100 100 2 50 fifo
valgrind --leak-check=full ./codexion 4 800 200 100 100 2 50 fifo
```

## Resources

### References

- [POSIX Threads Programming — LLNL HPC Tutorials](https://hpc-tutorials.llnl.gov/posix/)
- [OSTEP — Condition Variables (Remzi & Andrea Arpaci-Dusseau)](https://pages.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf)
- [OSTEP — Semaphores](https://pages.cs.wisc.edu/~remzi/OSTEP/threads-sema.pdf)
- [Dining philosophers problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest deadline first scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- `man pthread_create`, `man pthread_mutex_lock`, `man pthread_cond_wait`,
  `man pthread_cond_timedwait`, `man gettimeofday`

### Use of AI

AI (Claude, by Anthropic) was used as a learning assistant, not as a code generator
for the core logic:

- **Concept explanations**: mutexes vs. condition variables, how deadlock and
  starvation happen, and how FIFO / EDF scheduling differ.
- **Design discussion**: talking through how to use a priority queue (heap) for
  fair dongle allocation. Hints and guiding questions were preferred over ready-made
  answers.
- **Debugging help**: finding the cause of segmentation faults and of problems in
  thread creation failure / cleanup paths.
- **Documentation**: drafting this README.

All code was written, reviewed, and tested by the author, who can explain every part.
