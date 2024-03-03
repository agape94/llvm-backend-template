//=====-- PrototypeMCAsmInfo.h - Prototype asm properties -----------*- C++ -*--====//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the PrototypeMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PROTOTYPE_MCTARGETDESC_PROTOTYPEMCASMINFO_H
#define LLVM_LIB_TARGET_PROTOTYPE_MCTARGETDESC_PROTOTYPEMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class PrototypeMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit PrototypeMCAsmInfo(const Triple &TheTriple,
                          const MCTargetOptions &Options);
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_PROTOTYPE_MCTARGETDESC_PROTOTYPEMCASMINFO_H
