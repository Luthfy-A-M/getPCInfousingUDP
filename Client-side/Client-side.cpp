#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

void GetDriveSpace() {
    wchar_t driveLetter = L'A';
    while (driveLetter <= L'Z') {
        std::wstring drive(1, driveLetter);
        drive += L":\\"; // Append ":\\" to create a drive path

        ULARGE_INTEGER freeBytes, totalBytes, totalFreeBytes;

        if (GetDiskFreeSpaceExW(drive.c_str(), &freeBytes, &totalBytes, &totalFreeBytes)) {
            std::wcout << L"Drive " << drive << std::endl;
            std::wcout << L"  Total Space: " << totalBytes.QuadPart / (1024 * 1024) << L" MB" << std::endl;
            std::wcout << L"  Free Space: " << freeBytes.QuadPart / (1024 * 1024) << L" MB" << std::endl;
            std::wcout << L"  Total Free Space: " << totalFreeBytes.QuadPart / (1024 * 1024) << L" MB" << std::endl;
        }
        else {
            // Check if the drive is valid (e.g., CD-ROM or unmounted drive)
            if (GetLastError() != ERROR_INVALID_PARAMETER) {
                std::wcerr << L"Error accessing drive " << drive << std::endl;
            }
        }

        driveLetter++;
    }
}

void GetSystemMemoryUsage() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    if (GlobalMemoryStatusEx(&memInfo)) {
        std::wcout << L"Total Physical Memory: " << memInfo.ullTotalPhys / (1024 * 1024) << L" MB" << std::endl;
        std::wcout << L"Available Physical Memory: " << memInfo.ullAvailPhys / (1024 * 1024) << L" MB" << std::endl;
        std::wcout << L"Used Physical Memory: " << (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024 * 1024) << L" MB" << std::endl;
        std::wcout << L"Total Virtual Memory: " << memInfo.ullTotalVirtual / (1024 * 1024) << L" MB" << std::endl;
        std::wcout << L"Available Virtual Memory: " << memInfo.ullAvailVirtual / (1024 * 1024) << L" MB" << std::endl;
        std::wcout << L"Used Virtual Memory: " << (memInfo.ullTotalVirtual - memInfo.ullAvailVirtual) / (1024 * 1024) << L" MB" << std::endl;
    }
    else {
        std::wcerr << L"Error getting memory info." << std::endl;
    }
}

void MonitorResources() {
    while (true) {
        std::wcout << L"\nMemory Usage:\n";
        GetSystemMemoryUsage();
        std::wcout << L"\nDrive Space:\n";
        GetDriveSpace();

        // Wait for 1 second before the next update
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    // Start the monitoring in a separate thread
    std::thread monitoringThread(MonitorResources);

    // Join the thread to the main thread to keep the program running
    monitoringThread.join();

    return 0;
}
