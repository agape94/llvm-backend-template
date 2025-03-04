//===-- SkeletonMCAsmInfo.cpp - Skeleton asm properties -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the SkeletonMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "SkeletonMCAsmInfo.h"

#include "llvm/TargetParser/Triple.h"

using namespace llvm;

void SkeletonMCAsmInfo::anchor() {}

SkeletonMCAsmInfo::SkeletonMCAsmInfo(const Triple & /*TheTriple*/,
                               const MCTargetOptions &Options) {}
