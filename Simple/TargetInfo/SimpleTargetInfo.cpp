//===-- SimpleTargetInfo.cpp - Simple Target Implementation -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/SimpleTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheSimpleTarget() {
  static Target TheSimpleTarget;
  return TheSimpleTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSimpleTargetInfo() {
  RegisterTarget<Triple::simple> X(getTheSimpleTarget(), "simple", "Simple",
                                  "Simple");
}
