/////////////////////////////////////////////////////////////////////////////////////////////
//
// Some RGB colors provided for convenience
//
/////////////////////////////////////////////////////////////////////////////////////////////

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
#include "Arduino.h"

#define RGB(R, G, B) (((uint32_t) R<<16)+((uint32_t) G<<8)+B)

const uint32_t clAqua = RGB(0, 255, 255);
const uint32_t clBlack = RGB(0, 0, 0);
const uint32_t clBlue = RGB(0, 0, 255);
const uint32_t clCream = RGB(255, 251, 240);
const uint32_t clGray = RGB(128, 128, 128);
const uint32_t clFuchsia = RGB(255, 0, 255);
const uint32_t clGreen = RGB(0, 128, 0);
const uint32_t clLime = RGB(0, 255, 0);
const uint32_t clMaroon = RGB(128, 0, 0);
const uint32_t clNavy = RGB(0, 0, 128);
const uint32_t clOlive = RGB(128, 128, 0);
const uint32_t clPurple = RGB(255, 0, 255);
const uint32_t clRed = RGB(255, 0, 0);
const uint32_t clSilver = RGB(192, 192, 192);
const uint32_t clTeal = RGB(0, 128, 128);
const uint32_t clWhite = RGB(255, 255, 255);
const uint32_t clYellow = RGB(255, 255, 0);
