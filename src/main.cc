// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#include <clang/Frontend/LogDiagnosticPrinter.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Serialization/PCHContainerOperations.h>
#include <clang/Tooling/ArgumentsAdjusters.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/VirtualFileSystem.h>
#include <llvm/Support/WithColor.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <memory>
#include <sstream>

#include "ccat/action_factory.hh"
#include "ccat/check_base.hh"
#include "ccat/context.hh"
#include "checkers/loop-reverse-unsigned-type-check.hh"
#include "utils.hh"

using clang::PCHContainerOperations;
using clang::tooling::ClangTool;
using clang::tooling::CommonOptionsParser;

static llvm::cl::OptionCategory ccat_category("ccat options");

void RegisterChecks() {
  ccat::CheckFactories::Instance().Factories.push_back(
      std::make_unique<ccat::LoopReverseUnsignedTypeCheckFactory>());
}

int main(int argc, const char *argv[]) {
  (void)argc;
  (void)argv;
  RegisterChecks();

  llvm::Expected<CommonOptionsParser> opts = CommonOptionsParser::create(
      argc, argv, ccat_category, llvm::cl::ZeroOrMore);
  if (!opts) {
    llvm::WithColor::error() << llvm::toString(opts.takeError());
    return 1;
  }

  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();

  ClangTool tool(opts->getCompilations(), opts->getSourcePathList());

  clang::tooling::ArgumentsAdjuster libclang_arguments_inserter =
      [](const clang::tooling::CommandLineArguments& Args,
         llvm::StringRef Filename) {
        (void)Filename;
        (void)Args;
        clang::tooling::CommandLineArguments AdjustedArgs = Args;

        auto I = AdjustedArgs.begin();
        if (I != AdjustedArgs.end() && !llvm::StringRef(*I).startswith("-")) {
          ++I;
        }

        std::ostringstream oss;
        oss << "-I" << ccat::ClangResourceDir << "/include";

        AdjustedArgs.insert(I, oss.str());

        return AdjustedArgs;
      };

  tool.appendArgumentsAdjuster(libclang_arguments_inserter);
  tool.appendArgumentsAdjuster(clang::tooling::getStripPluginsAdjuster());

  ccat::CCatContext ctx;
  std::error_code ec;
  llvm::raw_fd_ostream output("-", ec);
  clang::TextDiagnosticPrinter printer(output, new clang::DiagnosticOptions(),
                                       false);
  clang::DiagnosticsEngine diag_engine(new clang::DiagnosticIDs(),
                                       new clang::DiagnosticOptions(), &printer,
                                       false);
  ctx.DiagEngine = &diag_engine;

  ccat::ActionFactory factory(&ctx);
  return tool.run(&factory);
}
