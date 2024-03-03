//===-- PrototypeMachineFuctionInfo.cpp - Prototype machine function info ---===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PrototypeMachineFunctionInfo.h"

using namespace llvm;

void PrototypeMachineFunctionInfo::anchor() {}

MachineFunctionInfo *PrototypeMachineFunctionInfo::clone(
    BumpPtrAllocator &Allocator, MachineFunction &DestMF,
    const DenseMap<MachineBasicBlock *, MachineBasicBlock *> &Src2DstMBB)
    const {
  return DestMF.cloneInfo<PrototypeMachineFunctionInfo>(*this);
}
