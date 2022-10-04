// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#include "ccat/check_base.hh"
#include <sstream>

namespace ccat {

void CheckBase::SetupContext(CCatContext *ctx) {
  ctx_ = ctx;
}

void CheckBase::run(const MatchFinder::MatchResult& result) {
  this->OnMatched(result, ctx_);
}

CheckFactories& CheckFactories::Instance() {
  static CheckFactories instance;
  return instance;
}

clang::DiagnosticBuilder CheckBase::Report(llvm::StringRef name,
                                           clang::SourceLocation loc,
                                           llvm::StringRef desc,
                                           clang::DiagnosticIDs::Level level) {
  auto id = ctx_->DiagEngine->getDiagnosticIDs()->getCustomDiagID(
      level, (desc + " [" + name + "]").str());

  return ctx_->DiagEngine->Report(loc, id);
}

}  // namespace ccat
