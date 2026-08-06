<h1 align="center">Rhienel</h1>
<p align="center">
  <img width="649" height="404" alt="image" src="https://github.com/user-attachments/assets/ff8c1591-0f8c-42a6-9f45-f4f86834db26" />

<p align="center">
  A really simple C and Assembly based kernel.

  Note: This screenshot is really old.
</p>

# Requirements
- Any distro of Linux, or macOS if pre-compiled.
- QEMU (specifically qemu-system-aarch64)
- Around 5 GB host storage minimum. (Or whatever is the minimum for your distro)
- arch64-none-elf-gcc
- 1 GB RAM, preferrably 4 or 8 GB of RAM.
- A human brain
- Python 3.9 or up.

# What can Rhienel do?
It's a kernel. It's a foundation of an operating system.

Currently as of release v0.1.1, it doesnt really have display drivers so you will have to run compile.py or run.py. For now, it's a microkernel. Some files and code snippets are built out of the Linux kernel v5.15.1.

# Compiling and running Rhienel

To compile Rhienel, CD into the Rhienel-master folder in your terminal and run the command:
```
python3 compile.py
```
The script will also ask if you want to run QEMU and launch the kernel. Either type in 'Y' or 'N'.


To run Rhienel, CD into the Rhienel-master folder in your terminal and run the command:
```
python3 run.py 
```

To also quit QEMU, you will have to press CTRL + A and then X.

# Folder tree and contents
```
/
├── boot
│   └── head.S
├── compile.py
├── include
│   └── assembler.h
├── linker.ld
├── main.c
├── run.py
└── stubs.c
```

- `/boot/head.S`: Validates CPU stuff, sets it up for kernel
- `/include/assembler.h`: Full of custom shortcuts and shortcuts/headers
- `/linker.ld`: Links stuff for GCC to compile the main kernel.
- `/stubs.c`: Fix stuff in `/boot/head.S`.
- `/main.c`: The main kernel.
- `compile.py`: Compile Rhienel as `image.elf`. Run this script while CDed into Rhienel folder. Also gives you option to run directly.
- `run.py`: Run Rhienel/`image.elf`. Have to be CDed into Rhienel folder.


