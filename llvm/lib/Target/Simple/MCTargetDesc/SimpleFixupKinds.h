//===-- SimpleFixupKinds.h - Simple Specific Fixup Entries --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIMPLE_MCTARGETDESC_SIMPLEFIXUPKINDS_H
#define LLVM_LIB_TARGET_SIMPLE_MCTARGETDESC_SIMPLEFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace Simple {
// Although most of the current fixup types reflect a unique relocation
// one can have multiple fixup types for a given relocation and thus need
// to be uniquely named.
//
// This table *must* be in the save order of
// MCFixupKindInfo Infos[Simple::NumTargetFixupKinds]
// in SimpleAsmBackend.cpp.
//
enum Fixups {
  // Results in R_Simple_NONE
  FIXUP_SIMPLE_NONE = FirstTargetFixupKind,

  FIXUP_SIMPLE_21,   // 21-bit symbol relocation
  FIXUP_SIMPLE_21_F, // 21-bit symbol relocation, last two bits masked to 0
  FIXUP_SIMPLE_25,   // 25-bit branch targets
  FIXUP_SIMPLE_32,   // general 32-bit relocation
  FIXUP_SIMPLE_HI16, // upper 16-bits of a symbolic relocation
  FIXUP_SIMPLE_LO16, // lower 16-bits of a symbolic relocation

  // Marker
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
} // namespace Simple
} // namespace llvm

#endif // LLVM_LIB_TARGET_SIMPLE_MCTARGETDESC_SIMPLEFIXUPKINDS_H
