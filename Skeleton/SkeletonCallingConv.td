//===- SkeletonCallingConv.td - Calling Conventions Skeleton -------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the Skeleton architectures.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Return Value Calling Conventions
//===----------------------------------------------------------------------===//

// Skeleton C Calling convention.
def CC_Skeleton : CallingConv<[]>;
// Skeleton C return-value convention.
def RetCC_Skeleton : CallingConv<[]>;
