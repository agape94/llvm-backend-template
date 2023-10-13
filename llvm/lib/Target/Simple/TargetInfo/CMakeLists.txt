add_llvm_component_library(LLVMSimpleInfo
  SimpleTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  Simple
  )
