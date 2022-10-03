// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/MultiplexConsumer.h>
#include <clang/Frontend/PrecompiledPreamble.h>
#include <memory>
#include <vector>
#include "ccat/check_base.hh"

namespace ccat {

struct ASTConsumer : public clang::MultiplexConsumer {
  inline ASTConsumer(std::vector<std::unique_ptr<clang::ASTConsumer>> consumers,
                     std::unique_ptr<clang::ast_matchers::MatchFinder> finder,
                     std::vector<CheckPtr> checks)
      : MultiplexConsumer(std::move(consumers)),
        finder_(std::move(finder)),
        checks_(std::move(checks)) {
  }

 private:
  std::unique_ptr<clang::ast_matchers::MatchFinder> finder_;
  std::vector<CheckPtr> checks_;
};

}  // namespace ccat
