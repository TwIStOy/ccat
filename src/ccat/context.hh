// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

#include <clang/AST/ASTContext.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceManager.h>
#include <llvm/ADT/StringRef.h>

namespace ccat {

struct CCatContext {
  clang::SourceManager *SourceManager;
  llvm::StringRef File;
  clang::ASTContext *AstContext;
  clang::DiagnosticsEngine *DiagEngine;
  clang::DiagnosticConsumer *DiagConsumer;
};

}  // namespace ccat
