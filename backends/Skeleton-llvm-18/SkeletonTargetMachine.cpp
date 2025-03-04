//===-- SkeletonTargetMachine.cpp - Define TargetMachine for Skeleton ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Implements the info about Skeleton target spec.
//
//===----------------------------------------------------------------------===//

#include "SkeletonTargetMachine.h"

#include "Skeleton.h"
#include "SkeletonMachineFunctionInfo.h"
#include "SkeletonTargetObjectFile.h"
#include "SkeletonTargetTransformInfo.h"
#include "TargetInfo/SkeletonTargetInfo.h"
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
void initializeSkeletonMemAluCombinerPass(PassRegistry &);
} // namespace llvm

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSkeletonTarget() {
  // Register the target.
  RegisterTargetMachine<SkeletonTargetMachine> registered_target(
      getTheSkeletonTarget());
  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeSkeletonDAGToDAGISelPass(PR);
}

static std::string computeDataLayout() {
  // Here you have to define the data layout for your target. Here is an example:
  // return "E"        // Big endian
  //        "-m:e"     // ELF name manging
  //        "-p:32:32" // 32-bit pointers, 32 bit aligned
  //        "-i64:64"  // 64 bit integers, 64 bit aligned
  //        "-a:0:32"  // 32 bit alignment of objects of aggregate type
  //        "-n32"     // 32 bit native integer width
  //        "-S64";    // 64 bit natural stack alignment
  // The Data layout has to be kept in sync with clang/lib/Basic/Targets.cpp
  return "";
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::PIC_);
}

SkeletonTargetMachine::SkeletonTargetMachine(
    const Target &T, const Triple &TT, StringRef Cpu, StringRef FeatureString,
    const TargetOptions &Options, std::optional<Reloc::Model> RM,
    std::optional<CodeModel::Model> CodeModel, CodeGenOptLevel OptLevel,
    bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(), TT, Cpu, FeatureString, Options,
                        getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CodeModel, CodeModel::Medium),
                        OptLevel),
      Subtarget(TT, Cpu, FeatureString, *this, Options, getCodeModel(),
                OptLevel), TLOF(new SkeletonTargetObjectFile()), TLInfo(*this, Subtarget), FrameLowering(Subtarget) {
  initAsmInfo();
}

TargetTransformInfo
SkeletonTargetMachine::getTargetTransformInfo(const Function &F) const {
  return TargetTransformInfo(SkeletonTTIImpl(this, F));
}

MachineFunctionInfo *SkeletonTargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return SkeletonMachineFunctionInfo::create<SkeletonMachineFunctionInfo>(Allocator,
                                                                    F, STI);
}

namespace {
// Skeleton Code Generator Pass Configuration Options.
class SkeletonPassConfig : public TargetPassConfig {
public:
  SkeletonPassConfig(SkeletonTargetMachine &TM, PassManagerBase *PassManager)
      : TargetPassConfig(TM, *PassManager) {}

  SkeletonTargetMachine &getSkeletonTargetMachine() const {
    return getTM<SkeletonTargetMachine>();
  }

  bool addInstSelector() override;
  void addPreSched2() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *
SkeletonTargetMachine::createPassConfig(PassManagerBase &PassManager) {
  return new SkeletonPassConfig(*this, &PassManager);
}

// Install an instruction selector pass.
bool SkeletonPassConfig::addInstSelector() {
  addPass(createSkeletonISelDag(getSkeletonTargetMachine()));
  return false;
}

// Implemented by targets that want to run passes immediately before
// machine code is emitted.
void SkeletonPassConfig::addPreEmitPass() {
}

// Run passes after prolog-epilog insertion and before the second instruction
// scheduling pass.
void SkeletonPassConfig::addPreSched2() {
}
