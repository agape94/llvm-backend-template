//===-- SimpleInstPrinter.cpp - Convert Simple MCInst to asm syntax ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class prints an Simple MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "SimpleInstPrinter.h"
#include "SimpleMCExpr.h"
#include "SimpleAluCode.h"
#include "SimpleCondCode.h"
#include "MCTargetDesc/SimpleMCTargetDesc.h"
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
#include "SimpleGenAsmWriter.inc"

void SimpleInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) const {
  OS << StringRef(getRegisterName(Reg)).lower();
}

// bool SimpleInstPrinter::printAlias(const MCInst *MI, raw_ostream &OS) {
//   switch (MI->getOpcode()) {
//   case Simple::LDW_RI:
//     // ld 4[*%rN], %rX => ld [++imm], %rX
//     // ld -4[*%rN], %rX => ld [--imm], %rX
//     // ld 4[%rN*], %rX => ld [imm++], %rX
//     // ld -4[%rN*], %rX => ld [imm--], %rX
//     return printMemoryLoadIncrement(MI, OS, "ld", 4);
//   case Simple::LDHs_RI:
//     return printMemoryLoadIncrement(MI, OS, "ld.h", 2);
//   case Simple::LDHz_RI:
//     return printMemoryLoadIncrement(MI, OS, "uld.h", 2);
//   case Simple::LDBs_RI:
//     return printMemoryLoadIncrement(MI, OS, "ld.b", 1);
//   case Simple::LDBz_RI:
//     return printMemoryLoadIncrement(MI, OS, "uld.b", 1);
//   case Simple::SW_RI:
//     // st %rX, 4[*%rN] => st %rX, [++imm]
//     // st %rX, -4[*%rN] => st %rX, [--imm]
//     // st %rX, 4[%rN*] => st %rX, [imm++]
//     // st %rX, -4[%rN*] => st %rX, [imm--]
//     return printMemoryStoreIncrement(MI, OS, "st", 4);
//   case Simple::STH_RI:
//     return printMemoryStoreIncrement(MI, OS, "st.h", 2);
//   case Simple::STB_RI:
//     return printMemoryStoreIncrement(MI, OS, "st.b", 1);
//   default:
//     return false;
//   }
// }

void SimpleInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
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

void SimpleInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                 StringRef Annotation,
                                 const MCSubtargetInfo & /*STI*/,
                                 raw_ostream &OS) {
  // if (!PrintAlias(MI, OS) && !printAliasInstr(MI, Address, OS))
  //   printInstruction(MI, Address, OS);
  // printAnnotation(OS, Annotation);

  printInstruction(MI, Address, OS);
}
