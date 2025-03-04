//===-- SkeletonMCTargetDesc.cpp - Skeleton Target Descriptions -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides Skeleton specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "SkeletonMCTargetDesc.h"
#include "SkeletonInstPrinter.h"
#include "SkeletonMCAsmInfo.h"
#include "TargetInfo/SkeletonTargetInfo.h"
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
#include "SkeletonGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "SkeletonGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "SkeletonGenRegisterInfo.inc"

using namespace llvm;

static MCInstPrinter *createSkeletonMCInstPrinter(const Triple & /*T*/,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  return new SkeletonInstPrinter(MAI, MII, MRI);
}

static MCSubtargetInfo *
createSkeletonMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  std::string CPUName = std::string(CPU);
  if (CPUName.empty())
    CPUName = "generic";

  return createSkeletonMCSubtargetInfoImpl(TT, CPUName, /*TuneCPU*/ CPUName, FS);
}

static MCInstrInfo *createSkeletonMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitSkeletonMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createSkeletonMCRegisterInfo(const Triple & /*TT*/) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitSkeletonMCRegisterInfo(X, SKELETON::R0);
  return X;
}


extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSkeletonTargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfo<SkeletonMCAsmInfo> X(getTheSkeletonTarget());

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(getTheSkeletonTarget(),
                                      createSkeletonMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(getTheSkeletonTarget(),
                                    createSkeletonMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(getTheSkeletonTarget(),
                                          createSkeletonMCSubtargetInfo);

  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(getTheSkeletonTarget(),
                                        createSkeletonMCInstPrinter);

}
