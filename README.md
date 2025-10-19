# Hotel Management System (C)

Simple console-based Hotel Management System implemented in C for educational/demo purposes.
This program demonstrates basic data structures, input parsing, simple state management,
and file export (CSV & JSON). It is intended for classroom submission and demonstration.

## Features
- Manage a small set of rooms (default 10 rooms; configurable in source)
- Check-in guests (name, nights)
- Check-out guests (prints bill and frees the room)
- View room status (occupied / vacant)
- Export current room status/results to CSV and JSON files

## Tech
- Language: C (C99)
- No external libraries required (uses standard C stdio/string/stdlib)
- Single-file implementation: `main.c`

## Compile
```bash
gcc -std=c99 -Wall -O2 -o hotel main.c
