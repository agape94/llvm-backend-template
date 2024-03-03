add_llvm_component_library(LLVMPrototypeDesc
  PrototypeAsmBackend.cpp
  PrototypeELFObjectWriter.cpp
  PrototypeInstPrinter.cpp
  PrototypeMCAsmInfo.cpp
  # PrototypeMCCodeEmitter.cpp
  PrototypeMCExpr.cpp
  PrototypeMCTargetDesc.cpp

  LINK_COMPONENTS
  PrototypeInfo
  MC
  MCDisassembler
  Support
  TargetParser

  ADD_TO_COMPONENT
  Prototype
)
