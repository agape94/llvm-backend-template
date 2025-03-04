//===- SkeletonSubtarget.cpp - Skeleton Subtarget Information -----------*- C++ -*-=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the Skeleton specific subclass of TargetSubtarget.
//
//===----------------------------------------------------------------------===//

#include "SkeletonSubtarget.h"

#include "Skeleton.h"

#define DEBUG_TYPE "skeleton-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "SkeletonGenSubtargetInfo.inc"

using namespace llvm;

void SkeletonSubtarget::initSubtargetFeatures(StringRef CPU, StringRef FS) {
  std::string CPUName = std::string(CPU);
  if (CPUName.empty())
    CPUName = "generic";

  ParseSubtargetFeatures(CPUName, /*TuneCPU*/ CPUName, FS);
}

SkeletonSubtarget &SkeletonSubtarget::initializeSubtargetDependencies(StringRef CPU,
                                                                StringRef FS) {
  initSubtargetFeatures(CPU, FS);
  return *this;
}

SkeletonSubtarget::SkeletonSubtarget(const Triple &TargetTriple, StringRef Cpu,
                               StringRef FeatureString, const TargetMachine &TM,
                               const TargetOptions & /*Options*/,
                               CodeModel::Model /*CodeModel*/,
                               CodeGenOptLevel /*OptLevel*/)
    : SkeletonGenSubtargetInfo(TargetTriple, Cpu, /*TuneCPU*/ Cpu, FeatureString),
      FrameLowering(initializeSubtargetDependencies(Cpu, FeatureString)),
      TLInfo(TM, *this) {}
