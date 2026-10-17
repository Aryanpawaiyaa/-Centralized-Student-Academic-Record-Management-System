// =============================================================================
//  main.cpp  —  Entry point for the Academic Management System
// =============================================================================
#include "MenuController.h"
#include <iostream>
#include <stdexcept>

// Enable ANSI Virtual Terminal Processing on Windows
#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>

  static void enableWindowsANSI() {
      HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
      if (hOut == INVALID_HANDLE_VALUE) return;
      DWORD dwMode = 0;
      if (!GetConsoleMode(hOut, &dwMode)) return;
      dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
      SetConsoleMode(hOut, dwMode);
  }
#endif

int main() {
#ifdef _WIN32
    enableWindowsANSI();
#endif

    try {
        MenuController app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "\n[FATAL] Unhandled exception: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "\n[FATAL] Unknown exception occurred.\n";
        return 2;
    }
    return 0;
}
