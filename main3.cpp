/** Example3
 *  g++ -std=c++14 -fopenmp main3.cpp -o ex3 && ./ex3
 */

#include <iostream>
#include <cstdlib>
#include <chrono>

/** Compute the n'th fibonacci number,
 *  F_n = F_{n-1} + F_{n-2}, F_0 = 0, F_1 = 1
 */
unsigned long fibonacci(unsigned int n)
{
  return 0;
}

int main(int argc, char *argv[])
{
  // start duration measurement
  std::chrono::time_point<std::chrono::steady_clock> tStart = std::chrono::steady_clock::now();

  unsigned int n = 0;
  std::cout << "Enter value n: ";
  std::cin >> n;
  std::cout << "fibonacci(" << n << "): " << fibonacci(n) << std::endl;

  // end duration measurement
  std::chrono::time_point<std::chrono::steady_clock> tEnd = std::chrono::steady_clock::now();
  std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart);

  // print total duration of program
  std::cout << "Total duration: " << duration.count() << " ms." << std::endl;

  return EXIT_SUCCESS;
}
