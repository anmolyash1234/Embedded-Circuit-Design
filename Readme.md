# 🕒 Digital Clock & Stopwatch using 8051 Microcontroller

This project implements a **Digital Clock** and **Stopwatch** using the **8051 microcontroller** and **6 common-cathode 7-segment displays**. It supports real-time display in `hh:mm:ss` format, with user-controlled **start/stop** functionality and **dual display modes**: clock and stopwatch.

---

## 📦 Features

- 🕘 **Displays time** in `hh:mm:ss` format using 6-digit 7-segment display
- 🔁 **Two modes**:
  - **Clock Mode** – continuously running real-time clock
  - **Stopwatch Mode** – counts up from 00:00:00
- 🖲️ **Button Controls**:
  - `Mode` – toggles between Clock and Stopwatch
  - `Start/Stop` – controls timing in both modes
- ⏱️ **Uses Timer0 interrupt** for 1-second updates
- 💡 **Multiplexed 7-segment display** control for smooth digit transitions

---

## 🧰 Hardware Components

| Component                     | Quantity |
|------------------------------|----------|
| AT89C51 / 8051 Microcontroller | 1        |
| 7-Segment Displays (Common Cathode) | 6        |
| Push Buttons (Mode, Start/Stop)     | 2        |
| Current-Limiting Resistors    | 6–12     |
| Breadboard / PCB              | 1        |
| Crystal Oscillator (11.0592 MHz) | 1     |
| Capacitors, Power Supply, Wires | —      |

---

## 🔧 How It Works

- **Timer0** is configured in mode 1 to generate a 1-second interrupt.
- On each interrupt, the ISR updates seconds, minutes, and hours.
- A **mode switch** selects between a normal clock and a stopwatch.
- A **start/stop button** allows users to pause/resume the counting.
- **Multiplexing** is done by enabling one 7-segment digit at a time using port pins.

---

## 📂 Project Structure

