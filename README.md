# 🚀 The AMD GPU Learning Roadmap

This roadmap takes you from a single string of text to the high-performance matrix multiplication concepts discussed in the 1998 Lin et al. paper.

## 🛠️ Challenge 1: The "Greeting" (Massive Parallelism)

The Goal: Write a kernel where 512 threads each calculate their own unique "Global ID" and print it.
### 📚 What you need to learn first:

    The __global__ Qualifier: In C++, functions are just functions. In HIP, __global__ tells the compiler: "This function is called by the CPU but runs on the GPU."

    Built-in Dim3 Variables: Research threadIdx, blockIdx, and blockDim. These are uint3 structs (having .x, .y, and .z members) that the GPU automatically fills for every thread.

    Kernel Launch Syntax: Research hipLaunchKernelGGL. It’s the "ignition switch" for the GPU.

    Host-Device Synchronization: Research hipDeviceSynchronize(). Without this, your C++ main() will finish before the GPU even starts printing.

### 📝 The Challenge Task:

    Create a kernel that calculates a 1D global index: Index=(blockIdx.x×blockDim.x)+threadIdx.x.

    Launch it with 8 blocks and 64 threads per block.

    Print the index from inside the kernel. Notice if the numbers come out in perfect 0-511 order (Hint: they won't).

## 💾 Challenge 2: The Memory Bridge (Data Movement)

The Goal: Move a vector (array) from your RAM to the GPU VRAM, modify it, and bring it back.
### 📚 What you need to learn first:

    Pointer Separation: You now have two distinct memory spaces. A pointer like float* h_data (Host) cannot be dereferenced by the GPU, and float* d_data (Device) cannot be used by the CPU.

    The Big Three Functions: 
    1.  hipMalloc(): Like malloc, but for VRAM.
    2.  hipMemcpy(): The "shuttle" between CPU and GPU.
    3.  hipFree(): Crucial for preventing GPU memory leaks.

    Error Handling: Research how to use hipGetErrorString(). GPU errors are often silent "fails" that return 0s instead of crashing.

### 📝 The Challenge Task:

    Create two arrays of N=1024 floats on the CPU.

    Allocate space on the GPU and copy the data over.

    Write a kernel that adds the two arrays together (C[i]=A[i]+B[i]).

    Copy the result back to the CPU and verify the math.

## 🧱 Challenge 3: Tiling & LDS (Shared Memory)

The Goal: Perform a small Matrix Multiplication using Local Data Share (LDS)—the heart of the 1998 paper's optimization.
### 📚 What you need to learn first:

    2D Indexing: Research how to map a 1D array to a 2D matrix: Index=row×width+col.

    __shared__ Memory: This is a tiny, super-fast "scratchpad" shared by all threads in a single block.

    Barrier Synchronization: Research __syncthreads(). It ensures every thread in a block has finished Task A before any thread starts Task B. This is vital for "Pipelining."

### 📝 The Challenge Task:

    Write a "Naive" Matrix Multiply where every thread fetches from Global Memory.

    Write a "Tiled" version:

        Load a "Tile" of Matrix A and B into __shared__ memory.

        Call __syncthreads().

        Do the math from the fast shared memory.

        Repeat for the next tile.

    The Comparison: This is where you reproduce the "logic" of the paper. Even on modern hardware, the tiled version should be significantly faster.

## 📊 Challenge 4: The Performance Lab (Profiling)

The Goal: Prove your "Tiled" version is better using actual hardware metrics.
### 📚 What you need to learn first:

    GFLOPS Calculation: Research how to calculate Giga-Floating Point Operations Per Second. For M×N matrix multiplication, the formula is (2×M×N×K)/Time.

    AMD Profiling Tools: Learn the basics of rocprof. Specifically, how to track "VALU Utilization" (how much math you're doing) vs. "MemUnitBusy" (how much time you're wasting waiting for data).

    Occupancy: Research what "Occupancy" means on an AMD GPU. It’s the ratio of active wavefronts to the maximum possible.

### 📝 The Challenge Task:

    Run your Challenge 3 code for matrix sizes: 128, 512, 1024, and 2048.

    Use rocprof to generate a CSV of the execution times.

    Graph your results. You should see the "Tiled" version's GFLOPS curve stay high while the "Naive" version drops off as the matrix gets larger.


Note : I created this roadmap to learn about GPU programming on my AMD Strix Halo Chipset (Radeon 8060S). I wanted to do a review of the research paper mentioned above in the decription and also to see if I can play around with the findings of the paper. I asked gemini to create a bare minimum roadmap that would help me learn enough about gpu programming to recreate the findings of the paper.