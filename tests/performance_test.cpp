#include <chrono>
#include <iostream>
#include <vector>

#include "scene.h"

using namespace std;
using namespace std::chrono;

void test_generate_performance() {
  cout << "=== Sudoku Generation Performance Test ===" << endl;

  vector<double> times;
  const int iterations = 100;

  for (int i = 0; i < iterations; ++i) {
    CScene scene;

    auto start = high_resolution_clock::now();
    scene.generate();
    auto end = high_resolution_clock::now();

    duration<double, milli> duration = end - start;
    times.push_back(duration.count());

    if (i % 10 == 0) {
      cout << "Generation " << i << " completed, time: " << duration.count() << " ms"
           << endl;
    }
  }

  // Calculate statistics
  double sum = 0, min = times[0], max = times[0];
  for (double time : times) {
    sum += time;
    if (time < min) min = time;
    if (time > max) max = time;
  }
  double avg = sum / iterations;

  cout << "\nStatistics: " << endl;
  cout << "Test count: " << iterations << endl;
  cout << "Average time: " << avg << " ms" << endl;
  cout << "Minimum time: " << min << " ms" << endl;
  cout << "Maximum time: " << max << " ms" << endl;
  cout << endl;
}

void test_erase_performance() {
  cout << "=== Random Erase Performance Test ===" << endl;

  CScene scene;
  scene.generate();

  vector<int> difficulty_levels = {30, 40, 50, 60, 70};

  for (int level : difficulty_levels) {
    auto start = high_resolution_clock::now();
    scene.eraseRandomGrids(level);
    auto end = high_resolution_clock::now();

    duration<double, milli> duration = end - start;
    cout << "Erased " << level << " cells, time: " << duration.count() << " ms"
         << endl;

    // Regenerate board for next test
    scene.generate();
  }
  cout << endl;
}

void test_complete_check_performance() {
  cout << "=== Completion Check Performance Test ===" << endl;

  CScene scene;
  scene.generate();

  vector<double> times;
  const int iterations = 1000;

  for (int i = 0; i < iterations; ++i) {
    auto start = high_resolution_clock::now();
    bool result = scene.isComplete();
    auto end = high_resolution_clock::now();

    duration<double, nano> duration = end - start;
    times.push_back(duration.count());
  }

  // Calculate statistics
  double sum = 0, min = times[0], max = times[0];
  for (double time : times) {
    sum += time;
    if (time < min) min = time;
    if (time > max) max = time;
  }
  double avg = sum / iterations;

  cout << "Statistics: " << endl;
  cout << "Test count: " << iterations << endl;
  cout << "Average time: " << avg << " ns" << endl;
  cout << "Minimum time: " << min << " ns" << endl;
  cout << "Maximum time: " << max << " ns" << endl;
  cout << endl;
}

int main() {
  cout << "Sudoku Game Performance Test" << endl;
  cout << "================" << endl;
  cout << endl;

  test_generate_performance();
  test_erase_performance();
  test_complete_check_performance();

  cout << "Test completed!" << endl;
  return 0;
}