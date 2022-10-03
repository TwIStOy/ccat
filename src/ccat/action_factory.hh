// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>

#include "ccat/context.hh"

namespace ccat {

class Action final : public clang::ASTFrontendAction {
 public:
  explicit Action(CCatContext *ctx);

  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
      clang::CompilerInstance& CI, llvm::StringRef InFile) final;

 private:
  CCatContext *ctx_;
};

class ActionFactory : public clang::tooling::FrontendActionFactory {
 public:
  explicit ActionFactory(CCatContext *ctx) : ctx_(ctx) {
  }

  std::unique_ptr<clang::FrontendAction> create();

 private:
  CCatContext *ctx_;
};

}  // namespace ccat
