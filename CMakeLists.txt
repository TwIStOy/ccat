cmake_minimum_required(VERSION 3.8)

project(ccat LANGUAGES CXX C)

set(DEFAULT_CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ccat "")

set_property(TARGET ccat PROPERTY CXX_STANDARD 20)
set_property(TARGET ccat PROPERTY CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(PEDANTIC_COMPILE_FLAGS
      -pedantic-errors
      -Wall
      -Wextra
      -pedantic
      -Wold-style-cast
      -Wundef
      -Wredundant-decls
      -Wwrite-strings
      -Wpointer-arith
      -Wcast-qual
      -Wformat=2
      -Wmissing-include-dirs
      -Wcast-align
      -Wctor-dtor-privacy
      -Wdisabled-optimization
      -Winvalid-pch
      -Woverloaded-virtual
      -Wconversion
      -Wswitch-enum
      -Wundef
      -Wno-ctor-dtor-privacy
      -Wno-format-nonliteral)
  if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.6)
    set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS} -Wno-dangling-else
                               -Wno-unused-local-typedefs)
  endif()
  if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
    set(PEDANTIC_COMPILE_FLAGS
        ${PEDANTIC_COMPILE_FLAGS}
        -Wdouble-promotion
        -Wtrampolines
        -Wzero-as-null-pointer-constant
        -Wuseless-cast
        -Wvector-operation-performance
        -Wsized-deallocation
        -Wshadow)
  endif()
  if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
    set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS} -Wshift-overflow=2
                               -Wnull-dereference -Wduplicated-cond)
  endif()
  set(WERROR_FLAG -Werror)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(PEDANTIC_COMPILE_FLAGS
      -Wall
      -Wextra
      -pedantic
      -Wconversion
      -Wundef
      -Wdeprecated
      -Wweak-vtables
      -Wshadow
      -Wno-gnu-zero-variadic-macro-arguments)
  check_cxx_compiler_flag(-Wzero-as-null-pointer-constant HAS_NULLPTR_WARNING)
  if(HAS_NULLPTR_WARNING)
    set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS}
                               -Wzero-as-null-pointer-constant)
  endif()
  set(WERROR_FLAG -Werror)
endif()

if(MSVC)
  set(PEDANTIC_COMPILE_FLAGS /W3)
  set(WERROR_FLAG /WX)
endif()

find_package(Clang REQUIRED)

