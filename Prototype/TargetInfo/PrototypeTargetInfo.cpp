//===-- PrototypeTargetInfo.cpp - Prototype Target Implementation -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/PrototypeTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getThePrototypeTarget() {
  static Target ThePrototypeTarget;
  return ThePrototypeTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePrototypeTargetInfo() {
  RegisterTarget<Triple::prototype> X(getThePrototypeTarget(), "prototype", "Prototype",
                                  "Prototype");
}
