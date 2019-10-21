/** Example2
 *  g++ -std=c++14 -fopenmp -Ofast main2.cpp -lcrypto -lssl -o ex2 && OMP_CANCELLATION=True ./ex2
 */

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <string>
#include <cstring>
#include <omp.h>
#include <iomanip>
#include <unistd.h>  // sleep

#include <openssl/md5.h>


const char alphabet[] =
//  "0123456789"
//  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz";

void printHash(unsigned char *hash)
{
  unsigned char *hashByte = hash;
  while (*hashByte)
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)*(hashByte++);
  std::cout << std::dec;
}

bool crackPassword(unsigned char *passwordHash, int passwordLength, std::string &guessedPassword)
{
  // generate random string with letters from "alphabet"
  for(int i = 0; i < passwordLength; i++)
    guessedPassword[i] = alphabet[rand() % (sizeof(alphabet)-1)];

  // compute MD5 hash
  unsigned char guessedHash[MD5_DIGEST_LENGTH];
  MD5((const unsigned char*)guessedPassword.c_str(), guessedPassword.length(), guessedHash);

  // check if hash value matches passwordHash
  if (strncmp((const char *)guessedHash, (const char *)passwordHash, passwordLength) == 0)
  {
    return true;
  }

  return false;
}

int main(int argc, char *argv[])
{
  // start duration measurement
  std::chrono::time_point<std::chrono::steady_clock> tStart = std::chrono::steady_clock::now();


  std::cout << "Enter your password: ";

  std::string password;
  std::cin >> password;

  unsigned char passwordHash[MD5_DIGEST_LENGTH];
  MD5((const unsigned char*)password.c_str(), password.length(), passwordHash);
  std::cout << "Hash is: ";

  printHash(passwordHash);
  std::cout << std::endl;

  std::string foundPassword;
  bool passwordFound = false;

  // Start parallel region: a team of threads continues executing concurrently.
  #pragma omp parallel firstprivate(passwordHash), shared(passwordFound)
  {
    srand(omp_get_thread_num());
    const int passwordLength = 5;

    std::string guessedPassword(passwordLength, ' ');
    while (!passwordFound)
    {
      passwordFound = crackPassword(passwordHash, passwordLength, guessedPassword);

      #pragma omp cancellation point parallel
    }

    if (passwordFound)
    {
      std::cout << "Found password on thread " << omp_get_thread_num() << ": " << guessedPassword << std::endl;
      #pragma omp cancel parallel
    }
  }

  // end duration measurement
  std::chrono::time_point<std::chrono::steady_clock> tEnd = std::chrono::steady_clock::now();
  std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart);

  // print total duration of program
  std::cout << "Total duration: " << duration.count() << " ms." << std::endl;

  return EXIT_SUCCESS;
}
