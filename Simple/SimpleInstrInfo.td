//===-- SimpleInstrInfo.td - Target Description for Simple Target -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the Simple instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "SimpleInstrFormats.td"

// -------------------------------------------------- //
// Instruction Operands and Patterns
// -------------------------------------------------- //

//  These are target-independent nodes, but have target-specific formats.
def SDT_SimpleCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>,
                                            SDTCisVT<1, i32>]>;
def SDT_SimpleCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i32>,
                                          SDTCisVT<1, i32>]>;

// def Call             : SDNode<"SimpleISD::CALL", SDT_SimpleCall,
//                               [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
//                                SDNPVariadic]>;

def SimpleRetNode    : SDNode<"SimpleISD::RET", SDTBrind,
                              [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def CallSeqStart     : SDNode<"ISD::CALLSEQ_START", SDT_SimpleCallSeqStart,
                              [SDNPHasChain, SDNPOutGlue]>;
def CallSeqEnd       : SDNode<"ISD::CALLSEQ_END", SDT_SimpleCallSeqEnd,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                              "#ADJCALLSTACKDOWN $amt1 $amt2",
                              [(CallSeqStart timm:$amt1, timm:$amt2)]>
{
  let Opcode = 0b0000;
}

def ADJCALLSTACKUP   : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                              "#ADJCALLSTACKUP $amt1 $amt2",
                              [(CallSeqEnd timm:$amt1, timm:$amt2)]>
{
  let Opcode = 0b0001;
}


// -------------------------------------------------- //
// Very Simple instruction set
// -------------------------------------------------- //
 
def ADD_I : InstSimple<(outs GPR:$dst), (ins GPR:$b, i32imm:$c),
                      "assembly string for add register and imm32 instruction",
                      [(set GPR:$dst, (add GPR:$b, imm:$c))]>
{
  let Opcode = 0b1000;
}

def RET : InstSimple<(outs), (ins GPR:$a), "return $a",[(SimpleRetNode GPR:$a)]>
{
  let Opcode = 0b1100;
}