/** Example4
 *  g++ -std=c++14 -fopenmp -Ofast main4.cpp -o ex4 && ./ex4
 */

#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  const int N = 1000;
  int t[N];

  // test schedule types: static [8.9s], dynamic [6.6s], guided [6.3s], auto [9.2s], runtime [6.3s]
  #pragma omp parallel for schedule(static)
  for (int i = 0; i < N; i++)
  {
    t[i] = omp_get_thread_num();
    
    usleep(t[i] + i);   // wait for (t[i] + i) mircoseconds
  }

  // print which thread executed which number
  std::cout << "The loop was partitioning as follows: " << std::endl;
  for (int i = 0; i < N; i++)
  {
    std::cout << t[i];
  }
  std::cout << std::endl;
  
  return EXIT_SUCCESS;
}
