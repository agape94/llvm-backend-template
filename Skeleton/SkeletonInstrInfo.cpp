//===-- SkeletonInstrInfo.cpp - Skeleton Instruction Information ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the Skeleton implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "SkeletonInstrInfo.h"
#include "SkeletonAluCode.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "SkeletonGenInstrInfo.inc"

SkeletonInstrInfo::SkeletonInstrInfo()
    : SkeletonGenInstrInfo(SKELETON::ADJCALLSTACKDOWN, SKELETON::ADJCALLSTACKUP),
      RegisterInfo() {}
