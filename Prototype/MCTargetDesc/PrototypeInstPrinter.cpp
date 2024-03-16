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

void PrototypeInstPrinter::printMemOperand(const MCInst *MI, int opNum,
                       raw_ostream &OS, const char *Modifier) {
  // If this is an ADD operand, emit it like normal operands.
  // if (Modifier && !strcmp(Modifier, "arith")) {
  //   printOperand(MI, opNum, OS);
  //   OS << ", ";
  //   printOperand(MI, opNum + 1, OS);
  //   return;
  // }

  const MCOperand &Op1 = MI->getOperand(opNum);
  const MCOperand &Op2 = MI->getOperand(opNum + 1);

  bool PrintedFirstOperand = false;
  if (Op1.isReg()) {
    printOperand(MI, opNum, OS);
    PrintedFirstOperand = true;
  }

  // Skip the second operand iff it adds nothing (literal 0 or %g0) and we've
  // already printed the first one
  const bool SkipSecondOperand = PrintedFirstOperand && ((Op2.isReg()) ||
                              (Op2.isImm() && Op2.getImm() == 0));

  if (!SkipSecondOperand) {
    if (PrintedFirstOperand)
      OS << '+';
    printOperand(MI, opNum + 1, OS);
  }
}

void PrototypeInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                 StringRef Annotation,
                                 const MCSubtargetInfo & /*STI*/,
                                 raw_ostream &OS) {

  printInstruction(MI, Address, OS);
}
