# 🧮 Posit vs Float Benchmark – Linux Foundation Mentorship (Option B)

This project is a submission for the [Linux Foundation Mentorship Program (LFX) – Option B](https://lfx.linuxfoundation.org/tools/mentorship/), focused on identifying a C++ program that could benefit from replacing traditional IEEE 754 floating-point types with [Posits](https://posithub.org).

## 📌 Project Goal

To benchmark and evaluate the performance, precision, and edge-case behavior of **Posit arithmetic (via `soft-posit-cpp`)** versus traditional **IEEE 754 floating-point arithmetic**, using:
- Dot product on vectors
- Matrix multiplication (32×32)
- Edge case testing: NaR, small/large values, infinities

## 🔧 Technologies Used
- `C++11` with `soft-posit-cpp` Posit arithmetic library
- `std::chrono` for timing
- Benchmarking on Linux (Debian-based) system
- Posit type used: `posit32_t`

## 🧪 Benchmark Highlights

| Operation               | Float Time | Posit Time | Accuracy (Frobenius / Error) |
|------------------------|------------|------------|-------------------------------|
| Dot Product (size 100) | 3.47 µs    | 25.98 µs   | Absolute Error: 0.0           |
| Matrix Mult (32x32)    | 1.32 ms    | 6.07 ms    | Frobenius Norm: 0.0006        |

✅ Posits provided **better accuracy** with **robust edge case handling**, though currently slower due to software-only implementation.

---

## 🧾 Files in this Repository

- `benchmark_dot.cpp`: Main benchmarking file
- `README.md`: This document
- `Posit.docx`: Final report with results, analysis, and discussion

---

## 🚀 How to Run

1. **Clone and build soft-posit-cpp:**
   ```bash
   git clone https://github.com/Posit-Foundation/soft-posit-cpp.git
   cd soft-posit-cpp
   mkdir build && cd build
   cmake ..
   make
   ```

2. **Compile benchmark:**
   ```bash
   g++ -o benchmark_dot benchmark_dot.cpp -I../include -L. -lsoftposit -std=c++11
   ```

3. **Run benchmark:**
   ```bash
   ./benchmark_dot
   ```

> Ensure `libsoftposit.so` is in your library path or run with `LD_LIBRARY_PATH=.`

---

## 🖼️ Sample Output

```
[Dot Product]
Float:  28381.000000000000000
Posit:  28381.000000000000000
Error:  0.000000000000000

[Matrix Multiplication (32x32)]
Float-Posit difference (Frobenius norm): 0.000597598

Execution times:
Float:  3.47 µs
Posit:  25.98 µs
```

---

## 📚 References

- [soft-posit-cpp GitHub](https://github.com/Posit-Foundation/soft-posit-cpp)
- Gustafson, J.L. (2017) *Posit Arithmetic*
- IEEE 754 Standard for Floating Point Arithmetic

---

## 📎 Report and Screenshots

You can find the full write-up and screenshots in the [`Posit.docx`](./Posit.docx.pdf) document.

---

## 🙋‍♂️ Author

**Vincent Ochieng**  
Email: ohanga46@gmail.com  
