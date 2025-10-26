#define NOMINMAX
#include <windows.h>
#include <iostream>

DWORD WINAPI worker(LPVOID param) {
    int* counter = (int*)param;
    for (int i = 0; i < 1000; ++i) {
        InterlockedIncrement((LONG*)counter);
    }
    return 0;
}

int main() {
    int counter = 0;
    HANDLE t1 = CreateThread(NULL, 0, worker, &counter, 0, NULL);
    HANDLE t2 = CreateThread(NULL, 0, worker, &counter, 0, NULL);

    HANDLE threads[2] = { t1, t2 };
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    CloseHandle(t1);
    CloseHandle(t2);

    if (counter == 2000) {
        std::cout << "Sync test passed\n";
        return 0;
    }
    else {
        std::cout << "Sync test failed, counter=" << counter << "\n";
        return 1;
    }
}