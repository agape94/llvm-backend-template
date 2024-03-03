//===-- PrototypeMCTargetDesc.h - Prototype Target Descriptions ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides Prototype specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PROTOTYPE_MCTARGETDESC_PROTOTYPEMCTARGETDESC_H
#define LLVM_LIB_TARGET_PROTOTYPE_MCTARGETDESC_PROTOTYPEMCTARGETDESC_H

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

extern Target ThePrototypeTarget;

MCAsmBackend *createPrototypeAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                    const MCRegisterInfo &MRI,
                                    const MCTargetOptions &Options);

std::unique_ptr<MCObjectTargetWriter> createPrototypeELFObjectWriter(uint8_t OSABI);
} // namespace llvm

// Defines symbolic names for Prototype registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "PrototypeGenRegisterInfo.inc"

// Defines symbolic names for the Prototype instructions.
#define GET_INSTRINFO_ENUM
#include "PrototypeGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "PrototypeGenSubtargetInfo.inc"

#endif // LLVM_LIB_TARGET_PROTOTYPE_MCTARGETDESC_PROTOTYPEMCTARGETDESC_H