if(NOT CLANG_RESOURCE_DIR)
  find_program(CLANG_EXECUTABLE clang++)

  if(NOT CLANG_EXECUTABLE)
    message(FATAL_ERROR "clang executable not found.")
  endif()

  execute_process(
    COMMAND ${CLANG_EXECUTABLE} -print-resource-dir
    RESULT_VARIABLE CLANG_FIND_RESOURCE_DIR_RESULT
    OUTPUT_VARIABLE CLANG_RESOURCE_DIR
    ERROR_VARIABLE CLANG_FIND_RESOURCE_DIR_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  message(NOTICE "clang-resource-dir: ${CLANG_RESOURCE_DIR}")

  if(CLANG_FIND_RESOURCE_DIR_RESULT)
    message(FATAL_ERROR "Error retrieving Clang resource directory with Clang \
            executable. Output:\n ${CLANG_FIND_RESOURCE_DIR_ERROR}")
  endif()
endif()

target_link_libraries(
  ccat
  PRIVATE clangIndex
          clangFormat
          clangTooling
          clangToolingInclusions
          clangToolingCore
          clangFrontend
          clangParse
          clangSerialization
          clangSema
          clangAST
          clangASTMatchers
          clangLex
          clangDriver
          clangBasic
          clangHandleLLVM
          clangTidyLLVMLibcModule
          clangTidyLLVMModule
          LLVMAArch64AsmParser
          LLVMAArch64CodeGen
          LLVMAArch64Desc
          LLVMAArch64Disassembler
          LLVMAArch64Info
          LLVMAArch64Utils
          LLVMAggressiveInstCombine
          LLVMAMDGPUAsmParser
          LLVMAMDGPUCodeGen
          LLVMAMDGPUDesc
          LLVMAMDGPUDisassembler
          LLVMAMDGPUInfo
          LLVMAMDGPUTargetMCA
          LLVMAMDGPUUtils
          LLVMAnalysis
          LLVMARMAsmParser
          LLVMARMCodeGen
          LLVMARMDesc
          LLVMARMDisassembler
          LLVMARMInfo
          LLVMARMUtils
          LLVMAsmParser
          LLVMAsmPrinter
          LLVMAVRAsmParser
          LLVMAVRCodeGen
          LLVMAVRDesc
          LLVMAVRDisassembler
          LLVMAVRInfo
          LLVMBinaryFormat
          LLVMBitReader
          LLVMBitstreamReader
          LLVMBitWriter
          LLVMBPFAsmParser
          LLVMBPFCodeGen
          LLVMBPFDesc
          LLVMBPFDisassembler
          LLVMBPFInfo
          LLVMCFGuard
          LLVMCFIVerify
          LLVMCodeGen
          LLVMCore
          LLVMCoroutines
          LLVMCoverage
          LLVMDebugInfoCodeView
          LLVMDebuginfod
          LLVMDebugInfoDWARF
          LLVMDebugInfoGSYM
          LLVMDebugInfoMSF
          LLVMDebugInfoPDB
          LLVMDemangle
          LLVMDiff
          LLVMDlltoolDriver
          LLVMDWARFLinker
          LLVMDWP
          LLVMExecutionEngine
          LLVMExegesis
          LLVMExegesisAArch64
          LLVMExegesisMips
          LLVMExegesisPowerPC
          LLVMExegesisX86
          LLVMExtensions
          LLVMFileCheck
          LLVMFrontendOpenACC
          LLVMFrontendOpenMP
          LLVMFuzzerCLI
          LLVMFuzzMutate
          LLVMGlobalISel
          LLVMHexagonAsmParser
          LLVMHexagonCodeGen
          LLVMHexagonDesc
          LLVMHexagonDisassembler
          LLVMHexagonInfo
          LLVMInstCombine
          LLVMInstrumentation
          LLVMInterfaceStub
          LLVMInterpreter
          LLVMipo
          LLVMIRReader
          LLVMJITLink
          LLVMLanaiAsmParser
          LLVMLanaiCodeGen
          LLVMLanaiDesc
          LLVMLanaiDisassembler
          LLVMLanaiInfo
          LLVMLibDriver
          LLVMLinker
          LLVMLTO
          LLVMMC
          LLVMMCA
          LLVMMCDisassembler
          LLVMMCJIT
          LLVMMCParser
          LLVMMipsAsmParser
          LLVMMipsCodeGen
          LLVMMipsDesc
          LLVMMipsDisassembler
          LLVMMipsInfo
          LLVMMIRParser
          LLVMMSP430AsmParser
          LLVMMSP430CodeGen
          LLVMMSP430Desc
          LLVMMSP430Disassembler
          LLVMMSP430Info
          LLVMNVPTXCodeGen
          LLVMNVPTXDesc
          LLVMNVPTXInfo
          LLVMObjCARCOpts
          LLVMObjCopy
          LLVMObject
          LLVMObjectYAML
          LLVMOption
          LLVMOrcJIT
          LLVMOrcShared
          LLVMOrcTargetProcess
          LLVMPasses
          LLVMPowerPCAsmParser
          LLVMPowerPCCodeGen
          LLVMPowerPCDesc
          LLVMPowerPCDisassembler
          LLVMPowerPCInfo
          LLVMProfileData
          LLVMRemarks
          LLVMRISCVAsmParser
          LLVMRISCVCodeGen
          LLVMRISCVDesc
          LLVMRISCVDisassembler
          LLVMRISCVInfo
          LLVMRuntimeDyld
          LLVMScalarOpts
          LLVMSelectionDAG
          LLVMSparcAsmParser
          LLVMSparcCodeGen
          LLVMSparcDesc
          LLVMSparcDisassembler
          LLVMSparcInfo
          LLVMSupport
          LLVMSymbolize
          LLVMSystemZAsmParser
          LLVMSystemZCodeGen
          LLVMSystemZDesc
          LLVMSystemZDisassembler
          LLVMSystemZInfo
          LLVMTableGen
          LLVMTableGenGlobalISel
          LLVMTarget
          LLVMTextAPI
          LLVMTransformUtils
          LLVMVEAsmParser
          LLVMVECodeGen
          LLVMVectorize
          LLVMVEDesc
          LLVMVEDisassembler
          LLVMVEInfo
          LLVMWebAssemblyAsmParser
          LLVMWebAssemblyCodeGen
          LLVMWebAssemblyDesc
          LLVMWebAssemblyDisassembler
          LLVMWebAssemblyInfo
          LLVMWebAssemblyUtils
          LLVMWindowsDriver
          LLVMWindowsManifest
          LLVMX86AsmParser
          LLVMX86CodeGen
          LLVMX86Desc
          LLVMX86Disassembler
          LLVMX86Info
          LLVMX86TargetMCA
          LLVMXCoreCodeGen
          LLVMXCoreDesc
          LLVMXCoreDisassembler
          LLVMXCoreInfo
          LLVMXRay)

target_compile_options(ccat PRIVATE ${PEDANTIC_COMPILE_FLAGS} ${WERROR_FLAG})

target_include_directories(ccat PRIVATE src/)
target_include_directories(ccat SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
target_include_directories(ccat SYSTEM PRIVATE ${CLANG_INCLUDE_DIRS})

target_sources(
  ccat
  PRIVATE ./src/main.cc ./src/ccat/action_factory.cc ./src/ccat/check_base.cc
          ./src/utils.cc ./src/checkers/loop-reverse-unsigned-type-check.cc
          ./src/checkers/make-unique-replace-new.cc)

set_property(
  SOURCE ./src/utils.cc
  APPEND
  PROPERTY COMPILE_DEFINITIONS
           CLANG_RESOURCE_DIRECTORY=R"\(${CLANG_RESOURCE_DIR}\)")
