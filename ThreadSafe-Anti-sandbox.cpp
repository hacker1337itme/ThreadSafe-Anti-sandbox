#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <windows.h>

class SafeCounter {
private:
    int counter = 0;
    std::mutex mtx; // Protects counter

public:
    void increment() {
        std::lock_guard<std::mutex> lock(mtx); // Auto-lock/unlock
        ++counter;
    }

    int get() {
        std::lock_guard<std::mutex> lock(mtx);
        return counter;
    }
};

void worker(SafeCounter& sc, int times) {
    for (int i = 0; i < times; ++i) {
        sc.increment();
    }
}

DWORD WINAPI DummyThread(LPVOID) {
    // Dynamically adjust sleep based on current thread count
    DWORD threadCount = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        THREADENTRY32 te;
        te.dwSize = sizeof(te);
        if (Thread32First(snapshot, &te)) {
            do {
                if (te.th32OwnerProcessID == GetCurrentProcessId()) {
                    ++threadCount;
                }
            } while (Thread32Next(snapshot, &te));
        }
        CloseHandle(snapshot);
    }

    DWORD sleepTime = threadCount * 250; // Sleep 250 ms per thread
    Sleep(sleepTime);
    return 0;
}

bool detectSandboxByThreadSuspension() {
    DWORD startTime = GetTickCount();

    HANDLE hThread = CreateThread(nullptr, 0, DummyThread, nullptr, 0, nullptr);
    if (!hThread) {
        std::cerr << "Thread creation failed\n";
        return false;
    }

    Sleep(100); // Allow thread to run briefly

    SuspendThread(hThread);
    ResumeThread(hThread);

    DWORD elapsed = GetTickCount() - startTime;

    CloseHandle(hThread);

    if (elapsed > 500) {
        std::cout << "[!] Sandbox detected based on thread suspension delay: " << elapsed << " ms\n";
        return true;
    }

    std::cout << "[+] Environment appears clean (elapsed " << elapsed << " ms)\n";
    return false;
}

int main() {
    std::cout << "[Sandbox Check]\n";
    detectSandboxByThreadSuspension();

    std::cout << "\n[Thread Protection Test]\n";
    SafeCounter sc;
    const int threads = 4;
    const int incrementsPerThread = 100000;

    std::vector<std::thread> threadList;

    for (int i = 0; i < threads; ++i) {
        threadList.emplace_back(worker, std::ref(sc), incrementsPerThread);
    }

    for (auto& t : threadList) {
        t.join();
    }

    std::cout << "Final count: " << sc.get() << "\n";
    return 0;
}
