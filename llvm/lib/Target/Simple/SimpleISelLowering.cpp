//===-- SimpleISelLowering.cpp - Simple DAG Lowering Implementation ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the SimpleTargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "SimpleISelLowering.h"
#include "Simple.h"
#include "SimpleCondCode.h"
#include "SimpleMachineFunctionInfo.h"
#include "SimpleSubtarget.h"
#include "SimpleTargetObjectFile.h"
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

#define DEBUG_TYPE "simple-lower"

using namespace llvm;

SDValue SimpleTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  // Implement really simple calling convention
  // Every argument is passed through register
  // If not enough registers - just fail

  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();

  if (Ins.size() >= Simple::GPRRegClass.getNumRegs())
  {
    assert(!"not implemented");
    return SDValue();
  }

  SmallVector<SDValue, 4> ArgChains;

  // Emit CopyFromReg's
  for (unsigned i = 0, e = Ins.size(); i != e; ++i) 
  {
    if (Ins[i].VT.SimpleTy != MVT::i32)
      assert(!"not implemented");

    Register VReg = RegInfo.createVirtualRegister(&Simple::GPRRegClass);
    RegInfo.addLiveIn(Simple::GPRRegClass.getRegister(i), VReg);
    SDValue ArgIn = DAG.getCopyFromReg(Chain, DL, VReg, Ins[i].ArgVT);
    InVals.push_back(ArgIn);
    ArgChains.push_back(ArgIn.getValue(ArgIn->getNumValues() - 1));
  }

  // Return chain
  return DAG.getMergeValues(ArgChains, DL);

}

SDValue SimpleTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
    SelectionDAG &DAG) const  
{
  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  if (Outs.size() >= Simple::GPRRegClass.getNumRegs())
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

    unsigned loc_reg = Simple::GPRRegClass.getRegister(i);

    Chain = DAG.getCopyToReg(Chain, DL, loc_reg,
      OutVals[i], Flag);

    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(loc_reg, Outs[i].VT));
  }

  RetOps[0] = Chain;  // Update chain.

  // Add the flag if we have it.
  if (Flag.getNode())
    RetOps.push_back(Flag);

  // return chain
  return DAG.getMergeValues(RetOps, DL);
}

const char *SimpleTargetLowering::getTargetNodeName(unsigned Opcode) const
{
  switch (Opcode)
  {
  case SimpleISD::RET : return "Simple::RET";
  default             : return NULL;
  }
}

SimpleTargetLowering::SimpleTargetLowering(const TargetMachine &TM,
                                         const SimpleSubtarget &STI)
    : TargetLowering(TM) {
  // Set up the register classes.
  addRegisterClass(MVT::i32, &Simple::GPRRegClass);

  // Compute derived properties from the register classes
  TRI = STI.getRegisterInfo();
  computeRegisterProperties(TRI);

  setSchedulingPreference(Sched::Source);

  // Booleans always contain 0 or 1.
  setBooleanContents(ZeroOrOneBooleanContent);
}
