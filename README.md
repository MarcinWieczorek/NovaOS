NovaOS
======

Build:
`make`

Run:
`make run`

### Description
The project started as a BIOS bootloader. When I managed to load some disk and enter protected mode 
(not knowing nor understanding assembly at all) I wanted to call an empty `int main(void)` written in C.
That's how it all started. Needing to understand more what the University taught me about libc I started implementing my own (based on musl headers).  
`Apetyt rośnie w miarę jedzenia.`  
Feel free to contribute. Just stick to the code style.

### Features
- [X] Working interrupts
- [X] Ring 3
- [X] VFS
- [X] Basic VFAT driver
- [X] Linux-like system call support
- [X] Some of libc implemented
- [X] CMOS

### Todo
General
  - [ ] Move IDT table to a fixed offset
  - [ ] Good memory allocation
  - [ ] Memory map detection
  - [ ] General purpose algorithms like stack, heap
  - [ ] ELF interpretter
  - [ ] Paging
  - [ ] Processes

VFAT
  - [ ] Creating files
  - [ ] Adding clusters when writing and running out of space

Drivers
  - [ ] Floppy Disk Controller
  - [ ] IDE
