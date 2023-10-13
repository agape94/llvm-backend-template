add_llvm_component_library(LLVMSimpleDesc
  SimpleAsmBackend.cpp
  SimpleELFObjectWriter.cpp
  SimpleInstPrinter.cpp
  SimpleMCAsmInfo.cpp
  SimpleMCCodeEmitter.cpp
  SimpleMCExpr.cpp
  SimpleMCTargetDesc.cpp

  LINK_COMPONENTS
  SimpleInfo
  MC
  MCDisassembler
  Support
  TargetParser

  ADD_TO_COMPONENT
  Simple
)
