//===-- PrototypeTargetMachine.h - Define TargetMachine for Prototype --- C++ ---===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the Prototype specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PROTOTYPE_PROTOTYPETARGETMACHINE_H
#define LLVM_LIB_TARGET_PROTOTYPE_PROTOTYPETARGETMACHINE_H

#include "PrototypeISelLowering.h"
#include "PrototypeInstrInfo.h"
#include "PrototypeSelectionDAGInfo.h"
#include "PrototypeSubtarget.h"
#include "llvm/Target/TargetMachine.h"
#include <optional>

namespace llvm {

class PrototypeTargetMachine : public LLVMTargetMachine {
  PrototypeSubtarget Subtarget;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  
  PrototypeInstrInfo InstrInfo;
  PrototypeTargetLowering TLInfo;
  PrototypeFrameLowering FrameLowering;


public:
  PrototypeTargetMachine(const Target &TheTarget, const Triple &TargetTriple,
                     StringRef Cpu, StringRef FeatureString,
                     const TargetOptions &Options,
                     std::optional<Reloc::Model> RM,
                     std::optional<CodeModel::Model> CodeModel,
                     CodeGenOpt::Level OptLevel, bool JIT);

  const PrototypeSubtarget *
  getSubtargetImpl(const llvm::Function & /*Fn*/) const override {
    return &Subtarget;
  }

  TargetTransformInfo getTargetTransformInfo(const Function &F) const override;

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &pass_manager) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  MachineFunctionInfo *
  createMachineFunctionInfo(BumpPtrAllocator &Allocator, const Function &F,
                            const TargetSubtargetInfo *STI) const override;

  bool isMachineVerifierClean() const override {
    return false;
  }

  virtual const TargetRegisterInfo *getRegisterInfo() const 
  {
    return &InstrInfo.getRegisterInfo();
  }
  
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_PROTOTYPE_PROTOTYPETARGETMACHINE_H
