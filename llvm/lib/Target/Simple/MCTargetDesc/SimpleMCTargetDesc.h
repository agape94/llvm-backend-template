//===-- SimpleMCTargetDesc.h - Simple Target Descriptions ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides Simple specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIMPLE_MCTARGETDESC_SIMPLEMCTARGETDESC_H
#define LLVM_LIB_TARGET_SIMPLE_MCTARGETDESC_SIMPLEMCTARGETDESC_H

#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCSubtargetInfo;
class Target;

extern Target TheSimpleTarget;
} // namespace llvm

// Defines symbolic names for Simple registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "SimpleGenRegisterInfo.inc"

// Defines symbolic names for the Simple instructions.
#define GET_INSTRINFO_ENUM
#include "SimpleGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "SimpleGenSubtargetInfo.inc"

#endif // LLVM_LIB_TARGET_SIMPLE_MCTARGETDESC_SIMPLEMCTARGETDESC_H
