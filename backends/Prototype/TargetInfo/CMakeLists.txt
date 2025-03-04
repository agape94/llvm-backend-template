add_llvm_component_library(LLVMPrototypeInfo
  PrototypeTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  Prototype
  )
