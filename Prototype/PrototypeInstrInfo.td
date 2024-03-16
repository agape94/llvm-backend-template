//===-- PrototypeInstrInfo.td - Target Description for Prototype Target -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the Prototype instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "PrototypeInstrFormats.td"

// -------------------------------------------------- //
// Instruction Operands and Patterns
// -------------------------------------------------- //

//  These are target-independent nodes, but have target-specific formats.
def SDT_PrototypeCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>,
                                            SDTCisVT<1, i32>]>;
def SDT_PrototypeCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i32>,
                                          SDTCisVT<1, i32>]>;

def PrototypeRetNode    : SDNode<"PrototypeISD::RET", SDTBrind,
                              [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def CallSeqStart     : SDNode<"ISD::CALLSEQ_START", SDT_PrototypeCallSeqStart,
                              [SDNPHasChain, SDNPOutGlue]>;
def CallSeqEnd       : SDNode<"ISD::CALLSEQ_END", SDT_PrototypeCallSeqEnd,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

let hasSideEffects = 1 in {
def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                              [(CallSeqStart timm:$amt1, timm:$amt2)]>;
def ADJCALLSTACKUP : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                            [(CallSeqEnd timm:$amt1, timm:$amt2)]>;
}

// -------------------------------------------------- //
// Operand Classes
// -------------------------------------------------- //

// Signed Immediate Operand Class
class SImmAsmOperand<int width> : AsmOperandClass {
  let Name = "SImm" # width;
  let RenderMethod = "addImmOperands";
  //let DiagnosticType = !strconcat("Invalid", Name);
}

// Unsigned Immediate Operand Class
class ImmAsmOperand<int width> : AsmOperandClass {
  let Name = "Imm" # width;
  let RenderMethod = "addImmOperands";
  //let DiagnosticType = !strconcat("Invalid", Name);
}

// Signed Immediate Operands
def simm16 : Operand<i32>, PatLeaf <(imm), [{return isInt<16>(N->getSExtValue());}]>
{
  let ParserMatchClass = SImmAsmOperand<16>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeSImmOperand<16>";
}

def simm32 : Operand<i32>, PatLeaf <(imm), [{return isInt<32>(N->getSExtValue());}]>
{
  let ParserMatchClass = SImmAsmOperand<32>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeSImmOperand<32>";
}

// Unsigned Immediate Operands
def imm16 : Operand<i32>, PatLeaf <(imm), [{return isInt<16>(N->getZExtValue());}]>
{
  let ParserMatchClass = ImmAsmOperand<16>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeImmOperand<16>";
}

def imm32 : Operand<i32>, PatLeaf <(imm), [{return isInt<32>(N->getZExtValue());}]>
{
  let ParserMatchClass = ImmAsmOperand<32>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeImmOperand<32>";
}

def MEMrr : Operand<i32> {
  let PrintMethod = "printMemOperand";
  let MIOperandInfo = (ops GPR, GPR);
}

// -------------------------------------------------- //
// Very Simple instruction set
// -------------------------------------------------- //

def ADD_R : F_IR<0b110000,
                (outs GPROperand:$rd),
                (ins GPROperand:$rs1, GPROperand:$rs2), 
                "add $rd, $rs1, $rs2", 
                [(set GPROperand:$rd, (add GPROperand:$rs1, GPROperand:$rs2))]>;

def ADD_I : F_II<0b111000,
                (outs GPROperand:$rd),
                (ins GPROperand:$rs1, imm16:$imm16), 
                "add $rd, $rs1, $imm16", 
                [(set GPROperand:$rd, (add GPROperand:$rs1, i32:$imm16))]>;

def LO16 : SDNodeXForm<imm, [{
  // Transformation function: get the low 16 bits.
  return getI32Imm((unsigned short)N->getZExtValue(), SDLoc(N));
}]>;

def HI16 : SDNodeXForm<imm, [{
  // Transformation function: shift the immediate value down into the low bits.
  return getI32Imm((uint32_t)(((uint32_t)N->getZExtValue() >> 16) << 16), SDLoc(N));
}]>;

def imm16ShiftedSExt : PatLeaf<(imm), [{
  if (N->getZExtValue() & 0xFFFF) return false;
  if (N->getValueType(0) == MVT::i32)
    return true;
  // For 64-bit, make sure it is sext right.
  return N->getZExtValue() == (uint64_t)(int)N->getZExtValue();
}], HI16>;

def immZExt16  : PatLeaf<(imm), [{
  return (uint64_t)N->getZExtValue() == (unsigned short)N->getZExtValue();
}], LO16>;

def MOV : F_DI<0b101000, (outs GPROperand:$rd), (ins simm16:$imm),
                       "mov $rd, $imm",
                       [(set i32:$rd, imm16ShiftedSExt:$imm)]>;

def ORI   : F_DI<0b100111, (outs GPROperand:$rd), (ins GPROperand:$RST, simm16:$imm),
                    "ori $rd, $RST, $imm",
                    [(set i32:$rd, (or i32:$RST, immZExt16:$imm))]>;

// Arbitrary immediate support.  Implement in terms of LIS/ORI.
def : Pat<(i32 imm:$imm),
          (ORI (MOV (HI16 imm:$imm)), (LO16 imm:$imm))>;

let isReturn = 1, isTerminator = 1, hasDelaySlot = 1, isBarrier = 1 in{
  def RET : PrototypeInst<(outs), (ins GPROperand:$rs),
                   "ret",
                   [(PrototypeRetNode GPROperand:$rs)]> {
    // Post encoding is not needed for RET.
    let PostEncoderMethod = "";
  }
}

// -------------------------------------------------- //
// LOAD / STORE instructions
// -------------------------------------------------- //
def ADDRrr : ComplexPattern<iPTR, 2, "SelectADDRrr", [], []>;

