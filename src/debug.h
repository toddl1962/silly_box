/////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2021, Todd W. Lumpkin
//
//  This file is part of the "Silly Box" program.
//
//  "Silly Box" is free software: you can redistribute it and/or modify it under the terms 
//  of the GNU General Public License as published by the Free Software Foundation, 
//  version 3 of the License.
//
//  "Silly Box" is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along with "Silly Box" 
//  in a file named gpl-3.0.txt.  If not, see <https://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

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
