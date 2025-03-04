add_llvm_component_group(Skeleton)

set(LLVM_TARGET_DEFINITIONS Skeleton.td)

tablegen(LLVM SkeletonGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM SkeletonGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM SkeletonGenCallingConv.inc -gen-callingconv)
tablegen(LLVM SkeletonGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM SkeletonGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM SkeletonGenInstrInfo.inc -gen-instr-info)
# tablegen(LLVM SkeletonGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM SkeletonGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM SkeletonGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(SkeletonCommonTableGen)

add_llvm_target(SkeletonCodeGen
  SkeletonAsmPrinter.cpp
  SkeletonFrameLowering.cpp
  SkeletonInstrInfo.cpp
  SkeletonISelDAGToDAG.cpp
  SkeletonISelLowering.cpp
  SkeletonMachineFunctionInfo.cpp
  SkeletonMCInstLower.cpp
  SkeletonRegisterInfo.cpp
  SkeletonSelectionDAGInfo.cpp
  SkeletonSubtarget.cpp
  SkeletonTargetMachine.cpp
  SkeletonTargetObjectFile.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  SkeletonAsmParser
  SkeletonDesc
  SkeletonInfo
  MC
  SelectionDAG
  Support
  Target
  TargetParser
  TransformUtils

  ADD_TO_COMPONENT
  Skeleton
  )

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
