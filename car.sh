g++ -D__USE_MINGW_ANSI_STDIO=0 -O3 -Ofast -march=native -mtune=native -flto -fomit-frame-pointer -funroll-loops -ffast-math -fwhole-program -fno-exceptions -fno-rtti -fexceptions -pthread main.cpp ANNA-CPU.hpp -o anna
chmod +x anna
./anna
