// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Basic/SourceLocation.h>
#include <llvm/ADT/StringRef.h>

#include <list>
#include <memory>
#include <string_view>

#include "ccat/context.hh"

namespace ccat {

using clang::ast_matchers::MatchFinder;

class CheckBase : public MatchFinder::MatchCallback {
 public:
  virtual ~CheckBase() = default;

  void SetupContext(CCatContext *ctx);

  // return if this check is enabled or not with given context
  virtual bool Enabled(CCatContext *ctx) const = 0;

  // register matcher into finder
  virtual void RegisterMatcher(MatchFinder *finder) = 0;

 protected:
  CheckBase() = default;

  // emit when a registered matcher is matched
  virtual void OnMatched(const MatchFinder::MatchResult& result,
                         CCatContext *ctx) = 0;

  clang::DiagnosticBuilder Report(
      llvm::StringRef name, clang::SourceLocation loc, llvm::StringRef desc,
      clang::DiagnosticIDs::Level level = clang::DiagnosticIDs::Level::Warning);

 private:
  void run(const MatchFinder::MatchResult& Result) final;

 private:
  CCatContext *ctx_;
};

using CheckPtr = std::unique_ptr<CheckBase>;

struct CheckFactoryBase {
  virtual ~CheckFactoryBase() = default;

  // return check's name
  virtual std::string_view Name() const = 0;

  // construct a new check
  virtual CheckPtr CreateCheck() const = 0;
};

using CheckFactoryPtr = std::unique_ptr<CheckFactoryBase>;

struct CheckFactories {
  std::list<CheckFactoryPtr> Factories;

  static CheckFactories& Instance();
};

}  // namespace ccat
