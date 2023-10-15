//=-- SimpleMCInstLower.cpp - Convert Simple MachineInstr to an MCInst --------=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains code to lower Simple MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "SimpleMCInstLower.h"

#include "MCTargetDesc/SimpleMCExpr.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/IR/Constants.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

// MCSymbol *
// SimpleMCInstLower::GetGlobalAddressSymbol(const MachineOperand &MO) const {
//   return Printer.getSymbol(MO.getGlobal());
// }

// MCSymbol *
// SimpleMCInstLower::GetBlockAddressSymbol(const MachineOperand &MO) const {
//   return Printer.GetBlockAddressSymbol(MO.getBlockAddress());
// }

// MCSymbol *
// SimpleMCInstLower::GetExternalSymbolSymbol(const MachineOperand &MO) const {
//   return Printer.GetExternalSymbolSymbol(MO.getSymbolName());
// }

// MCSymbol *SimpleMCInstLower::GetJumpTableSymbol(const MachineOperand &MO) const {
//   SmallString<256> Name;
//   raw_svector_ostream(Name) << Printer.MAI->getPrivateGlobalPrefix() << "JTI"
//                             << Printer.getFunctionNumber() << '_'
//                             << MO.getIndex();
//   // Create a symbol for the name.
//   return Ctx.getOrCreateSymbol(Name.str());
// }

// MCSymbol *
// SimpleMCInstLower::GetConstantPoolIndexSymbol(const MachineOperand &MO) const {
//   SmallString<256> Name;
//   raw_svector_ostream(Name) << Printer.MAI->getPrivateGlobalPrefix() << "CPI"
//                             << Printer.getFunctionNumber() << '_'
//                             << MO.getIndex();
//   // Create a symbol for the name.
//   return Ctx.getOrCreateSymbol(Name.str());
// }

// MCOperand SimpleMCInstLower::LowerSymbolOperand(const MachineOperand &MO,
//                                                MCSymbol *Sym) const {
//   SimpleMCExpr::VariantKind Kind;

//   switch (MO.getTargetFlags()) {
//   case SimpleII::MO_NO_FLAG:
//     Kind = SimpleMCExpr::VK_Simple_None;
//     break;
//   case SimpleII::MO_ABS_HI:
//     Kind = SimpleMCExpr::VK_Simple_ABS_HI;
//     break;
//   case SimpleII::MO_ABS_LO:
//     Kind = SimpleMCExpr::VK_Simple_ABS_LO;
//     break;
//   default:
//     llvm_unreachable("Unknown target flag on GV operand");
//   }

//   const MCExpr *Expr =
//       MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None, Ctx);
//   if (!MO.isJTI() && MO.getOffset())
//     Expr = MCBinaryExpr::createAdd(
//         Expr, MCConstantExpr::create(MO.getOffset(), Ctx), Ctx);
//   Expr = SimpleMCExpr::create(Kind, Expr, Ctx);
//   return MCOperand::createExpr(Expr);
// }

MCOperand SimpleMCInstLower::LowerOperand(const MachineOperand &MO,
                                         unsigned offset) const {
  MachineOperandType MOTy = MO.getType();

  switch (MOTy) 
  {
    default: llvm_unreachable("unknown operand type");
    case MachineOperand::MO_Register:
      return MCOperand::createReg(MO.getReg());
    case MachineOperand::MO_Immediate:
      return MCOperand::createImm(MO.getImm() + offset);
  }

  return MCOperand();
}

void SimpleMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());

  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);
    MCOperand MCOp = LowerOperand(MO);

    if (MCOp.isValid())
      OutMI.addOperand(MCOp);
  }
}
