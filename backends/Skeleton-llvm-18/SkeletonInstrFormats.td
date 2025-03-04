//===- SkeletonInstrFormats.td - Skeleton Instruction Formats ----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

class SkeletonInst<dag outs, dag ins, string asm, list<dag> pattern = []>
    : Instruction {
  bits<32> Inst;
  bits<32> SoftFail = 0;
  let Namespace = "SKELETON";
  let Size = 4;
  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asm;
  let Pattern = pattern;
  let DecoderNamespace = "SKELETON";
}

// Pseudo instructions
class Pseudo<dag outs, dag ins, list<dag> pattern>
  : SkeletonInst<outs, ins, "", pattern> {
  let isCodeGenOnly       = 1;
  let isPseudo            = 1;
  let hasNoSchedulingInfo = 1;
}
