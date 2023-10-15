//===-- SimpleISelLowering.h - Simple DAG Lowering Interface -....-*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that Simple uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIMPLE_SIMPLEISELLOWERING_H
#define LLVM_LIB_TARGET_SIMPLE_SIMPLEISELLOWERING_H

#include "Simple.h"
#include "SimpleRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
namespace SimpleISD {
enum {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,

  // Return with a glue operand. Operand 0 is the chain operand.
  RET,

  // CALL - These operations represent an abstract call instruction, which
  // includes a bunch of information.
  // CALL
};
} // namespace SimpleISD

class SimpleSubtarget;

class SimpleTargetLowering : public TargetLowering {
public:
  SimpleTargetLowering(const TargetMachine &TM, const SimpleSubtarget &STI);

  const SimpleRegisterInfo *TRI;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_SIMPLE_SIMPLEISELLOWERING_H
