How to build:
1. `mkdir build`
2. `cd build`
3.  
    1. `cmake ..` to build slow version 
    2. `cmake -DCMAKE_CXX_FLAGS="${DCMAKE_CXX_FLAGS} -DFAST_SOLUTION" ..` to build fast version with threads
4.  `make`
5. `./task-1` to run
