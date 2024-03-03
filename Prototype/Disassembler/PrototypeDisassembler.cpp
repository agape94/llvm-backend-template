//===- PrototypeDisassembler.cpp - Disassembler for Prototype -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file is part of the Prototype Disassembler.
//
//===----------------------------------------------------------------------===//

#include "PrototypeDisassembler.h"

#include "PrototypeAluCode.h"
#include "PrototypeCondCode.h"
#include "PrototypeInstrInfo.h"
#include "TargetInfo/PrototypeTargetInfo.h"
#include "llvm/MC/MCDecoderOps.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/MathExtras.h"

using namespace llvm;

typedef MCDisassembler::DecodeStatus DecodeStatus;

static MCDisassembler *createPrototypeDisassembler(const Target & /*T*/,
                                               const MCSubtargetInfo &STI,
                                               MCContext &Ctx) {
  return new PrototypeDisassembler(STI, Ctx);
}

template <int width>
static DecodeStatus decodeSImmOperand(MCInst &Inst, uint64_t Imm, uint64_t Address, const void *Decoder) {
  // Cast the immediate value to a 16-bit integer
  int16_t Imm16 = static_cast<int16_t>(Imm);

  // Add the immediate operand to the instruction
  Inst.addOperand(MCOperand::createImm(Imm16));
  return MCDisassembler::Success;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePrototypeDisassembler() {
  // Register the disassembler
  TargetRegistry::RegisterMCDisassembler(getThePrototypeTarget(),
                                         createPrototypeDisassembler);
}

PrototypeDisassembler::PrototypeDisassembler(const MCSubtargetInfo &STI, MCContext &Ctx)
    : MCDisassembler(STI, Ctx) {}

#include "PrototypeGenDisassemblerTables.inc"

DecodeStatus
PrototypeDisassembler::getInstruction(MCInst &Instr, uint64_t &Size,
                                  ArrayRef<uint8_t> Bytes, uint64_t Address,
                                  raw_ostream & /*CStream*/) const {

  MCDisassembler::DecodeStatus status = MCDisassembler::DecodeStatus::Fail; 
  Check(status, MCDisassembler::DecodeStatus::SoftFail);
  return status == MCDisassembler::DecodeStatus::Success ? status : MCDisassembler::DecodeStatus::Fail;
}

