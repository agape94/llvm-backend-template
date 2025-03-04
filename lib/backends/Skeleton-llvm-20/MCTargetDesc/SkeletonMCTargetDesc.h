//===-- SkeletonMCTargetDesc.h - Skeleton Target Descriptions ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides Skeleton specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SKELETON_MCTARGETDESC_SKELETONMCTARGETDESC_H
#define LLVM_LIB_TARGET_SKELETON_MCTARGETDESC_SKELETONMCTARGETDESC_H

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

extern Target TheSkeletonTarget;

MCAsmBackend *createSkeletonAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                    const MCRegisterInfo &MRI,
                                    const MCTargetOptions &Options);

std::unique_ptr<MCObjectTargetWriter> createSkeletonELFObjectWriter(uint8_t OSABI);
} // namespace llvm

// Defines symbolic names for Skeleton registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "SkeletonGenRegisterInfo.inc"

// Defines symbolic names for the Skeleton instructions.
#define GET_INSTRINFO_ENUM
#include "SkeletonGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "SkeletonGenSubtargetInfo.inc"

#endif // LLVM_LIB_TARGET_SKELETON_MCTARGETDESC_SKELETONMCTARGETDESC_H
