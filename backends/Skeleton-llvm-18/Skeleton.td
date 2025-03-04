//===- Skeleton.td - Describe the Skeleton Target Machine --------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "SkeletonSchedule.td"
include "SkeletonRegisterInfo.td"
include "SkeletonCallingConv.td"
include "SkeletonInstrInfo.td"

def SkeletonInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// Skeleton processors supported.
//===----------------------------------------------------------------------===//

def : ProcessorModel<"generic", SkeletonSchedModel, []>;

def SkeletonInstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def Skeleton : Target {
  // Pull in Instruction Info:
  let InstructionSet = SkeletonInstrInfo;
  let AssemblyWriters = [SkeletonInstPrinter];
}
