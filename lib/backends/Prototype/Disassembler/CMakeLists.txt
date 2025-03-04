add_llvm_component_library(LLVMPrototypeDisassembler
  PrototypeDisassembler.cpp

  LINK_COMPONENTS
  CodeGenTypes
  PrototypeDesc
  PrototypeInfo
  MC
  MCDisassembler
  Support

  ADD_TO_COMPONENT
  Prototype
  )
