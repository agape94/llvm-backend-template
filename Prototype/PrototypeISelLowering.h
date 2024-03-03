//===-- PrototypeISelLowering.h - Prototype DAG Lowering Interface -....-*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that Prototype uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PROTOTYPE_PROTOTYPEISELLOWERING_H
#define LLVM_LIB_TARGET_PROTOTYPE_PROTOTYPEISELLOWERING_H

#include "Prototype.h"
#include "PrototypeRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
namespace PrototypeISD {
enum {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,

  // Return with a glue operand. Operand 0 is the chain operand.
  RET,

  // CALL - These operations represent an abstract call instruction, which
  // includes a bunch of information.
  // CALL
};
} // namespace PrototypeISD

class PrototypeSubtarget;

class PrototypeTargetLowering : public TargetLowering {
public:
  PrototypeTargetLowering(const TargetMachine &TM, const PrototypeSubtarget &STI);
  
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

  const PrototypeRegisterInfo *TRI;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_PROTOTYPE_PROTOTYPEISELLOWERING_H
