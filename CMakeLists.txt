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
          LLVMOption
          LLVMSupport)

target_compile_options(ccat PRIVATE ${PEDANTIC_COMPILE_FLAGS} ${WERROR_FLAG})

target_include_directories(ccat PRIVATE src/)
target_include_directories(ccat SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
target_include_directories(ccat SYSTEM PRIVATE ${CLANG_INCLUDE_DIRS})

target_sources(
  ccat
  PRIVATE ./src/main.cc ./src/ccat/action_factory.cc ./src/ccat/check_base.cc
          ./src/checkers/loop-reverse-unsigned-type-check.cc)
