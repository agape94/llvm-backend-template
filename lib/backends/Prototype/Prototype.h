//===-- Prototype.h - Top-level interface for Prototype representation --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// Prototype back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PROTOTYPE_PROTOTYPE_H
#define LLVM_LIB_TARGET_PROTOTYPE_PROTOTYPE_H

#include "llvm/Pass.h"

namespace llvm {
class FunctionPass;
class PrototypeTargetMachine;
class PassRegistry;

// createPrototypeISelDag - This pass converts a legalized DAG into a
// Prototype-specific DAG, ready for instruction scheduling.
FunctionPass *createPrototypeISelDag(PrototypeTargetMachine &TM);

// createPrototypeDelaySlotFillerPass - This pass fills delay slots
// with useful instructions or nop's
FunctionPass *createPrototypeDelaySlotFillerPass(const PrototypeTargetMachine &TM);

// createPrototypeMemAluCombinerPass - This pass combines loads/stores and
// arithmetic operations.
FunctionPass *createPrototypeMemAluCombinerPass();

// createPrototypeSetflagAluCombinerPass - This pass combines SET_FLAG and ALU
// operations.
FunctionPass *createPrototypeSetflagAluCombinerPass();

void initializePrototypeDAGToDAGISelPass(PassRegistry &);

} // namespace llvm

#endif // LLVM_LIB_TARGET_PROTOTYPE_PROTOTYPE_H
