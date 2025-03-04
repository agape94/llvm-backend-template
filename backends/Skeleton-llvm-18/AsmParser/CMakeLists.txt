include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_component_library(LLVMSkeletonAsmParser
  SkeletonAsmParser.cpp

  LINK_COMPONENTS
  CodeGenTypes
  SkeletonDesc
  SkeletonInfo
  MC
  MCParser
  Support

  ADD_TO_COMPONENT
  Skeleton
  )

add_dependencies( LLVMSkeletonAsmParser SkeletonCommonTableGen )
