*This project has been created as part of the 42 curriculum by ypopovyc.*

# Codexion

## Description

**Codexion** is a multithreaded simulation of coders competing for a limited shared
resource — USB dongles — in order to compile quantum code. Each coder is represented
by a POSIX thread and cycles endlessly through three states: **compiling**, **debugging**,
and **refactoring**. Compiling requires holding two dongles simultaneously (one on each
side), and dongles are shared between neighboring coders sitting in a circle.

The goal of the project is to explore classic concurrency problems — mutual exclusion,
deadlock, starvation, and fair scheduling — through a practical, testable simulation.
The program supports two arbitration policies for dongle access, selectable via a flag:

- **fifo** — dongles are granted strictly in request-arrival order.
- **edf** — dongles are granted to the coder with the earliest burnout deadline
  (`last_compile_start + time_to_burnout`).

If a coder fails to start compiling before `time_to_burnout` milliseconds have elapsed,
they burn out and the simulation stops. The simulation also stops successfully once
every coder has completed `number_of_compiles_required` compiles.

## Instructions

### Compilation

```
make
```

This builds the `codexion` binary using `cc` with `-Wall -Wextra -Werror -pthread`,
using the existing C style. The official Norm checker was not run for this revision.

### Execution

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

- `scheduler` must be exactly `fifo` or `edf`.
- All arguments are mandatory; invalid input (negative values, non-integers, or an
  unrecognized scheduler) is rejected.

### Cleaning

```
make clean   # removes object files
make fclean  # removes object files and the binary
make re      # fclean + full rebuild
```

## Resources

- https://man7.org/linux/man-pages/man7/pthreads.7.html
- https://youtu.be/f-IlYeyTwzY?si=U1MrciDbJXer3Kta
- https://youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2&si=WBrd0u9GLBq94Aax
- https://edf-hpc.github.io/verrou/hg-manual.html

**AI usage:** The original curriculum implementation used AI for explanations
and debugging guidance. The 2026-10-21 maintenance fixes, regression suite and
CI were prepared with OpenAI Codex. This distinction applies to the current
source as well as the documentation.

## Blocking cases handled

- **Deadlock prevention (breaking circular wait):** dongle acquisition order depends
  on coder parity. Coders with an **even** id acquire their **left** dongle first, then
  their **right** dongle. Coders with an **odd** id acquire their **right** dongle
  first, then their **left** dongle. This breaks the circular-wait condition of
  Coffman's four conditions: it is no longer possible for every coder in the circle to
  simultaneously hold one dongle while waiting on the next, since neighboring coders
  now request their shared dongle in a consistent, non-symmetric order.
- **EDF ordering:** waiting coders are ordered by their last compile timestamp,
  preserving both waiters and breaking equal-deadline ties in FIFO order.
  This policy is not a proof of starvation freedom.
- **Cooldown handling:** a dongle is not immediately available to the next requester
  once released. On wake-up (broadcast), a waiting coder is only allowed to leave the
  wait loop and take the dongle if three conditions all hold: the cooldown period has
  elapsed, the coder is the one selected as next-in-line by the scheduling policy, and
  the dongle is currently free. Any coder not meeting all three conditions goes back to
  waiting for cooldown timedwait or until next broadcast.
- **Precise burnout detection:** burnout is monitored independently of the coder
  threads themselves, so that a coder that is blocked or sleeping does not prevent
  detection of its own deadline being missed, keeping the burnout log within the
  required precision window.
- **Log serialization:** all state-change messages are protected by a single
  dedicated logging mutex, guaranteeing that two log lines can never interleave with
  each other on output.

## Thread synchronization mechanisms

- **`pthread_mutex_t`** protects the state of each dongle (availability, cooldown
  timestamp, current holder/waiting information) so that no two coder threads can
  observe or modify a dongle's state at the same time.
- **`pthread_cond_t`**, used with `pthread_cond_timedwait` and woken via
  `pthread_cond_broadcast`, manages the waiting queue for each dongle. With a short timed recheck, a coder thread that cannot yet take a dongle sleeps on the
  condition variable and is woken whenever the dongle's state changes (release,
  cooldown expiry check point, etc.). On each wake-up, the thread re-checks under the
  mutex whether it is actually allowed to proceed (cooldown elapsed, selected by the
  scheduler, dongle free) — this re-check pattern guards against spurious wakeups and
  against race conditions where multiple threads wake up simultaneously but only one
  should actually acquire the resource.
- A single **logging mutex** serializes all `write`/`printf` calls used for state-change
  messages, ensuring thread-safe, non-interleaved output between coder threads and the
  monitor thread.
- The **monitor thread** communicates with coder threads only through the mutex-protected
  shared state (last compile start times, burnout flags); it never accesses coder-local
  data without holding the corresponding lock, which prevents a torn read/write race
  between a coder updating its own timing and the monitor evaluating it for burnout.

These locks protect shared state, but passing bounded tests is not proof of
race freedom or starvation freedom. EDF orders the two adjacent waiters by
last compile time (earliest burnout deadline), with FIFO tie-breaking.

## Verification

```sh
make re
make test
make re CFLAGS="-Wall -Wextra -Werror -pthread -g -fsanitize=address,undefined"
make test CFLAGS="-Wall -Wextra -Werror -pthread -g -fsanitize=address,undefined"
```

The public suite checks queue preservation, EDF ordering and ties, stopped
acquisition, ownership on release, invalid worker counts, one-worker burnout,
and repeated FIFO/EDF completion for 2/4/5 workers with timeouts.
ThreadSanitizer/Helgrind have not been validated for this revision.
Worker sleeps are not interruptible; shutdown can wait for the current sleep.

Maintenance fixes and public regression checks were prepared with OpenAI Codex on 2026-10-21.
