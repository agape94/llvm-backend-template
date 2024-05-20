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

// Category: Integer.
class F_I<dag outs, dag ins, string asm, list<dag> pattern = []>
   : SkeletonInst<outs, ins, asm, pattern> {
  bits<5>  rd;
  bits<5>  rs1;
  let Inst{25-21} = rd;
  let Inst{20-16} = rs1;
}

// Category: Only destination register
class F_D<dag outs, dag ins, string asm, list<dag> pattern = []>
   : SkeletonInst<outs, ins, asm, pattern> {
  bits<5>  rd;
  let Inst{25-21} = rd;
}

// Category: Destination memory location
class F_MEM<bits<6> func, dag ins, string asm, list<dag> pattern = []>
   : SkeletonInst<(outs), ins, asm, pattern> {
  bits<16>  mem;
  bits<5>  rs1;
  let Inst{31-26} = func;
  let Inst{25-10} = mem;
  let Inst{9-5} = rs1;
}

// Category: Destination memory location register
class F_MR<bits<6> func, dag ins, string asm, list<dag> pattern = []>
   : SkeletonInst<(outs), ins, asm, pattern> {
  bits<5>  rs;
  bits<5>  dst;
  let Inst{31-26} = func;
  let Inst{25-21} = dst;
  let Inst{20-16} = rs;
}

// Category: Destination register from memory location register
class F_RM<bits<6> func, dag outs, dag ins, string asm, list<dag> pattern = []>
   : SkeletonInst<outs, ins, asm, pattern> {
  bits<5>  rs;
  bits<5>  dst;
  let Inst{31-26} = func;
  let Inst{25-21} = dst;
  let Inst{20-16} = rs;
}

// Category: Destination register and 16-bit immediate
class F_DI<bits<6> func, dag outs, dag ins, string asm, list<dag> pattern = []>
   : F_D<outs, ins, asm, pattern> {
  bits<16> imm;

  let Inst{31-26} = func;
  let Inst{20-16} = 0;
  let Inst{15-0}  = imm;
}

// Format: Two registers
class F_IR<bits<6> func, dag outs, dag ins, string asm, list<dag> pattern = []>
   : F_I<outs, ins, asm, pattern> {
  bits<5> rs2;

  let Inst{31-26} = func;
  let Inst{15-11} = rs2;
}

// Format: Integer with 16bit immediate.
class F_II<bits<6> func, dag outs, dag ins, string asm, list<dag> pattern = []>
   : F_I<outs, ins, asm, pattern> {
  bits<16> imm16;

  let Inst{31-26} = func;
  let Inst{15-0}  = imm16;
}


