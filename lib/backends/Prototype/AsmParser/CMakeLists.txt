include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_component_library(LLVMPrototypeAsmParser
  PrototypeAsmParser.cpp

  LINK_COMPONENTS
  CodeGenTypes
  PrototypeDesc
  PrototypeInfo
  MC
  MCParser
  Support

  ADD_TO_COMPONENT
  Prototype
  )

add_dependencies( LLVMPrototypeAsmParser PrototypeCommonTableGen )
