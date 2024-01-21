add_llvm_component_library(LLVMSimpleDisassembler
  SimpleDisassembler.cpp

  LINK_COMPONENTS
  CodeGenTypes
  SimpleDesc
  SimpleInfo
  MC
  MCDisassembler
  Support

  ADD_TO_COMPONENT
  Simple
  )
