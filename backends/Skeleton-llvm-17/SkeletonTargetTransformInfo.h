//===-- SkeletonTargetTransformInfo.h - Skeleton specific TTI ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file a TargetTransformInfo::Concept conforming object specific to the
// Skeleton target machine. It uses the target's detailed information to
// provide more precise answers to certain TTI queries, while letting the
// target independent and default TTI implementations handle the rest.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SKELETON_SKELETONTARGETTRANSFORMINFO_H
#define LLVM_LIB_TARGET_SKELETON_SKELETONTARGETTRANSFORMINFO_H

#include "Skeleton.h"
#include "SkeletonSubtarget.h"
#include "SkeletonTargetMachine.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/BasicTTIImpl.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/Support/MathExtras.h"

namespace llvm {
class SkeletonTTIImpl : public BasicTTIImplBase<SkeletonTTIImpl> {
  typedef BasicTTIImplBase<SkeletonTTIImpl> BaseT;
  typedef TargetTransformInfo TTI;
  friend BaseT;

  const SkeletonSubtarget *ST;
  const SkeletonTargetLowering *TLI;

  const SkeletonSubtarget *getST() const { return ST; }
  const SkeletonTargetLowering *getTLI() const { return TLI; }

public:
  explicit SkeletonTTIImpl(const SkeletonTargetMachine *TM, const Function &F)
      : BaseT(TM, F.getParent()->getDataLayout()), ST(TM->getSubtargetImpl(F)),
        TLI(ST->getTargetLowering()) {}
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_SKELETON_SKELETONTARGETTRANSFORMINFO_H
