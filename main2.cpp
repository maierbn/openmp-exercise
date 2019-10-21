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

// define the alphabet from which the password is created (currently only lower-case letters, try uncommenting also numbers and captial letters)
const char alphabet[] =
//  "0123456789"
//  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz";

// print out the hash value as hex numbers
void printHash(unsigned char *hash)
{
  unsigned char *hashByte = hash;
  while (*hashByte)
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)*(hashByte++);
  std::cout << std::dec;
}

// generate a random password string, compute its MD5 hash and compare it to the given passwordHash, return true if password was found
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

  // get the password from user
  std::cout << "Enter your password: ";

  std::string password;
  std::cin >> password;

  // compute the MD5 hash of the password, this value will normally be stored in the database of a login server
  unsigned char passwordHash[MD5_DIGEST_LENGTH];
  MD5((const unsigned char*)password.c_str(), password.length(), passwordHash);
  
  // print the hash
  std::cout << "Hash is: ";
  printHash(passwordHash);
  std::cout << std::endl;


  std::string foundPassword;
  bool passwordFound = false;

  // Initialize the random number generator with seed value 0. It will now always generate exactly the same number sequence (used in crackPassword)
  srand(0);
  
  // allocate an empty string to hold the guessed passwords
  const int passwordLength = 5;
  std::string guessedPassword(passwordLength, ' ');
  
  // repeat trying a random password until the correct one is found
  while (!passwordFound)
  {
    passwordFound = crackPassword(passwordHash, passwordLength, guessedPassword);
  }

  // if password was found, print message
  if (passwordFound)
  {
    std::cout << "Found password: " << guessedPassword << std::endl;
  }

  // end duration measurement
  std::chrono::time_point<std::chrono::steady_clock> tEnd = std::chrono::steady_clock::now();
  std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart);

  // print total duration of program
  std::cout << "Total duration: " << duration.count() << " ms." << std::endl;

  return EXIT_SUCCESS;
}
