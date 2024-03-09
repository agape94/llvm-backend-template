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

// Address operand
def mem : Operand<iPTR> {
  let PrintMethod = "printMemOperand";
  let MIOperandInfo = (ops GPROperand, simm16);
  let EncoderMethod = "getMemEncoding";
}

def addr : 
  ComplexPattern<iPTR, 2, "SelectAddr", [frameindex], []>;

//===----------------------------------------------------------------------===//
// Pattern fragment for load/store
//===----------------------------------------------------------------------===//

class AlignedLoad<PatFrag Node> :
  PatFrag<(ops node:$ptr), (Node node:$ptr), [{
  LoadSDNode *LD = cast<LoadSDNode>(N);
  return LD->getMemoryVT().getSizeInBits()/8 <= LD->getAlign().value();
}]>;

class AlignedStore<PatFrag Node> :
  PatFrag<(ops node:$val, node:$ptr), (Node node:$val, node:$ptr), [{
  StoreSDNode *SD = cast<StoreSDNode>(N);
  return SD->getMemoryVT().getSizeInBits()/8 <= SD->getAlign().value();
}]>;

// Load/Store PatFrags.
def load_a          : AlignedLoad<load>;
def store_a         : AlignedStore<store>;

// -------------------------------------------------- //
// Very Prototype instruction set
// -------------------------------------------------- //

// Memory Load/Store
let canFoldAsLoad = 1 in
class LoadM<bits<6> func, string asm, PatFrag OpNode, RegisterClass RC,
            Operand MemOpnd, bit Pseudo>:
  FMem<func, (outs RC:$ra), (ins MemOpnd:$addr),
     !strconcat(asm, "$ra, $addr"),
     [(set RC:$ra, (OpNode addr:$addr))]> {
  let isPseudo = Pseudo;
}

class StoreM<bits<6> func, string asm, PatFrag OpNode, RegisterClass RC,
             Operand MemOpnd, bit Pseudo>:
  FMem<func, (outs), (ins RC:$ra, MemOpnd:$addr),
     !strconcat(asm, "\t$ra, $addr"),
     [(OpNode RC:$ra, addr:$addr)]> {
  let isPseudo = Pseudo;
}

//@ 32-bit load.
class LoadM32<bits<6> func, string asm, PatFrag OpNode, bit Pseudo = 0>
  : LoadM<func, asm, OpNode, GPR, mem, Pseudo> {
}

// 32-bit store.
class StoreM32<bits<6> func, string asm, PatFrag OpNode, bit Pseudo = 0>
  : StoreM<func, asm, OpNode, GPR, mem, Pseudo> {
}

/// Load and Store Instructions
///  aligned
def LD     : LoadM32<0x01,  "ld",  load_a>;
def ST     : StoreM32<0x02, "st",  store_a>;

def ADD_R : F_IR<0b110000,
                (outs GPROperand:$rd),
                (ins GPROperand:$rs1, GPROperand:$rs2), 
                "add $rd, $rs1, $rs2", 
                [(set GPROperand:$rd, (add GPROperand:$rs1, GPROperand:$rs2))]>;

def ADD_I : F_IR<0b110001,
                (outs GPROperand:$rd),
                (ins GPROperand:$rs1, simm32:$imm32), 
                "add $rd, $rs1, $imm32", 
                [(set GPROperand:$rd, (add GPROperand:$rs1, simm32:$imm32))]>;

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

def LDI : F_DI<0b101000, (outs GPROperand:$rd), (ins simm16:$imm),
                       "ld $rd, $imm",
                       [(set i32:$rd, imm16ShiftedSExt:$imm)]>;

def ORI   : F_DI<0b100111, (outs GPROperand:$rd), (ins GPROperand:$RST, simm16:$imm),
                    "ori $rd, $RST, $imm",
                    [(set i32:$rd, (or i32:$RST, immZExt16:$imm))]>;

// Arbitrary immediate support.  Implement in terms of LIS/ORI.
def : Pat<(i32 imm:$imm),
          (ORI (LDI (HI16 imm:$imm)), (LO16 imm:$imm))>;

let isReturn = 1, isTerminator = 1, hasDelaySlot = 1, isBarrier = 1 in{
  def RET : PrototypeInst<(outs), (ins GPROperand:$rs),
                   "ret",
                   [(PrototypeRetNode GPROperand:$rs)]> {
    // Post encoding is not needed for RET.
    let PostEncoderMethod = "";
  }
}