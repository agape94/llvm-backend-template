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

// -------------------------------------------------- //
// Very Simple instruction set
// -------------------------------------------------- //

//def ADD_I : F_II<0b100000,
//                (outs GPROperand:$rd),
//                (ins GPROperand:$rs1, simm16:$imm16), 
//                "add", 
//                [(set GPROperand:$rd, (add GPROperand:$rs1, simm16:$imm16))]>;

def ADD_I : F_IR<0b110000,
                (outs GPROperand:$rd),
                (ins GPROperand:$rs1, GPROperand:$rs2), 
                "add", 
                [(set GPROperand:$rd, (add GPROperand:$rs1, GPROperand:$rs2))]>;

//let isReturn = 1, isTerminator = 1, isBarrier = 1 in {
//  def RET : Pseudo<(outs), (ins GPROperand:$src), [(SimpleRetNode GPROperand:$src)]>;
//}

// Load 32int instruction
//def LOAD32ri : SimpleInst<(outs GPR:$dst), (ins simm32:$src), 
//                          "ld", "$dst, $src", 
//                          [(set GPR:$dst, (load simm32:$src))]> {
//  let mayLoad = 1;
//}

//def LOAD16ri : F_DI<0b100100,
//                (outs GPROperand:$rd),
//                (ins simm16:$imm16), 
//                "ld", 
//                [(set GPROperand:$rd, (load simm16:$imm16))]>;


// Extract bits 0-15 (low-end) of an immediate value.
def LO16 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint32_t)N->getZExtValue() & 0xffff,
                                   SDLoc(N), MVT::i32);
}]>;

// Extract bits 16-31 (high-end) of an immediate value.
// Transformation function: shift the immediate value down into the low bits.
def HI16 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint32_t)N->getZExtValue() >> 16, SDLoc(N),
                                   MVT::i32);
}]>;

def i32lo16 : Operand<i32>, PatLeaf <(i32 imm), [{
    // i32lo16 predicate - true if the 32-bit immediate has only rightmost 16
    // bits set.
    return ((N->getZExtValue() & 0xFFFFUL) == N->getZExtValue());}], LO16>
{
  let ParserMatchClass = SImmAsmOperand<16>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeSImmOperand<16>";
}

def i32hi16 : Operand<i32>, PatLeaf <(i32 imm), [{
    // i32hi16 predicate - true if the 32-bit immediate has only leftmost 16
    // bits set.
    return ((N->getZExtValue() & 0xFFFF0000UL) == N->getZExtValue());}], HI16>
{
  let ParserMatchClass = SImmAsmOperand<16>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeSImmOperand<16>";
}

let isAsCheapAsAMove = 1, isCodeGenOnly = 1 in
  def MOVHI : F_DI<0b100100, (outs GPROperand:$rd), (ins i32hi16:$imm16),
                     "mov\t$rd, $imm16",
                     [(set GPROperand:$rd, i32hi16:$imm16)]>;
let isAsCheapAsAMove = 1, isCodeGenOnly = 1 in
  def MOVLO : F_DI<0b100101, (outs GPROperand:$rd), (ins i32lo16:$imm16),
                     "mov\t$rd, $imm16",
                     [(set GPROperand:$rd, i32lo16:$imm16)]>;

let isReturn = 1, isTerminator = 1, hasDelaySlot = 1, isBarrier = 1 in{
  def RET : SimpleInst<(outs), (ins GPROperand:$rs),
                   "ret", "",
                   [(SimpleRetNode GPROperand:$rs)]> {
    // Post encoding is not needed for RET.
    let PostEncoderMethod = "";
  }
}


// unsigned 16-bit immediate
//def : Pat<(i32 i32lo16:$imm), (OR_I_LO (i32 R0), imm:$imm)>;

// arbitrary immediate
//def : Pat<(i32 imm:$imm), (OR_I_LO (MOVHI (HI16 imm:$imm)), (LO16 imm:$imm))>;