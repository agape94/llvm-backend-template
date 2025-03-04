//===-- Skeleton.h - Top-level interface for Skeleton representation --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// Skeleton back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SKELETON_SKELETON_H
#define LLVM_LIB_TARGET_SKELETON_SKELETON_H

#include "llvm/Pass.h"

namespace llvm {
class FunctionPass;
class SkeletonTargetMachine;
class PassRegistry;

// createSkeletonISelDag - This pass converts a legalized DAG into a
// Skeleton-specific DAG, ready for instruction scheduling.
FunctionPass *createSkeletonISelDag(SkeletonTargetMachine &TM);

// createSkeletonDelaySlotFillerPass - This pass fills delay slots
// with useful instructions or nop's
FunctionPass *createSkeletonDelaySlotFillerPass(const SkeletonTargetMachine &TM);

// createSkeletonMemAluCombinerPass - This pass combines loads/stores and
// arithmetic operations.
FunctionPass *createSkeletonMemAluCombinerPass();

// createSkeletonSetflagAluCombinerPass - This pass combines SET_FLAG and ALU
// operations.
FunctionPass *createSkeletonSetflagAluCombinerPass();

void initializeSkeletonDAGToDAGISelLegacyPass(PassRegistry &);

} // namespace llvm

#endif // LLVM_LIB_TARGET_SKELETON_SKELETON_H
