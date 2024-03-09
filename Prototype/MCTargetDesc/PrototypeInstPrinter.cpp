//===-- PrototypeInstPrinter.cpp - Convert Prototype MCInst to asm syntax ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class prints an Prototype MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "PrototypeInstPrinter.h"
#include "PrototypeMCExpr.h"
#include "PrototypeAluCode.h"
#include "PrototypeCondCode.h"
#include "MCTargetDesc/PrototypeMCTargetDesc.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#define PRINT_ALIAS_INSTR
#include "PrototypeGenAsmWriter.inc"

void PrototypeInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                    raw_ostream &OS, const char *Modifier) {
  assert((Modifier == nullptr || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg())
    OS << "%" << getRegisterName(Op.getReg());
  else if (Op.isImm())
    OS << formatHex(Op.getImm());
  else {
    assert(Op.isExpr() && "Expected an expression");
    Op.getExpr()->print(OS, &MAI);
  }
}

static void printMemoryBaseRegister(raw_ostream &OS, const unsigned AluCode,
                                    const MCOperand &RegOp) {
  assert(RegOp.isReg() && "Register operand expected");
  OS << "[";
  if (LPAC::isPreOp(AluCode))
    OS << "*";
  OS << "%" << PrototypeInstPrinter::getRegisterName(RegOp.getReg());
  if (LPAC::isPostOp(AluCode))
    OS << "*";
  OS << "]";
}

template <unsigned SizeInBits>
static void printMemoryImmediateOffset(const MCAsmInfo &MAI,
                                       const MCOperand &OffsetOp,
                                       raw_ostream &OS) {
  assert((OffsetOp.isImm() || OffsetOp.isExpr()) && "Immediate expected");
  if (OffsetOp.isImm()) {
    assert(isInt<SizeInBits>(OffsetOp.getImm()) && "Constant value truncated");
    OS << OffsetOp.getImm();
  } 
  else
  {
    OffsetOp.getExpr()->print(OS, &MAI);
  }
}

void PrototypeInstPrinter::printMemOperand(const MCInst *MI, int OpNo,
                                       raw_ostream &OS, const char *Modifier) {
  const MCOperand &RegOp = MI->getOperand(OpNo);
  const MCOperand &OffsetOp = MI->getOperand(OpNo + 1);
  const MCOperand &AluOp = MI->getOperand(OpNo + 2);
  const unsigned AluCode = AluOp.getImm();

  // Offset
  printMemoryImmediateOffset<16>(MAI, OffsetOp, OS);

  // Register
  printMemoryBaseRegister(OS, AluCode, RegOp);

}

void PrototypeInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                 StringRef Annotation,
                                 const MCSubtargetInfo & /*STI*/,
                                 raw_ostream &OS) {

  printInstruction(MI, Address, OS);
}
