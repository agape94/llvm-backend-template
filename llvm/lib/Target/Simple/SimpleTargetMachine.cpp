//===-- SimpleTargetMachine.cpp - Define TargetMachine for Simple ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Implements the info about Simple target spec.
//
//===----------------------------------------------------------------------===//

#include "SimpleTargetMachine.h"

#include "Simple.h"
#include "SimpleMachineFunctionInfo.h"
#include "SimpleTargetObjectFile.h"
#include "SimpleTargetTransformInfo.h"
#include "TargetInfo/SimpleTargetInfo.h"
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
void initializeSimpleMemAluCombinerPass(PassRegistry &);
} // namespace llvm

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSimpleTarget() {
  // Register the target.
  RegisterTargetMachine<SimpleTargetMachine> registered_target(
      getTheSimpleTarget());
  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeSimpleDAGToDAGISelPass(PR);
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

SimpleTargetMachine::SimpleTargetMachine(
    const Target &T, const Triple &TT, StringRef Cpu, StringRef FeatureString,
    const TargetOptions &Options, std::optional<Reloc::Model> RM,
    std::optional<CodeModel::Model> CodeModel, CodeGenOpt::Level OptLevel,
    bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(), TT, Cpu, FeatureString, Options,
                        getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CodeModel, CodeModel::Medium),
                        OptLevel),
      Subtarget(TT, Cpu, FeatureString, *this, Options, getCodeModel(),
                OptLevel), TLOF(new SimpleTargetObjectFile()), TLInfo(*this, Subtarget), FrameLowering(Subtarget) {
  initAsmInfo();
}

TargetTransformInfo
SimpleTargetMachine::getTargetTransformInfo(const Function &F) const {
  return TargetTransformInfo(SimpleTTIImpl(this, F));
}

MachineFunctionInfo *SimpleTargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return SimpleMachineFunctionInfo::create<SimpleMachineFunctionInfo>(Allocator,
                                                                    F, STI);
}

namespace {
// Simple Code Generator Pass Configuration Options.
class SimplePassConfig : public TargetPassConfig {
public:
  SimplePassConfig(SimpleTargetMachine &TM, PassManagerBase *PassManager)
      : TargetPassConfig(TM, *PassManager) {}

  SimpleTargetMachine &getSimpleTargetMachine() const {
    return getTM<SimpleTargetMachine>();
  }

  bool addInstSelector() override;
  void addPreSched2() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *
SimpleTargetMachine::createPassConfig(PassManagerBase &PassManager) {
  return new SimplePassConfig(*this, &PassManager);
}

// Install an instruction selector pass.
bool SimplePassConfig::addInstSelector() {
  addPass(createSimpleISelDag(getSimpleTargetMachine()));
  return false;
}

// Implemented by targets that want to run passes immediately before
// machine code is emitted.
void SimplePassConfig::addPreEmitPass() {
}

// Run passes after prolog-epilog insertion and before the second instruction
// scheduling pass.
void SimplePassConfig::addPreSched2() {
}
