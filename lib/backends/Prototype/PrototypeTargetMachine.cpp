//===-- PrototypeTargetMachine.cpp - Define TargetMachine for Prototype ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Implements the info about Prototype target spec.
//
//===----------------------------------------------------------------------===//

#include "PrototypeTargetMachine.h"

#include "Prototype.h"
#include "PrototypeMachineFunctionInfo.h"
#include "PrototypeTargetObjectFile.h"
#include "PrototypeTargetTransformInfo.h"
#include "TargetInfo/PrototypeTargetInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Target/TargetOptions.h"
#include <optional>

using namespace llvm;

namespace llvm {
void initializePrototypeMemAluCombinerPass(PassRegistry &);
} // namespace llvm

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePrototypeTarget() {
  // Register the target.
  RegisterTargetMachine<PrototypeTargetMachine> registered_target(
      getThePrototypeTarget());
  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializePrototypeDAGToDAGISelPass(PR);
}

static std::string computeDataLayout() {
  // Data layout (keep in sync with clang/lib/Basic/Targets.cpp)
  return "E"        // Big endian
         "-m:e"     // ELF name manging
         "-p:32:32" // 32-bit pointers, 32 bit aligned
         "-i64:64"  // 64 bit integers, 64 bit aligned
         "-a:0:32"  // 32 bit alignment of objects of aggregate type
         "-n32"     // 32 bit native integer width
         "-S64";    // 64 bit natural stack alignment
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::PIC_);
}

PrototypeTargetMachine::PrototypeTargetMachine(
    const Target &T, const Triple &TT, StringRef Cpu, StringRef FeatureString,
    const TargetOptions &Options, std::optional<Reloc::Model> RM,
    std::optional<CodeModel::Model> CodeModel, CodeGenOpt::Level OptLevel,
    bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(), TT, Cpu, FeatureString, Options,
                        getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CodeModel, CodeModel::Medium),
                        OptLevel),
      Subtarget(TT, Cpu, FeatureString, *this, Options, getCodeModel(),
                OptLevel), TLOF(new PrototypeTargetObjectFile()), TLInfo(*this, Subtarget), FrameLowering(Subtarget) {
  initAsmInfo();
}

TargetTransformInfo
PrototypeTargetMachine::getTargetTransformInfo(const Function &F) const {
  return TargetTransformInfo(PrototypeTTIImpl(this, F));
}

MachineFunctionInfo *PrototypeTargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return PrototypeMachineFunctionInfo::create<PrototypeMachineFunctionInfo>(Allocator,
                                                                    F, STI);
}

namespace {
// Prototype Code Generator Pass Configuration Options.
class PrototypePassConfig : public TargetPassConfig {
public:
  PrototypePassConfig(PrototypeTargetMachine &TM, PassManagerBase *PassManager)
      : TargetPassConfig(TM, *PassManager) {}

  PrototypeTargetMachine &getPrototypeTargetMachine() const {
    return getTM<PrototypeTargetMachine>();
  }

  bool addInstSelector() override;
  void addPreSched2() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *
PrototypeTargetMachine::createPassConfig(PassManagerBase &PassManager) {
  return new PrototypePassConfig(*this, &PassManager);
}

// Install an instruction selector pass.
bool PrototypePassConfig::addInstSelector() {
  addPass(createPrototypeISelDag(getPrototypeTargetMachine()));
  return false;
}

// Implemented by targets that want to run passes immediately before
// machine code is emitted.
void PrototypePassConfig::addPreEmitPass() {
}

// Run passes after prolog-epilog insertion and before the second instruction
// scheduling pass.
void PrototypePassConfig::addPreSched2() {
}
