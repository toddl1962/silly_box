#pragma once

#ifdef DEBUG
  #define DebugInit(_X) Serial.begin(_X)
  #define DebugPrint(_X) Serial.print(_X)
  #define DebugPrintln(_X) Serial.println(_X)
  #define DebugHexPrint(_X) Serial.print(_X, HEX)
  #define DebugHexPrintln(_X) Serial.println(_X, HEX)
#else
  #define DebugInit(_X)
  #define DebugPrint(_X)
  #define DebugPrintln(_X)
  #define DebugHexPrint(_X)
  #define DebugHexPrintln(_X)
#endif
