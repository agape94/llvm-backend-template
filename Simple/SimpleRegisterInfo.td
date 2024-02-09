//===- SimpleRegisterInfo.td - Simple Register defs ------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//  Declarations that describe the Simple register file
//===----------------------------------------------------------------------===//

// Registers are identified with 5-bit ID numbers.
class SimpleReg<bits<5> Enc, string n> : Register<n> {
  let HWEncoding{15-5} = 0;
  let HWEncoding{4-0} = Enc;
  let Namespace = "SIMPLE";
}

let Namespace = "SIMPLE" in {
  def sub_32 : SubRegIndex<32>;
}

// Integer registers
foreach i = 0-31 in {
  def R#i : SimpleReg<i, "r"#i>, DwarfRegNum<[i]>;
}

// Register classes.
class SimpleRegisterClass<list<ValueType> types, int size,
                          int alignment, dag regList, int copyCost = 1>
    : RegisterClass <"SIMPLE", types, alignment, regList> {
      let Size = size;
      let CopyCost = copyCost;
}

def GPR : SimpleRegisterClass <[i32, f32], 32, 32, (add (sequence "R%u", 0, 31))>;

// Register operands
class SimpleRegisterOperand<RegisterClass RC> : RegisterOperand<RC>
{
  let DecoderMethod = "decode"#RC#"RegisterClass";
}

def GPROperand : SimpleRegisterOperand<GPR>
{
  // Add specifics for certain registers here ...
  // Ex: If R0 is a constant register and always returns 0, we would specify this as following
  // let GIZeroRegister = R0;
}
