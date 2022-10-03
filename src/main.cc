// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#include <clang/Serialization/PCHContainerOperations.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/VirtualFileSystem.h>
#include <llvm/Support/WithColor.h>
#include <memory>

#include "ccat/action_factory.hh"
#include "ccat/check_base.hh"
#include "ccat/context.hh"
#include "checkers/loop-reverse-unsigned-type-check.hh"

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

  llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem> base_fs(
      new llvm::vfs::OverlayFileSystem(llvm::vfs::getRealFileSystem()));

  ClangTool tool(opts->getCompilations(), opts->getSourcePathList(),
                 std::make_shared<PCHContainerOperations>(), base_fs);

  ccat::CCatContext ctx;
  ccat::ActionFactory factory(&ctx);
  return tool.run(&factory);
}
