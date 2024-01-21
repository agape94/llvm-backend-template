//===-- Simple.h - Top-level interface for Simple representation --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// Simple back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIMPLE_SIMPLE_H
#define LLVM_LIB_TARGET_SIMPLE_SIMPLE_H

#include "llvm/Pass.h"

namespace llvm {
class FunctionPass;
class SimpleTargetMachine;
class PassRegistry;

// createSimpleISelDag - This pass converts a legalized DAG into a
// Simple-specific DAG, ready for instruction scheduling.
FunctionPass *createSimpleISelDag(SimpleTargetMachine &TM);

// createSimpleDelaySlotFillerPass - This pass fills delay slots
// with useful instructions or nop's
FunctionPass *createSimpleDelaySlotFillerPass(const SimpleTargetMachine &TM);

// createSimpleMemAluCombinerPass - This pass combines loads/stores and
// arithmetic operations.
FunctionPass *createSimpleMemAluCombinerPass();

// createSimpleSetflagAluCombinerPass - This pass combines SET_FLAG and ALU
// operations.
FunctionPass *createSimpleSetflagAluCombinerPass();

void initializeSimpleDAGToDAGISelPass(PassRegistry &);

} // namespace llvm

#endif // LLVM_LIB_TARGET_SIMPLE_SIMPLE_H
