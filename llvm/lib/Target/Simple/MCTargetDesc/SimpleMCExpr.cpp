//===-- SimpleMCExpr.cpp - Simple specific MC expression classes ------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "SimpleMCExpr.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCStreamer.h"
using namespace llvm;

#define DEBUG_TYPE "simplemcexpr"

const SimpleMCExpr *SimpleMCExpr::create(VariantKind Kind, const MCExpr *Expr,
                                       MCContext &Ctx) {
  return new (Ctx) SimpleMCExpr(Kind, Expr);
}

void SimpleMCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
  if (Kind == VK_Simple_None) {
    Expr->print(OS, MAI);
    return;
  }

  switch (Kind) {
  default:
    llvm_unreachable("Invalid kind!");
  case VK_Simple_ABS_HI:
    OS << "hi";
    break;
  case VK_Simple_ABS_LO:
    OS << "lo";
    break;
  }

  OS << '(';
  const MCExpr *Expr = getSubExpr();
  Expr->print(OS, MAI);
  OS << ')';
}

void SimpleMCExpr::visitUsedExpr(MCStreamer &Streamer) const {
  Streamer.visitUsedExpr(*getSubExpr());
}

bool SimpleMCExpr::evaluateAsRelocatableImpl(MCValue &Res,
                                            const MCAsmLayout *Layout,
                                            const MCFixup *Fixup) const {
  if (!getSubExpr()->evaluateAsRelocatable(Res, Layout, Fixup))
    return false;

  Res =
      MCValue::get(Res.getSymA(), Res.getSymB(), Res.getConstant(), getKind());

  return true;
}
