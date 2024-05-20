//===-- SkeletonRegisterInfo.cpp - Skeleton Register Information ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the Skeleton implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "SkeletonRegisterInfo.h"
#include "SkeletonAluCode.h"
#include "SkeletonCondCode.h"
#include "SkeletonFrameLowering.h"
#include "SkeletonInstrInfo.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_REGINFO_TARGET_DESC
#include "SkeletonGenRegisterInfo.inc"

using namespace llvm;

SkeletonRegisterInfo::SkeletonRegisterInfo() : SkeletonGenRegisterInfo(SKELETON::R0) {}

const uint16_t *
SkeletonRegisterInfo::getCalleeSavedRegs(const MachineFunction * /*MF*/) const {
  static const uint16_t CalleeSavedRegs[] = {
    SKELETON::R4,
    0
  };
  return CalleeSavedRegs;
}

BitVector SkeletonRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved.set(SKELETON::R8);
  // Add more registers that should be reserved
  return Reserved;
}

bool SkeletonRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                            int SPAdj, unsigned FIOperandNum,
                                            RegScavenger *RS) const {
  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  uint64_t StackSize = MFI.getStackSize();
  int64_t Offset = MFI.getObjectOffset(FrameIndex) + StackSize;

  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  DebugLoc dl = II->getDebugLoc();
  Register DstReg = MI.getOperand(0).getReg();

  // Materialize the offset via an ADD_I instruction.
  BuildMI(MF, dl, TII.get(SKELETON::MOV), DstReg)
    // .addReg(SKELETON::R2)  // Assuming R2 is the stack pointer.
    .addImm(Offset);

  // Replace the FrameIndex with the register that holds the actual memory address.
  MI.getOperand(FIOperandNum).ChangeToRegister(DstReg, false);

  // true indicates that we changed the instruction.
  return true;
}

Register
SkeletonRegisterInfo::getFrameRegister(const MachineFunction & /*MF*/) const {
  return SKELETON::R10;
}
