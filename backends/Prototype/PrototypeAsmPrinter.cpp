//===-- PrototypeAsmPrinter.cpp - Prototype LLVM assembly writer ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the Prototype assembly language.
//
//===----------------------------------------------------------------------===//

#include "PrototypeAluCode.h"
#include "PrototypeCondCode.h"
#include "PrototypeInstrInfo.h"
#include "PrototypeMCInstLower.h"
#include "PrototypeTargetMachine.h"
#include "MCTargetDesc/PrototypeInstPrinter.h"
#include "TargetInfo/PrototypeTargetInfo.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "asm-printer"

using namespace llvm;

namespace {
class PrototypeAsmPrinter : public AsmPrinter {
public:
  explicit PrototypeAsmPrinter(TargetMachine &TM,
                           std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "Prototype Assembly Printer"; }

  void printOperand(const MachineInstr *MI, int OpNum, raw_ostream &O);
  bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                       const char *ExtraCode, raw_ostream &O) override;
  void emitInstruction(const MachineInstr *MI) override;
  bool isBlockOnlyReachableByFallthrough(
      const MachineBasicBlock *MBB) const override;
  void printMemOperand(const MachineInstr *MI, int opNum, raw_ostream &OS);

private:
  void customEmitInstruction(const MachineInstr *MI);
  void emitCallInstruction(const MachineInstr *MI);
};
} // end of anonymous namespace

void PrototypeAsmPrinter::printOperand(const MachineInstr *MI, int OpNum,
                                   raw_ostream &O) {
  const MachineOperand &MO = MI->getOperand(OpNum);

  switch (MO.getType()) {
  case MachineOperand::MO_Register:
    O << PrototypeInstPrinter::getRegisterName(MO.getReg());
    break;

  case MachineOperand::MO_Immediate:
    O << MO.getImm();
    break;

  case MachineOperand::MO_MachineBasicBlock:
    O << *MO.getMBB()->getSymbol();
    break;

  case MachineOperand::MO_GlobalAddress:
    O << *getSymbol(MO.getGlobal());
    break;

  case MachineOperand::MO_BlockAddress: {
    MCSymbol *BA = GetBlockAddressSymbol(MO.getBlockAddress());
    O << BA->getName();
    break;
  }

  case MachineOperand::MO_ExternalSymbol:
    O << *GetExternalSymbolSymbol(MO.getSymbolName());
    break;

  case MachineOperand::MO_JumpTableIndex:
    O << MAI->getPrivateGlobalPrefix() << "JTI" << getFunctionNumber() << '_'
      << MO.getIndex();
    break;

  case MachineOperand::MO_ConstantPoolIndex:
    O << MAI->getPrivateGlobalPrefix() << "CPI" << getFunctionNumber() << '_'
      << MO.getIndex();
    return;

  default:
    llvm_unreachable("<unknown operand type>");
  }
}

// PrintAsmOperand - Print out an operand for an inline asm expression.
bool PrototypeAsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                                      const char *ExtraCode, raw_ostream &O) {
  printOperand(MI, OpNo, O);
  return true;
}

void PrototypeAsmPrinter::printMemOperand(const MachineInstr *MI, int OpNum, raw_ostream &O) {
  const MachineOperand &MO = MI->getOperand(OpNum);

  assert(MO.isReg() && "Expecting a register operand");

  O << "[";
  O << PrototypeInstPrinter::getRegisterName(MO.getReg());

  if (MI->getNumOperands() > OpNum + 1) {
    const MachineOperand &OffsetMO = MI->getOperand(OpNum + 1);
    assert(OffsetMO.isImm() && "Expecting an immediate operand");
    int64_t Offset = OffsetMO.getImm();
    if (Offset > 0)
      O << "+" << Offset;
    else if (Offset < 0)
      O << Offset;
  }

  O << "]";
}

//===----------------------------------------------------------------------===//

void PrototypeAsmPrinter::emitInstruction(const MachineInstr *MI) {
  PrototypeMCInstLower MCInstLowering(OutContext, *this);
  MCSubtargetInfo STI = getSubtargetInfo();
  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);
  OutStreamer->emitInstruction(TmpInst, STI);
}

// isBlockOnlyReachableByFallthough - Return true if the basic block has
// exactly one predecessor and the control transfer mechanism between
// the predecessor and this block is a fall-through.
// FIXME: could the overridden cases be handled in analyzeBranch?
bool PrototypeAsmPrinter::isBlockOnlyReachableByFallthrough(
    const MachineBasicBlock *MBB) const {
return false;
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePrototypeAsmPrinter() {
  RegisterAsmPrinter<PrototypeAsmPrinter> X(getThePrototypeTarget());
}
