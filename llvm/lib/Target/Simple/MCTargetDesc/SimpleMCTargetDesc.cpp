//===-- SimpleMCTargetDesc.cpp - Simple Target Descriptions -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides Simple specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "SimpleMCTargetDesc.h"
#include "SimpleInstPrinter.h"
#include "SimpleMCAsmInfo.h"
#include "TargetInfo/SimpleTargetInfo.h"
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
#include "SimpleGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "SimpleGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "SimpleGenRegisterInfo.inc"

using namespace llvm;

static MCInstPrinter *createSimpleMCInstPrinter(const Triple & /*T*/,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  return new SimpleInstPrinter(MAI, MII, MRI);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSimpleTargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfo<SimpleMCAsmInfo> X(getTheSimpleTarget());

  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(getTheSimpleTarget(),
                                        createSimpleMCInstPrinter);
}
