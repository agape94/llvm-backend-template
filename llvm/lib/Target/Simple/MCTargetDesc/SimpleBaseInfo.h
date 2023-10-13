//===-- SimpleBaseInfo.h - Top level definitions for Simple MC ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone helper functions and enum definitions for
// the Simple target useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIMPLE_MCTARGETDESC_SIMPLEBASEINFO_H
#define LLVM_LIB_TARGET_SIMPLE_MCTARGETDESC_SIMPLEBASEINFO_H

#include "SimpleMCTargetDesc.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {

// SimpleII - This namespace holds all of the target specific flags that
// instruction info tracks.
namespace SimpleII {
// Target Operand Flag enum.
enum TOF {
  //===------------------------------------------------------------------===//
  // Simple Specific MachineOperand flags.
  MO_NO_FLAG,

  // MO_ABS_HI/LO - Represents the hi or low part of an absolute symbol
  // address.
  MO_ABS_HI,
  MO_ABS_LO,
};
} // namespace SimpleII

static inline unsigned getSimpleRegisterNumbering(unsigned Reg) {
  switch (Reg) {
  case Simple::R0:
    return 0;
  case Simple::R1:
    return 1;
  case Simple::R2:
  case Simple::PC:
    return 2;
  case Simple::R3:
    return 3;
  case Simple::R4:
  case Simple::SP:
    return 4;
  case Simple::R5:
  case Simple::FP:
    return 5;
  case Simple::R6:
    return 6;
  case Simple::R7:
    return 7;
  case Simple::R8:
  case Simple::RV:
    return 8;
  case Simple::R9:
    return 9;
  case Simple::R10:
  case Simple::RR1:
    return 10;
  case Simple::R11:
  case Simple::RR2:
    return 11;
  case Simple::R12:
    return 12;
  case Simple::R13:
    return 13;
  case Simple::R14:
    return 14;
  case Simple::R15:
  case Simple::RCA:
    return 15;
  case Simple::R16:
    return 16;
  case Simple::R17:
    return 17;
  case Simple::R18:
    return 18;
  case Simple::R19:
    return 19;
  case Simple::R20:
    return 20;
  case Simple::R21:
    return 21;
  case Simple::R22:
    return 22;
  case Simple::R23:
    return 23;
  case Simple::R24:
    return 24;
  case Simple::R25:
    return 25;
  case Simple::R26:
    return 26;
  case Simple::R27:
    return 27;
  case Simple::R28:
    return 28;
  case Simple::R29:
    return 29;
  case Simple::R30:
    return 30;
  case Simple::R31:
    return 31;
  default:
    llvm_unreachable("Unknown register number!");
  }
}
} // namespace llvm
#endif // LLVM_LIB_TARGET_SIMPLE_MCTARGETDESC_SIMPLEBASEINFO_H
