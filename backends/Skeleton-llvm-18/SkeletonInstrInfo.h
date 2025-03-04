//===- SkeletonInstrInfo.h - Skeleton Instruction Information ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the Skeleton implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SKELETON_SKELETONINSTRINFO_H
#define LLVM_LIB_TARGET_SKELETON_SKELETONINSTRINFO_H

#include "SkeletonRegisterInfo.h"
#include "MCTargetDesc/SkeletonMCTargetDesc.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "SkeletonGenInstrInfo.inc"

namespace llvm {

class SkeletonInstrInfo : public SkeletonGenInstrInfo {
  const SkeletonRegisterInfo RegisterInfo;

public:
  SkeletonInstrInfo();

  // getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  // such, whenever a client has an instance of instruction info, it should
  // always be able to get register info as well (through this method).
  virtual const SkeletonRegisterInfo &getRegisterInfo() const {
    return RegisterInfo;
  }
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_SKELETON_SKELETONINSTRINFO_H
