# ThreadSafe-Anti-sandbox
Thread-safe Counter (Thread Protection) + Safe Counter To Detect Sandbox

To compile your updated C++ code on Windows using `g++` (MinGW), run the following command in a terminal:

```bash
g++ -std=c++17 -o ThreadSafe-Anti-sandbox.exe ThreadSafe-Anti-sandbox.cpp -lpsapi
```

**Important Notes:**

* Replace `your_file.cpp` with your actual filename.
* The `-lpsapi` link flag is needed for `CreateToolhelp32Snapshot` and `Thread32First/Next` functions, as they rely on `Kernel32.lib` and `Psapi.lib`. `-lpsapi` covers it for MinGW.

If you're using **Visual Studio**:

1. Create a new Console project.
2. Replace the main `.cpp` content with the code.
3. Build (Ctrl+Shift+B) and run (Ctrl+F5).

Let me know your compiler setup if you need tailored instructions.
