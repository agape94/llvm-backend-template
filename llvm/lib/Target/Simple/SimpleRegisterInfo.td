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
class SimpleReg<string n> : Register<n> {
  let Namespace = "Simple";
}

let Namespace = "Simple" in {
  def sub_32 : SubRegIndex<32>;
}

// Integer registers
foreach i = 0-6 in {
  def R#i : SimpleReg<"r"#i>, DwarfRegNum<[i]>;
}

// // Define a status register to capture the dependencies between the set flag
// // and setcc instructions
// def SR : SimpleReg< 0, "sw">;

// Register classes.
def GPR : RegisterClass<"Simple", [i32], 32,
    (add R0, R1, R2, R3, R4)>;

// // Condition code register class
// def CCR : RegisterClass<"Simple", [i32], 32, (add SR)> {
//   let CopyCost = -1; // Don't allow copying of status registers
//   let isAllocatable = 0;
// }
