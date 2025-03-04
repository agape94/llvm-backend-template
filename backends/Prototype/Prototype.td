//===- Prototype.td - Describe the Prototype Target Machine --------*- tablegen -*-===//
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

include "PrototypeSchedule.td"
include "PrototypeRegisterInfo.td"
include "PrototypeCallingConv.td"
include "PrototypeInstrInfo.td"

def PrototypeInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// Prototype processors supported.
//===----------------------------------------------------------------------===//

def : ProcessorModel<"generic", PrototypeSchedModel, []>;

def PrototypeInstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def Prototype : Target {
  // Pull in Instruction Info:
  let InstructionSet = PrototypeInstrInfo;
  let AssemblyWriters = [PrototypeInstPrinter];
}
