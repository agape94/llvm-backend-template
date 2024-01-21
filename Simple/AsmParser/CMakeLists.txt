include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_component_library(LLVMSimpleAsmParser
  SimpleAsmParser.cpp

  LINK_COMPONENTS
  CodeGenTypes
  SimpleDesc
  SimpleInfo
  MC
  MCParser
  Support

  ADD_TO_COMPONENT
  Simple
  )

add_dependencies( LLVMSimpleAsmParser SimpleCommonTableGen )
