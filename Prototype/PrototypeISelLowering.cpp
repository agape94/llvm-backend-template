//===-- PrototypeISelLowering.cpp - Prototype DAG Lowering Implementation ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the PrototypeTargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "PrototypeISelLowering.h"
#include "Prototype.h"
#include "PrototypeCondCode.h"
#include "PrototypeMachineFunctionInfo.h"
#include "PrototypeSubtarget.h"
#include "PrototypeTargetObjectFile.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/MachineValueType.h"
#include "llvm/CodeGen/RuntimeLibcalls.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/CodeGen/TargetCallingConv.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/KnownBits.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <utility>
#include <iostream>

#define DEBUG_TYPE "prototype-lower"

using namespace llvm;

SDValue PrototypeTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const 
{
  return Chain;
}

SDValue PrototypeTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
    SelectionDAG &DAG) const  
{
  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  if (Outs.size() >= PROTOTYPE::GPRRegClass.getNumRegs())
  {
    assert(!"not implemented");
    return SDValue();
  }

  // Return is always ret 0
  RetOps.push_back(DAG.getConstant(0, DL, MVT::i32));

  // Build CopyToReg's
  for (unsigned i = 0; i != Outs.size(); ++i) 
  {
    if (Outs[i].VT.PrototypeTy != MVT::i32)
      assert(!"not implemented");

    unsigned loc_reg = PROTOTYPE::GPRRegClass.getRegister(i);

    Chain = DAG.getCopyToReg(Chain, DL, loc_reg,
    OutVals[i], Flag);

    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(loc_reg, Outs[i].VT));
  }

  RetOps[0] = Chain;  // Update chain.

  // Add the flag if we have it.
  if (Flag.getNode())
    RetOps.push_back(Flag);

  return DAG.getNode(PrototypeISD::RET, DL, MVT::Other,
    ArrayRef<SDValue>(&RetOps[0], RetOps.size()));
}

const char *PrototypeTargetLowering::getTargetNodeName(unsigned Opcode) const
{
  switch (Opcode)
  {
  case PrototypeISD::RET   : return "PROTOTYPE::RET";
  default               : return NULL;
  }
}

PrototypeTargetLowering::PrototypeTargetLowering(const TargetMachine &TM,
                                         const PrototypeSubtarget &STI)
    : TargetLowering(TM) 
{
  // Set up the register classes.
  addRegisterClass(MVT::i32, &PROTOTYPE::GPRRegClass);

  // Compute derived properties from the register classes
  TRI = STI.getRegisterInfo();
  computeRegisterProperties(TRI);

  setSchedulingPreference(Sched::Source);

  // Booleans always contain 0 or 1.
  setBooleanContents(ZeroOrOneBooleanContent);
}
