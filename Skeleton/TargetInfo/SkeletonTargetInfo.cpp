//===-- SkeletonTargetInfo.cpp - Skeleton Target Implementation -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/SkeletonTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheSkeletonTarget() {
  static Target TheSkeletonTarget;
  return TheSkeletonTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSkeletonTargetInfo() {
  RegisterTarget<Triple::skeleton> X(getTheSkeletonTarget(), "skeleton", "Skeleton",
                                  "Skeleton");
}
