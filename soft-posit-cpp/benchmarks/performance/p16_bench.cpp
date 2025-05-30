#include "softposit_cpp.h"
#include <benchmark/benchmark.h>
#include <chrono>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

//
// Constants matching the Rust benchmark
const posit16 X(12.5);    // 0x6c80 in hex
const posit16 Y(117.334); // 0x79ab in hex

// Basic arithmetic operations
static void BM_p16_add(benchmark::State &state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(X + Y);
  }
}

static void BM_p16_sub(benchmark::State &state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(X - Y);
  }
}

static void BM_p16_mul(benchmark::State &state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(X * Y);
  }
}

static void BM_p16_div(benchmark::State &state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(X / Y);
  }
}

// Square root operation
static void BM_p16_sqrt(benchmark::State &state) {
  for (auto _ : state) {
    posit16 temp = X;
    benchmark::DoNotOptimize(temp.sqrt());
  }
}

// Rounding operation
static void BM_p16_round(benchmark::State &state) {
  for (auto _ : state) {
    posit16 temp = Y;
    benchmark::DoNotOptimize(temp.rint());
  }
}

// Mathematical functions - these will need implementation if not already
// available Note: The following functions may need to be added to your posit16
// class

// Exponential function
static void BM_p16_exp(benchmark::State &state) {
  for (auto _ : state) {
    // Implement exp for posit16 or use an approximation
    double value = X.toDouble();
    double result = std::exp(value);
    posit16 posit_result(result);
    benchmark::DoNotOptimize(posit_result);
  }
}

// Base-2 exponential function
static void BM_p16_exp2(benchmark::State &state) {
  for (auto _ : state) {
    // Implement exp2 for posit16 or use an approximation
    double value = X.toDouble();
    double result = std::exp2(value);
    posit16 posit_result(result);
    benchmark::DoNotOptimize(posit_result);
  }
}

// Natural logarithm
static void BM_p16_ln(benchmark::State &state) {
  for (auto _ : state) {
    // Implement ln for posit16 or use an approximation
    double value = X.toDouble();
    double result = std::log(value);
    posit16 posit_result(result);
    benchmark::DoNotOptimize(posit_result);
  }
}

// Base-2 logarithm
static void BM_p16_log2(benchmark::State &state) {
  for (auto _ : state) {
    // Implement log2 for posit16 or use an approximation
    double value = X.toDouble();
    double result = std::log2(value);
    posit16 posit_result(result);
    benchmark::DoNotOptimize(posit_result);
  }
}

// Sine of pi*x
static void BM_p16_sin_pi(benchmark::State &state) {
  for (auto _ : state) {
    // Implement sin_pi for posit16 or use an approximation
    double value = X.toDouble() * M_PI;
    double result = std::sin(value);
    posit16 posit_result(result);
    benchmark::DoNotOptimize(posit_result);
  }
}

// Cosine of pi*x
static void BM_p16_cos_pi(benchmark::State &state) {
  for (auto _ : state) {
    // Implement cos_pi for posit16 or use an approximation
    double value = X.toDouble() * M_PI;
    double result = std::cos(value);
    posit16 posit_result(result);
    benchmark::DoNotOptimize(posit_result);
  }
}

// Tangent of pi*x
static void BM_p16_tan_pi(benchmark::State &state) {
  for (auto _ : state) {
    // Implement tan_pi for posit16 or use an approximation
    double value = X.toDouble() * M_PI;
    double result = std::tan(value);
    posit16 posit_result(result);
    benchmark::DoNotOptimize(posit_result);
  }
}

// Using the provided quire16 implementation
// Create a PI constant for the quire16
quire16 createQuire16PI() {
  // Convert PI to a quire16 using the toPosit and qma methods
  quire16 q;
  q.clr();
  // Use a posit16 approximation of PI
  posit16 pi_posit(3.14159265358979323846);
  q.qma(pi_posit, posit16(1.0));
  return q;
}

// Static PI variable accessible for benchmarks
static quire16 QUIRE16_PI = createQuire16PI();

static void BM_q16_add_product(benchmark::State &state) {
  for (auto _ : state) {
    quire16 q = QUIRE16_PI;
    q.qma(X, Y); // Using qma method for fused multiply-add
    benchmark::DoNotOptimize(q);
  }
}

static void BM_q16_add_posit(benchmark::State &state) {
  for (auto _ : state) {
    quire16 q = QUIRE16_PI;
    // Since there's no direct += operator for posit16,
    // we use qma with 1.0 as the second operand
    q.qma(X, posit16(1.0));
    benchmark::DoNotOptimize(q);
  }
}

static void BM_q16_to_posit(benchmark::State &state) {
  for (auto _ : state) {
    quire16 q = QUIRE16_PI;
    benchmark::DoNotOptimize(q.toPosit());
  }
}

// Configure benchmark options to match Criterion behavior
static void CustomArguments(benchmark::internal::Benchmark *b) {
  b->MinTime(3.0)       // 3 second warmup
      ->Iterations(100) // 100 samples
      ->ReportAggregatesOnly(true);
}

// Register all benchmarks with custom arguments
BENCHMARK(BM_p16_add)->Apply(CustomArguments);
BENCHMARK(BM_p16_sub)->Apply(CustomArguments);
BENCHMARK(BM_p16_mul)->Apply(CustomArguments);
BENCHMARK(BM_p16_div)->Apply(CustomArguments);
BENCHMARK(BM_p16_sqrt)->Apply(CustomArguments);
BENCHMARK(BM_p16_round)->Apply(CustomArguments);
BENCHMARK(BM_p16_exp)->Apply(CustomArguments);
BENCHMARK(BM_p16_exp2)->Apply(CustomArguments);
BENCHMARK(BM_p16_ln)->Apply(CustomArguments);
BENCHMARK(BM_p16_log2)->Apply(CustomArguments);
BENCHMARK(BM_p16_sin_pi)->Apply(CustomArguments);
BENCHMARK(BM_p16_cos_pi)->Apply(CustomArguments);
BENCHMARK(BM_p16_tan_pi)->Apply(CustomArguments);
BENCHMARK(BM_q16_add_product)->Apply(CustomArguments);
BENCHMARK(BM_q16_add_posit)->Apply(CustomArguments);
BENCHMARK(BM_q16_to_posit)->Apply(CustomArguments);

BENCHMARK_MAIN();