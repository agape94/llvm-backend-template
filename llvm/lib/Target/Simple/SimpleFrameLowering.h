//===-- SimpleFrameLowering.h - Define frame lowering for Simple --*- C++-*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class implements Simple-specific bits of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIMPLE_SIMPLEFRAMELOWERING_H
#define LLVM_LIB_TARGET_SIMPLE_SIMPLEFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {

class BitVector;
class SimpleSubtarget;

class SimpleFrameLowering : public TargetFrameLowering {
private:
  void determineFrameLayout(MachineFunction &MF) const;
  void replaceAdjDynAllocPseudo(MachineFunction &MF) const;

protected:
  const SimpleSubtarget &STI;

public:
  explicit SimpleFrameLowering(const SimpleSubtarget &Subtarget)
      : TargetFrameLowering(StackGrowsDown,
                            /*StackAlignment=*/Align(8),
                            /*LocalAreaOffset=*/0),
        STI(Subtarget) {}

  // emitProlog/emitEpilog - These methods insert prolog and epilog code into
  // the function.
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;

  bool hasFP(const MachineFunction & /*MF*/) const override { return true; }

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS = nullptr) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_SIMPLE_SIMPLEFRAMELOWERING_H
