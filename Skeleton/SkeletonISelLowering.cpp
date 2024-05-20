//===-- SkeletonISelLowering.cpp - Skeleton DAG Lowering Implementation ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the SkeletonTargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "SkeletonISelLowering.h"
#include "Skeleton.h"
#include "SkeletonCondCode.h"
#include "SkeletonMachineFunctionInfo.h"
#include "SkeletonSubtarget.h"
#include "SkeletonTargetObjectFile.h"
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

#define DEBUG_TYPE "skeleton-lower"

using namespace llvm;

SDValue SkeletonTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const 
{
  return Chain;
}

SDValue SkeletonTargetLowering::LowerFRAMEADDR(SDValue Op,
                                            SelectionDAG &DAG) const {
  MachineFrameInfo &MFI = DAG.getMachineFunction().getFrameInfo();
  MFI.setFrameAddressIsTaken(true);

  EVT VT = Op.getValueType();
  SDLoc DL(Op);
  SDValue FrameAddr = DAG.getCopyFromReg(DAG.getEntryNode(), DL, SKELETON::R31, VT);
  unsigned Depth = cast<ConstantSDNode>(Op.getOperand(0))->getZExtValue();
  while (Depth--) {
    // const unsigned Offset = -8;
    // SDValue Ptr = DAG.getNode(ISD::ADD, DL, VT, FrameAddr,
    //                           DAG.getIntPtrConstant(Offset, DL));
    FrameAddr =
        DAG.getLoad(VT, DL, DAG.getEntryNode(), FrameAddr, MachinePointerInfo());
  }
  return FrameAddr;
}

SDValue SkeletonTargetLowering::LowerGlobalAddress(SDValue Op,
                                                 SelectionDAG &DAG) const {
  const GlobalValue *GV = cast<GlobalAddressSDNode>(Op)->getGlobal();
  int64_t Offset = cast<GlobalAddressSDNode>(Op)->getOffset();
  EVT PtrVT = Op.getValueType();

  // Create the TargetGlobalAddress node, folding in the constant offset.
  SDValue Result = DAG.getTargetGlobalAddress(GV, SDLoc(Op), PtrVT, Offset);
  return DAG.getNode(SkeletonISD::Wrapper, SDLoc(Op), PtrVT, Result);
}


SDValue SkeletonTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
    SelectionDAG &DAG) const  
{
  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  if (Outs.size() >= SKELETON::GPRRegClass.getNumRegs())
  {
    assert(!"not implemented");
    return SDValue();
  }

  // Return is always ret 0
  RetOps.push_back(DAG.getConstant(0, DL, MVT::i32));

  // Build CopyToReg's
  for (unsigned i = 0; i != Outs.size(); ++i) 
  {
    if (Outs[i].VT.SimpleTy != MVT::i32)
      assert(!"not implemented");

    unsigned loc_reg = SKELETON::GPRRegClass.getRegister(i);

    Chain = DAG.getCopyToReg(Chain, DL, loc_reg,
    OutVals[i], Flag);

    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(loc_reg, Outs[i].VT));
  }

  RetOps[0] = Chain;  // Update chain.

  // Add the flag if we have it.
  if (Flag.getNode())
    RetOps.push_back(Flag);

  return DAG.getNode(SkeletonISD::RET, DL, MVT::Other,
    ArrayRef<SDValue>(&RetOps[0], RetOps.size()));
}

const char *SkeletonTargetLowering::getTargetNodeName(unsigned Opcode) const
{
  switch (Opcode)
  {
  case SkeletonISD::RET   : return "SKELETON::RET";
  default               : return NULL;
  }
}

SkeletonTargetLowering::SkeletonTargetLowering(const TargetMachine &TM,
                                         const SkeletonSubtarget &STI)
    : TargetLowering(TM) 
{
  // Set up the register classes.
  addRegisterClass(MVT::i32, &SKELETON::GPRRegClass);

  // Compute derived properties from the register classes
  TRI = STI.getRegisterInfo();
  computeRegisterProperties(TRI);

  setSchedulingPreference(Sched::Source);

  // Booleans always contain 0 or 1.
  setBooleanContents(ZeroOrOneBooleanContent);
}
