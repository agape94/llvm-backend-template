//===- PrototypeRegisterInfo.td - Prototype Register defs ------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//  Declarations that describe the Prototype register file
//===----------------------------------------------------------------------===//

// Registers are identified with 5-bit ID numbers.
class PrototypeReg<bits<5> Enc, string n> : Register<n> {
  let HWEncoding{15-5} = 0;
  let HWEncoding{4-0} = Enc;
  let Namespace = "PROTOTYPE";
}

let Namespace = "PROTOTYPE" in {
  def sub_32 : SubRegIndex<32>;
}

// Integer registers
foreach i = 0-31 in {
  def R#i : PrototypeReg<i, "R"#i>, DwarfRegNum<[i]>;
}

// Register classes.
class PrototypeRegisterClass<list<ValueType> types, int size,
                          int alignment, dag regList, int copyCost = 1>
    : RegisterClass <"PROTOTYPE", types, alignment, regList> {
      let Size = size;
      let CopyCost = copyCost;
}

def GPR : PrototypeRegisterClass <[i32], 32, 32, (add (sequence "R%u", 0, 31))>;

// Register operands
class PrototypeRegisterOperand<RegisterClass RC> : RegisterOperand<RC>
{
  let DecoderMethod = "decode"#RC#"RegisterClass";
}

def GPROperand : PrototypeRegisterOperand<GPR>
{
  // Add specifics for certain registers here ...
  // Ex: If R0 is a constant register and always returns 0, we would specify this as following
  // let GIZeroRegister = R0;
}
