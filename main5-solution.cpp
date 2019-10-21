/** Example5
 *  g++ -std=c++14 -fopenmp -Ofast main5.cpp -o ex5 && ./ex5 && display image.ppm
 */

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <fstream>
#include <array>
#include <vector>

int main(int argc, char *argv[])
{
  // start duration measurement
  std::chrono::time_point<std::chrono::steady_clock> tStart = std::chrono::steady_clock::now();

  const int width = 1750;
  const int height = 1000;

  char result[width*height][3];

  // open file and write header
  std::ofstream file("image.ppm");
  file << "P6\n" << width << " " << height << "\n255\n";
  
  // define region of interest
#if 1
  // whole set
  const double xBegin = -2.5;
  const double xEnd = 1;
  const double yBegin = -1;
  const double yEnd = 1;
#else 
  // zoomed in
  const double xBegin = -0.75;
  const double xEnd = -0.74;
  const double yBegin = -0.14;
  const double yEnd = -0.13;

#endif

  // loop over pixels
  // test schedule types: static [8.9s], dynamic [6.6s], guided [6.3s], auto [9.2s], runtime [6.3s]
  //#pragma omp parallel for collapse(2) schedule(runtime)
  #pragma omp target teams distribute parallel for collapse(2) map(tofrom:result)
  for (int j = 0; j < height; j++)
  {
    for (int i = 0; i < width; i++)
    {
      // compute current pixel coordinates in complex plane
      double x0 = xBegin + (xEnd - xBegin) * i / (width-1);
      double y0 = yBegin + (yEnd - yBegin) * j / (height-1);

      // iterate governing sequence
      int iterationNo = 0;
      int nMaximumIterations = 1e4;
      double x = 0;
      double y = 0;

      for (; x*x + y*y <= 4.0 && iterationNo < nMaximumIterations; iterationNo++)
      {
        double xTemp = x0 + x*x - y*y;
        y = y0 + 2*x*y;
        x = xTemp;
      }

      // determine color for current pixel
      char color[3] = {char(255),char(255),char(255)};

      if (iterationNo < nMaximumIterations)
      {
        double r2 = x*x + y*y;
        double c = iterationNo + 1;                   // use this line on GPU - unfortunatelly, there are no math functions (yet)
        //double c = iterationNo + 1 - log(log2(r2));   // use this line on CPU

        double alpha = c - int(c);

        // define color palette as (r,g,b) triples
        const char colorPalette[9][3] = 
        {
          {char(255), char(255), char(0)},
          {char(255), char(200), char(0)},
          {char(255), char(150), char(50)},
          {char(255), char(100), char(100)},
          {char(255), char(0),   char(100)},
          {char(200), char(0),   char(100)},
          {char(100), char(0),   char(150)},
          {char(50),  char(50),  char(200)},
          {char(0),   char(100), char(250)}
        };
        
        const char *color0 = colorPalette[iterationNo % 9];
        const char *color1 = colorPalette[(iterationNo+1) % 9];

        color[0] = alpha*color1[0] + (1.-alpha)*(color0[0]);
        color[1] = alpha*color1[1] + (1.-alpha)*(color0[1]);
        color[2] = alpha*color1[2] + (1.-alpha)*(color0[2]);

      }

      // write color (r,g,b) ∈ [0,255]^3 to file
      //file.write(color.data(), 3);
      result[j*width + i][0] = color[0];
      result[j*width + i][1] = color[1];
      result[j*width + i][2] = color[2];
    }
  }

  for (int i = 0; i < width*height; i++)
    file.write(result[i], 3);

  file.close();

  // end duration measurement
  std::chrono::time_point<std::chrono::steady_clock> tEnd = std::chrono::steady_clock::now();
  std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart);

  // print total duration of program
  std::cout << "Total duration: " << duration.count() << " ms." << std::endl;

  return EXIT_SUCCESS;
}
