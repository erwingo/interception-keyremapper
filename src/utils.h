#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace Utils
{
  double getDifferenceBetweenClocks(double clock1, double clock2);

  std::string ws2utf8(std::wstring input);
  std::wstring utf82ws(std::string input);
  std::vector<std::wstring> getFileByLine(std::string path);

  // Sadly when using templates, you have to define
  // the methods to use in the header file
  template <typename F>
  struct KeyClick
  {
  private:
    int keyConsecutiveClicks = 0;
    double keyDownFirstTimeClock = NULL;
    double keyUpClock = NULL;
    DWORD localKeyCode;
    F *callback;

  public:
    KeyClick(DWORD keyCode, F cb)
    {
      localKeyCode = keyCode;
      callback = cb;
    }

    void handleKeyStroke(DWORD keyCode, bool isKeyDown)
    {
      if (isKeyDown)
      {
        if (keyCode != localKeyCode)
        {
          keyDownFirstTimeClock = NULL;
          keyConsecutiveClicks = 0;
          return;
        }
        if (!keyDownFirstTimeClock)
        {
          keyDownFirstTimeClock = clock();

          if (
              !keyUpClock ||
              Utils::getDifferenceBetweenClocks(keyDownFirstTimeClock, keyUpClock) > 200)
          {
            keyConsecutiveClicks = 0;
          }
        }
      }
      else
      {
        if (keyDownFirstTimeClock)
        {
          keyUpClock = clock();
          double timeDif = Utils::getDifferenceBetweenClocks(keyUpClock, keyDownFirstTimeClock);

          if (timeDif < 200)
          {
            keyConsecutiveClicks++;
            callback(keyConsecutiveClicks);
            keyDownFirstTimeClock = NULL;
          }
          else
          {
            keyConsecutiveClicks = 0;
            keyDownFirstTimeClock = NULL;
          }
        }
      }
    }
  };

  void writeToFile(std::string name, std::string content, bool append = true, bool newLine = true);

  std::string getActiveWindowProcessName(HWND hwnd);
} // namespace Utils