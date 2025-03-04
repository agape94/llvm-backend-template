//===-- PrototypeMCTargetDesc.cpp - Prototype Target Descriptions -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides Prototype specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "PrototypeMCTargetDesc.h"
#include "PrototypeInstPrinter.h"
#include "PrototypeMCAsmInfo.h"
#include "TargetInfo/PrototypeTargetInfo.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/TargetParser/Triple.h"
#include <cstdint>
#include <string>

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "PrototypeGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "PrototypeGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "PrototypeGenRegisterInfo.inc"

using namespace llvm;

static MCInstPrinter *createPrototypeMCInstPrinter(const Triple & /*T*/,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  return new PrototypeInstPrinter(MAI, MII, MRI);
}

static MCSubtargetInfo *
createPrototypeMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  std::string CPUName = std::string(CPU);
  if (CPUName.empty())
    CPUName = "generic";

  return createPrototypeMCSubtargetInfoImpl(TT, CPUName, /*TuneCPU*/ CPUName, FS);
}

static MCInstrInfo *createPrototypeMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitPrototypeMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createPrototypeMCRegisterInfo(const Triple & /*TT*/) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitPrototypeMCRegisterInfo(X, PROTOTYPE::R0);
  return X;
}


extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePrototypeTargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfo<PrototypeMCAsmInfo> X(getThePrototypeTarget());

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(getThePrototypeTarget(),
                                      createPrototypeMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(getThePrototypeTarget(),
                                    createPrototypeMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(getThePrototypeTarget(),
                                          createPrototypeMCSubtargetInfo);

  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(getThePrototypeTarget(),
                                        createPrototypeMCInstPrinter);

}
