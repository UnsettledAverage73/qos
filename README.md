
# ⚛️ QOS – Quantum Operating System

**Status:** 🚧 _Currently Under Development_

Welcome to **QOS (Quantum Operating System)** – a personal, educational project where I’m building an operating system from scratch using **C** and **Assembly**, with the long-term vision to explore how an OS could support **Quantum Computing** in the future.

---

## 🧠 Why QOS?

This idea struck me while I was deeply exploring how operating systems work at a low level. At the same time, I was also studying the basics of **Quantum Theory** and **Quantum Computing** – and I realized:

> "Quantum computers, like classical computers, will also need operating systems – so why not start experimenting with that idea?"

---

## 🛠️ Current Features (Work in Progress)

- [x] Basic bootloader (written in NASM)
- [x] Kernel entry point in C
- [ ] Screen output using direct video memory access
- [ ] Keyboard input (driver development)
- [ ] Multitasking and context switching
- [ ] Memory management (heap + paging)
- [ ] Quantum simulation hooks (planned for future)

---

## 💻 Requirements

Make sure you have the following tools installed before building QOS:

- `nasm` – Netwide Assembler
- `gcc` – GNU Compiler Collection (cross-compiler preferred)
- `make` – Build automation tool
- `qemu` – Emulator for testing the OS
- `ld` – GNU linker (usually comes with binutils)

### Ubuntu/Debian Install

```bash
sudo apt update
sudo apt install build-essential nasm qemu
````

---

## 📦 Installation & Build (Test Mode)

### 1. Clone the Repository

```bash
git clone https://github.com/atharvapatil1210/qos.git
cd qos
```

### 2. Build the OS

```bash
make
```

> If you're using a cross-compiler, set it in the Makefile or run:
> `export CC=i686-elf-gcc`

### 3. Run in Emulator

```bash
qemu-system-i386 -kernel bin/kernel.bin
```

---

## 🧾 Documentation

I am actively writing documentation to make this project beginner-friendly and helpful for anyone interested in OS development or system-level programming.

📄 A 40+ page academic-friendly doc is under construction – with clean structure and visuals!

---

## 🤝 Want to Contribute?

Contributions, suggestions, and questions are welcome!
Check out the [GitHub repo](https://github.com/atharvapatil1210/qos) and feel free to raise an issue or open a PR.

---

## 📌 Disclaimer

This project is primarily for **learning and academic exploration**. It's not intended for production use (yet 😉).

---

## 📬 Stay Connected

You can follow the project updates on [LinkedIn](https://www.linkedin.com/in/atharva-bodade/) or contribute via GitHub!

---
