//===- SimpleInstrFormats.td - Simple Instruction Formats ----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

class InstSimple<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;

  bits<4> Opcode;
  let Inst{31 - 28} = Opcode;

  let Namespace = "Simple";
  let DecoderNamespace = "Simple";

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr;
  let Pattern = pattern;
}

// Pseudo instructions
class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
    : InstSimple<outs, ins, asmstr, pattern> {
  let Inst{15 - 0} = 0;
  let isPseudo = 1;
}
