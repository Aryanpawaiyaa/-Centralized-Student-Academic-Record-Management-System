// =============================================================================
//  main.cpp  —  Entry point for the Academic Management System
// =============================================================================
#include "MenuController.h"
#include <iostream>

// Enable ANSI Virtual Terminal Processing and UTF-8 on Windows
#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>

  static void configureWindowsConsole() {
      HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
      if (hOut != INVALID_HANDLE_VALUE) {
          DWORD dwMode = 0;
          if (GetConsoleMode(hOut, &dwMode)) {
              dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
              SetConsoleMode(hOut, dwMode);
          }
      }
      SetConsoleOutputCP(CP_UTF8);
      SetConsoleCP(CP_UTF8);
  }
#endif

int main() {
#ifdef _WIN32
    configureWindowsConsole();
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
