add_llvm_component_group(Simple)

set(LLVM_TARGET_DEFINITIONS Simple.td)

tablegen(LLVM SimpleGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM SimpleGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM SimpleGenCallingConv.inc -gen-callingconv)
tablegen(LLVM SimpleGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM SimpleGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM SimpleGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM SimpleGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM SimpleGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM SimpleGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(SimpleCommonTableGen)

add_llvm_target(SimpleCodeGen
  SimpleAsmPrinter.cpp
  SimpleFrameLowering.cpp
  SimpleInstrInfo.cpp
  SimpleISelDAGToDAG.cpp
  SimpleISelLowering.cpp
  SimpleMachineFunctionInfo.cpp
  SimpleMCInstLower.cpp
  SimpleRegisterInfo.cpp
  SimpleSelectionDAGInfo.cpp
  SimpleSubtarget.cpp
  SimpleTargetMachine.cpp
  SimpleTargetObjectFile.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  SimpleAsmParser
  SimpleDesc
  SimpleInfo
  MC
  SelectionDAG
  Support
  Target
  TargetParser
  TransformUtils

  ADD_TO_COMPONENT
  Simple
  )

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
