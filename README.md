# ANNA
ANNA(Asadullah's Neural Network Architecture) is our new experimental architecture which we actively develop.
It's supposed to be a more efficient alternative to the Transformers architecture for roleplaying, story-telling and similar tasks which don't require the amount of performance as the average Transformer model offers.
Currently, it's simply an implementation of a MLP(Multi-Layer-Perceptron) with thread support through OMP.

# Getting started
Ensure you have `git` and `g++` installed on the device you want to run following commands on.
First,
```
git clone https://github.com/XeTute/ANNA
cd ANNA
```
then, on Linux:
```
g++ -D__USE_MINGW_ANSI_STDIO=0 -O3 -Ofast -march=native -mtune=native -flto -fomit-frame-pointer -funroll-loops -ffast-math -fopenmp -fwhole-program -fno-exceptions -fno-rtti -fexceptions main.cpp ANNA.hpp -o anna
chmod +x anna
./anna
```
or, if you're on Windows
```
g++ -D__USE_MINGW_ANSI_STDIO=0 -O3 -Ofast -march=native -mtune=native -flto -fomit-frame-pointer -funroll-loops -ffast-math -fopenmp -fwhole-program -fno-exceptions -fno-rtti -fexceptions main.cpp ANNA.hpp -o anna.exe
./anna.exe
```
This will download, compile and execute the latest main.cpp found on this GitHub repo.

# Other Notices
Currently, only CPUs(which support OpenMP / OMP) are supported, but we're planning to add support for CUDA.

Performance-wise, we noticed that smaller models(less than ~1k params) will train significantly faster when being single-threaded.
For models which are larger, it makes sense to train and inference using more than one thread, while keeping the amount of threads used less than the number of cores in your processor.

Compiler-wise, we noticed that MVSC may generate an executeable which will crash due to a vector access violation(especially when trying to perform backprop on a large model), which actually doesn't occur as far as we know. Please let us know if you found out that it actually is our fault.
In cases like that, please try using the MinGW(G++) compiler. In our tests, using the MinGW compiler trained, inferenced and saved the model correctly.
Also, if you're able to use MVSC without the executeable crashing, please use it, as it's faster on Windows platforms by a significant factor.

# Star History
<a href="https://star-history.com/#XeTute/ANNA&Date">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=XeTute/ANNA&type=Date&theme=dark" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=XeTute/ANNA&type=Date" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=XeTute/ANNA&type=Date" />
 </picture>
</a>
