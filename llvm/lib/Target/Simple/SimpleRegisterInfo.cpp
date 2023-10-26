//===-- SimpleRegisterInfo.cpp - Simple Register Information ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the Simple implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "SimpleRegisterInfo.h"
#include "SimpleAluCode.h"
#include "SimpleCondCode.h"
#include "SimpleFrameLowering.h"
#include "SimpleInstrInfo.h"
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
#include "SimpleGenRegisterInfo.inc"

using namespace llvm;

SimpleRegisterInfo::SimpleRegisterInfo() : SimpleGenRegisterInfo(Simple::R0) {}

const uint16_t *
SimpleRegisterInfo::getCalleeSavedRegs(const MachineFunction * /*MF*/) const {
  return 0;
}

BitVector SimpleRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  return Reserved;
}

bool SimpleRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                            int SPAdj, unsigned FIOperandNum,
                                            RegScavenger *RS) const {
  assert(!"NOT IMPLEMENTED");
}

Register
SimpleRegisterInfo::getFrameRegister(const MachineFunction & /*MF*/) const {
  return Simple::R0;
}
