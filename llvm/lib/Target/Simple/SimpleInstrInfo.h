//===- SimpleInstrInfo.h - Simple Instruction Information ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the Simple implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIMPLE_SIMPLEINSTRINFO_H
#define LLVM_LIB_TARGET_SIMPLE_SIMPLEINSTRINFO_H

#include "SimpleRegisterInfo.h"
#include "MCTargetDesc/SimpleMCTargetDesc.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "SimpleGenInstrInfo.inc"

namespace llvm {

class SimpleInstrInfo : public SimpleGenInstrInfo {
  const SimpleRegisterInfo RegisterInfo;

public:
  SimpleInstrInfo();

  // getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  // such, whenever a client has an instance of instruction info, it should
  // always be able to get register info as well (through this method).
  virtual const SimpleRegisterInfo &getRegisterInfo() const {
    return RegisterInfo;
  }

  bool areMemAccessesTriviallyDisjoint(const MachineInstr &MIa,
                                       const MachineInstr &MIb) const override;

  unsigned isLoadFromStackSlot(const MachineInstr &MI,
                               int &FrameIndex) const override;

  unsigned isLoadFromStackSlotPostFE(const MachineInstr &MI,
                                     int &FrameIndex) const override;

  unsigned isStoreToStackSlot(const MachineInstr &MI,
                              int &FrameIndex) const override;

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator Position,
                   const DebugLoc &DL, MCRegister DestinationRegister,
                   MCRegister SourceRegister, bool KillSource) const override;

  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator Position,
                           Register SourceRegister, bool IsKill, int FrameIndex,
                           const TargetRegisterClass *RegisterClass,
                           const TargetRegisterInfo *RegisterInfo,
                           Register VReg) const override;

  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator Position,
                            Register DestinationRegister, int FrameIndex,
                            const TargetRegisterClass *RegisterClass,
                            const TargetRegisterInfo *RegisterInfo,
                            Register VReg) const override;

  bool expandPostRAPseudo(MachineInstr &MI) const override;

  bool getMemOperandsWithOffsetWidth(
      const MachineInstr &LdSt,
      SmallVectorImpl<const MachineOperand *> &BaseOps, int64_t &Offset,
      bool &OffsetIsScalable, unsigned &Width,
      const TargetRegisterInfo *TRI) const override;

  bool getMemOperandWithOffsetWidth(const MachineInstr &LdSt,
                                    const MachineOperand *&BaseOp,
                                    int64_t &Offset, unsigned &Width,
                                    const TargetRegisterInfo *TRI) const;

  std::pair<unsigned, unsigned>
  decomposeMachineOperandsTargetFlags(unsigned TF) const override;

  ArrayRef<std::pair<unsigned, const char *>>
  getSerializableDirectMachineOperandTargetFlags() const override;

  bool analyzeBranch(MachineBasicBlock &MBB, MachineBasicBlock *&TrueBlock,
                     MachineBasicBlock *&FalseBlock,
                     SmallVectorImpl<MachineOperand> &Condition,
                     bool AllowModify) const override;

  unsigned removeBranch(MachineBasicBlock &MBB,
                        int *BytesRemoved = nullptr) const override;

  // For a comparison instruction, return the source registers in SrcReg and
  // SrcReg2 if having two register operands, and the value it compares against
  // in CmpValue. Return true if the comparison instruction can be analyzed.
  bool analyzeCompare(const MachineInstr &MI, Register &SrcReg,
                      Register &SrcReg2, int64_t &CmpMask,
                      int64_t &CmpValue) const override;

  // See if the comparison instruction can be converted into something more
  // efficient. E.g., on Simple register-register instructions can set the flag
  // register, obviating the need for a separate compare.
  bool optimizeCompareInstr(MachineInstr &CmpInstr, Register SrcReg,
                            Register SrcReg2, int64_t CmpMask, int64_t CmpValue,
                            const MachineRegisterInfo *MRI) const override;

  // Analyze the given select instruction, returning true if it cannot be
  // understood. It is assumed that MI->isSelect() is true.
  //
  // When successful, return the controlling condition and the operands that
  // determine the true and false result values.
  //
  //   Result = SELECT Cond, TrueOp, FalseOp
  //
  // Simple can optimize certain select instructions, for example by predicating
  // the instruction defining one of the operands and sets Optimizable to true.
  bool analyzeSelect(const MachineInstr &MI,
                     SmallVectorImpl<MachineOperand> &Cond, unsigned &TrueOp,
                     unsigned &FalseOp, bool &Optimizable) const override;

  // Given a select instruction that was understood by analyzeSelect and
  // returned Optimizable = true, attempt to optimize MI by merging it with one
  // of its operands. Returns NULL on failure.
  //
  // When successful, returns the new select instruction. The client is
  // responsible for deleting MI.
  //
  // If both sides of the select can be optimized, the TrueOp is modifed.
  // PreferFalse is not used.
  MachineInstr *optimizeSelect(MachineInstr &MI,
                               SmallPtrSetImpl<MachineInstr *> &SeenMIs,
                               bool PreferFalse) const override;

  bool reverseBranchCondition(
      SmallVectorImpl<MachineOperand> &Condition) const override;

  unsigned insertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TrueBlock,
                        MachineBasicBlock *FalseBlock,
                        ArrayRef<MachineOperand> Condition,
                        const DebugLoc &DL,
                        int *BytesAdded = nullptr) const override;
};

static inline bool isSPLSOpcode(unsigned Opcode) {
  switch (Opcode) {
  case Simple::LDBs_RI:
  case Simple::LDBz_RI:
  case Simple::LDHs_RI:
  case Simple::LDHz_RI:
  case Simple::STB_RI:
  case Simple::STH_RI:
    return true;
  default:
    return false;
  }
}

static inline bool isRMOpcode(unsigned Opcode) {
  switch (Opcode) {
  case Simple::LDW_RI:
  case Simple::SW_RI:
    return true;
  default:
    return false;
  }
}

static inline bool isRRMOpcode(unsigned Opcode) {
  switch (Opcode) {
  case Simple::LDBs_RR:
  case Simple::LDBz_RR:
  case Simple::LDHs_RR:
  case Simple::LDHz_RR:
  case Simple::LDWz_RR:
  case Simple::LDW_RR:
  case Simple::STB_RR:
  case Simple::STH_RR:
  case Simple::SW_RR:
    return true;
  default:
    return false;
  }
}

} // namespace llvm

#endif // LLVM_LIB_TARGET_SIMPLE_SIMPLEINSTRINFO_H
