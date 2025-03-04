//===-- SkeletonELFObjectWriter.cpp - Skeleton ELF Writer -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {

class SkeletonELFObjectWriter : public MCELFObjectTargetWriter {
public:
  explicit SkeletonELFObjectWriter(uint8_t OSABI);

  ~SkeletonELFObjectWriter() override = default;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
  bool needsRelocateWithSymbol(const MCSymbol &SD,
                               unsigned Type) const override;
};

} // end anonymous namespace

SkeletonELFObjectWriter::SkeletonELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit_=*/false, OSABI, ELF::EM_SKELETON,
                              /*HasRelocationAddend_=*/true) {}

unsigned SkeletonELFObjectWriter::getRelocType(MCContext & /*Ctx*/,
                                            const MCValue & /*Target*/,
                                            const MCFixup &Fixup,
                                            bool /*IsPCRel*/) const {
  return 0;
}

bool SkeletonELFObjectWriter::needsRelocateWithSymbol(const MCSymbol & /*SD*/,
                                                   unsigned Type) const {
  return false;
}

std::unique_ptr<MCObjectTargetWriter> createSkeletonELFObjectWriter(uint8_t OSABI)
{
  return std::make_unique<SkeletonELFObjectWriter>(OSABI);
}
