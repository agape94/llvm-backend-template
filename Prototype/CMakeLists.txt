add_llvm_component_group(Prototype)

set(LLVM_TARGET_DEFINITIONS Prototype.td)

tablegen(LLVM PrototypeGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM PrototypeGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM PrototypeGenCallingConv.inc -gen-callingconv)
tablegen(LLVM PrototypeGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM PrototypeGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM PrototypeGenInstrInfo.inc -gen-instr-info)
# tablegen(LLVM PrototypeGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM PrototypeGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM PrototypeGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(PrototypeCommonTableGen)

add_llvm_target(PrototypeCodeGen
  PrototypeAsmPrinter.cpp
  PrototypeFrameLowering.cpp
  PrototypeInstrInfo.cpp
  PrototypeISelDAGToDAG.cpp
  PrototypeISelLowering.cpp
  PrototypeMachineFunctionInfo.cpp
  PrototypeMCInstLower.cpp
  PrototypeRegisterInfo.cpp
  PrototypeSelectionDAGInfo.cpp
  PrototypeSubtarget.cpp
  PrototypeTargetMachine.cpp
  PrototypeTargetObjectFile.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  PrototypeAsmParser
  PrototypeDesc
  PrototypeInfo
  MC
  SelectionDAG
  Support
  Target
  TargetParser
  TransformUtils

  ADD_TO_COMPONENT
  Prototype
  )

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
