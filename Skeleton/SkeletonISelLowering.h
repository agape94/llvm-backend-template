//===-- SkeletonISelLowering.h - Skeleton DAG Lowering Interface -....-*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that Skeleton uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SKELETON_SKELETONISELLOWERING_H
#define LLVM_LIB_TARGET_SKELETON_SKELETONISELLOWERING_H

#include "Skeleton.h"
#include "SkeletonRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
namespace SkeletonISD {
enum {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,

  // Return with a glue operand. Operand 0 is the chain operand.
  RET,

  /// Wrapper - A wrapper node for TargetConstantPool, TargetExternalSymbol,
  /// and TargetGlobalAddress.
  Wrapper,

  // CALL - These operations represent an abstract call instruction, which
  // includes a bunch of information.
  // CALL
};
} // namespace SkeletonISD

class SkeletonSubtarget;

class SkeletonTargetLowering : public TargetLowering {
public:
  SkeletonTargetLowering(const TargetMachine &TM, const SkeletonSubtarget &STI);
  
  const char *getTargetNodeName(unsigned Opcode) const override;
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                              bool IsVarArg,
                              const SmallVectorImpl<ISD::InputArg> &Ins,
                              const SDLoc &DL, SelectionDAG &DAG,
                              SmallVectorImpl<SDValue> &InVals) const override;
  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                    const SmallVectorImpl<ISD::OutputArg> &Outs,
                    const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                    SelectionDAG &DAG) const override;

  SDValue LowerFRAMEADDR(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
  const SkeletonRegisterInfo *TRI;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_SKELETON_SKELETONISELLOWERING_H
