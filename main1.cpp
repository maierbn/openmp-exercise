/** Example1
 *  g++ -std=c++14 -fopenmp main1.cpp -o ex1 && ./ex1
 */

#include <iostream>
#include <cstdlib>
#include <chrono>

int main(int argc, char *argv[])
{
  // start duration measurement
  std::chrono::time_point<std::chrono::steady_clock> tStart = std::chrono::steady_clock::now();

  // Start parallel region: a team of threads continues executing concurrently.
  #pragma omp parallel
  {
    std::cout << "in parallel region" << std::endl;




    // Only one thread will execute the following, we don't know which.
    // All threads will wait and synchronize at the end of "omp single".
    #pragma omp single 
    {
      std::cout << "in single region" << std::endl;

    }  // <-- implicit barrier



    std::cout << "in parallel region 2" << std::endl;



    #pragma omp single nowait
    {
      std::cout << "in single region 2" << std::endl;
    }





    std::cout << "in parallel region 3" << std::endl;

  }  // <-- implict barrier

  // end duration measurement
  std::chrono::time_point<std::chrono::steady_clock> tEnd = std::chrono::steady_clock::now();
  std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart);

  // print total duration of program
  std::cout << "Total duration: " << duration.count() << " ms." << std::endl;

  return EXIT_SUCCESS;
}
