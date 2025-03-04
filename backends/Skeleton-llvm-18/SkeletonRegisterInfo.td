//===- SkeletonRegisterInfo.td - Skeleton Register defs ------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//  Declarations that describe the Skeleton register file
//===----------------------------------------------------------------------===//

// Registers are identified with 5-bit ID numbers.
class SkeletonReg<bits<5> Enc, string n> : Register<n> {
  let HWEncoding{15-5} = 0;
  let HWEncoding{4-0} = Enc;
  let Namespace = "SKELETON";
}

let Namespace = "SKELETON" in {
  def sub_32 : SubRegIndex<32>;
}

// Integer registers
foreach i = 0-31 in {
  def R#i : SkeletonReg<i, "R"#i>, DwarfRegNum<[i]>;
}

// Register classes.
class SkeletonRegisterClass<list<ValueType> types, int size,
                          int alignment, dag regList, int copyCost = 1>
    : RegisterClass <"SKELETON", types, alignment, regList> {
      let Size = size;
      let CopyCost = copyCost;
}

def GPR : SkeletonRegisterClass <[i32], 32, 32, (add (sequence "R%u", 0, 31))>;

// Register operands
class SkeletonRegisterOperand<RegisterClass RC> : RegisterOperand<RC>
{
  let DecoderMethod = "decode"#RC#"RegisterClass";
}

def GPROperand : SkeletonRegisterOperand<GPR>
{
  // Add specifics for certain registers here ...
  // Ex: If R0 is a constant register and always returns 0, we would specify this as following
  // let GIZeroRegister = R0;
}
