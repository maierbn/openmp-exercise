.. openmp-exercise documentation master file, created by
   sphinx-quickstart on Mon Jun 24 10:11:40 2019.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.
   
Resources
===============================

* `Guide into OpenMP <https://bisqwit.iki.fi/story/howto/openmp/>`_ (web page with nice examples)
* `OpenMP reference guide <https://www.openmp.org/wp-content/uploads/OpenMPRef-5.0-0519-web.pdf>`_ (12-page PDF with compressed information)

Login
====================

.. code-block:: bash

  ssh magic@ipvslogin
  # password: soft.... (password to be changed after exercise)
  ssh argon-fs
  srun -w argon-tesla1 --pty bash

Exercise 1 - Parallel Sections
===================================
* File: `main1.cpp`
* Command: 

  .. code-block:: bash

    g++ -std=c++14 -fopenmp main1.cpp -o ex1 && ./ex1

1. Make yourself familar with the following constructs:

  .. code-block:: c++

    #pragma omp parallel
    #pragma omp single
    #pragma omp master

2. Add the following function calls and print the number of threads and the own thread number. Try different locations in the code.

  .. code-block:: c++

    #include <omp.h>
    ...
    
    omp_get_thread_num()
    omp_get_num_threads()

3. Vary the number of used threads:

  .. code-block:: bash
    
    OMP_NUM_THREADS=1 ./ex1
    OMP_NUM_THREADS=2 ./ex1
    OMP_NUM_THREADS=5 ./ex1
    # etc
    # you can also set OMP_NUM_THREADS as global environment variable:
    
    export OMP_NUM_THREADS=24
    ./ex1

4. Understand scopes of variables:
  
  .. code-block:: c++
  
    private, shared, firstprivate
  
Exercise 2 - Application: Cracking a Password Hash
======================================================
* File `main2.cpp`
* A solution can be found in the file `main2-solution.cpp`
* Command:

  .. code-block:: bash

    g++ -std=c++14 -fopenmp -Ofast main2.cpp -lcrypto -lssl -o ex2 && OMP_CANCELLATION=True ./ex2
    
1. Find out what the program does.
2. Parallelize the program. It should now use multiple threads to search for the password. Once the password is found on any thread, the search should end on all threads and the program should measure its duration.

  The following constructs can be used:
  
  .. code-block:: c++

    #pragma omp parallel  // be careful with scope of variables!
    #pragma omp cancellation point parallel
    #pragma omp cancel parallel
    
Exercise 3 - Tasks
======================
* File `main3.cpp`
* A solution can be found in the file `main3-solution.cpp`
* Command:

  .. code-block:: bash

    g++ -std=c++14 -fopenmp main3.cpp -o ex3 && ./ex3
    
1. The program should compute a fibonacci number. Fibonacci numbers are defined as:

  .. math::

   F_0 = 0, F_1 = 1,
   
   F_n = F_{n-1} + F_{n-2}
  
  Complete the `fibonacci` function.
  
2. Now we use `omp tasks` to parallelize the recursive function calls. Again, take care with the scopes of variables. In a `omp parallel` block, the following spawns a new thread that runs `function()`.

  .. code-block:: c++

    #pragma omp task
      function();
    
  The thread from which this is called continues immediately. To wait for all running tasks to finish, call
  
  .. code-block:: c++

    #pragma omp taskwait
    
Exercise 4 - For Loop and Scheduling
=======================================
* File `main4.cpp`
* A solution can be found in the file `main4-solution.cpp`
* Command:

  .. code-block:: bash

    g++ -std=c++14 -fopenmp -Ofast main4.cpp -o ex4 && ./ex4
  
1. Parallelize the for loop with 
  
  .. code-block:: c++

    #pragma omp parallel for 
    
2. Test different scheduling strategies by adding `schedule(static)`. Comprehend the output and compare the runtimes.

Exercise 5 - Mandelbrot Set
=======================================
* File `main5.cpp`
* A solution can be found in the file `main5-solution.cpp`.
* Command:

  .. code-block:: bash

    g++ -std=c++14 -fopenmp -Ofast main5.cpp -o ex5 && ./ex5
    
    # displaying the generated image is not possible on argon-tesla1
    # go on argon-fs instead in a separate terminal
    cd /data/sgs/soft_tissue_robotics/src  # adjust to your folder
    display image.ppm
  
1. Run the program and try to understand the source code (the maths is irrelevant today).
2. Parallelize the for loop (add `collapse(2)`) and inspect the result. If it is wrong, maybe something has to be changed in the code? 
3. Offload the for loop to GPU.

  .. code-block:: c++

    pragma omp target teams distribute parallel for collapse(2)

  Add proper handling of the variables with the `map` statement. You also have to adjust the line with `log` and `log2`, because there is no math support on GPU within GCC 9.2.
