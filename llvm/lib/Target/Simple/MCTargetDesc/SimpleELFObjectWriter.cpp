//===-- SimpleELFObjectWriter.cpp - Simple ELF Writer -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/SimpleBaseInfo.h"
#include "MCTargetDesc/SimpleFixupKinds.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {

class SimpleELFObjectWriter : public MCELFObjectTargetWriter {
public:
  explicit SimpleELFObjectWriter(uint8_t OSABI);

  ~SimpleELFObjectWriter() override = default;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
  bool needsRelocateWithSymbol(const MCSymbol &SD,
                               unsigned Type) const override;
};

} // end anonymous namespace

SimpleELFObjectWriter::SimpleELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit_=*/false, OSABI, ELF::EM_SIMPLE,
                              /*HasRelocationAddend_=*/true) {}

unsigned SimpleELFObjectWriter::getRelocType(MCContext & /*Ctx*/,
                                            const MCValue & /*Target*/,
                                            const MCFixup &Fixup,
                                            bool /*IsPCRel*/) const {
  unsigned Type;
  unsigned Kind = static_cast<unsigned>(Fixup.getKind());
  switch (Kind) {
  case Simple::FIXUP_SIMPLE_21:
    Type = ELF::R_SIMPLE_21;
    break;
  case Simple::FIXUP_SIMPLE_21_F:
    Type = ELF::R_SIMPLE_21_F;
    break;
  case Simple::FIXUP_SIMPLE_25:
    Type = ELF::R_SIMPLE_25;
    break;
  case Simple::FIXUP_SIMPLE_32:
  case FK_Data_4:
    Type = ELF::R_SIMPLE_32;
    break;
  case Simple::FIXUP_SIMPLE_HI16:
    Type = ELF::R_SIMPLE_HI16;
    break;
  case Simple::FIXUP_SIMPLE_LO16:
    Type = ELF::R_SIMPLE_LO16;
    break;
  case Simple::FIXUP_SIMPLE_NONE:
    Type = ELF::R_SIMPLE_NONE;
    break;

  default:
    llvm_unreachable("Invalid fixup kind!");
  }
  return Type;
}

bool SimpleELFObjectWriter::needsRelocateWithSymbol(const MCSymbol & /*SD*/,
                                                   unsigned Type) const {
  switch (Type) {
  case ELF::R_SIMPLE_21:
  case ELF::R_SIMPLE_21_F:
  case ELF::R_SIMPLE_25:
  case ELF::R_SIMPLE_32:
  case ELF::R_SIMPLE_HI16:
    return true;
  default:
    return false;
  }
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createSimpleELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<SimpleELFObjectWriter>(OSABI);
}
