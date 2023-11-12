//===- Simple.td - Describe the Simple Target Machine --------*- tablegen -*-===//
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

include "SimpleSchedule.td"
include "SimpleRegisterInfo.td"
include "SimpleCallingConv.td"
include "SimpleInstrInfo.td"

def SimpleInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// Simple processors supported.
//===----------------------------------------------------------------------===//

def : ProcessorModel<"generic", SimpleSchedModel, []>;

def SimpleInstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def Simple : Target {
  // Pull in Instruction Info:
  let InstructionSet = SimpleInstrInfo;
  let AssemblyWriters = [SimpleInstPrinter];
}
