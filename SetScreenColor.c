
#include <stdio.h>

#if defined(__unix) || defined(__unix__) || defined(__LINUX__)

#define RED_TEXT			      "\033[31m"
#define GREEN_TEXT          "\033[32m"
#define ORANGE_TEXT         "\033[33m"
#define BLUE_TEXT           "\033[34m"
#define PURPLE_TEXT         "\033[35m"
#define CYAN_TEXT           "\033[36m"
#define GRAY_TEXT           "\033[37m"
#define LIGHTRED_TEXT       "\033[91m"
#define LIGHTGREEN_TEXT     "\033[92m"
#define LIGHTORANGE_TEXT    "\033[93m"
#define LIGHTBLUE_TEXT      "\033[94m"
#define LIGHTPURPLE_TEXT    "\033[95m"
#define LIGHTCYAN_TEXT      "\033[96m"
#define WHITE_TEXT          "\033[97m"

int SetScreenColor (char *TextColor) {
  printf(TextColor);
  return 0;
}


#elif defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)

#include <windows.h>

#define BLUE_TEXT        FOREGROUND_BLUE
#define GREEN_TEXT       FOREGROUND_GREEN
#define RED_TEXT         FOREGROUND_RED
#define CYAN_TEXT        FOREGROUND_BLUE | FOREGROUND_GREEN
#define PURPLE_TEXT      FOREGROUND_BLUE | FOREGROUND_RED
#define ORANGE_TEXT      FOREGROUND_GREEN | FOREGROUND_RED
#define WHITE_TEXT       FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
#define GRAY_TEXT        FOREGROUND_INTENSITY
#define LIGHTBLUE_TEXT   FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define LIGHTGREEN_TEXT  FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define LIGHTRED_TEXT    FOREGROUND_RED | FOREGROUND_INTENSITY
#define LIGHTCYAN_TEXT   FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define LIGHTPURPLE_TEXT FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY
#define LIGHTORANGE_TEXT FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY

int SetScreenColor (WORD TextColor) {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
  WORD saved_attributes;

  //
  // Save current attributes 
  //
  GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
  saved_attributes = consoleInfo.wAttributes;

  SetConsoleTextAttribute(hConsole, TextColor);

  return 0;
}

#else

#define BLUE_TEXT           0
#define GREEN_TEXT          0
#define RED_TEXT            0
#define CYAN_TEXT           0
#define PURPLE_TEXT         0
#define ORANGE_TEXT         0
#define WHITE_TEXT          0
#define GRAY_TEXT           0
#define LIGHTBLUE_TEXT      0
#define LIGHTGREEN_TEXT     0
#define LIGHTRED_TEXT       0
#define LIGHTCYAN_TEXT      0
#define LIGHTPURPLE_TEXT    0
#define LIGHTORANGE_TEXT    0

int SetScreenColor (int TextColor) {
  return 0;
}

#endif


