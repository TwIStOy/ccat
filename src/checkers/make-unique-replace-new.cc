// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#include "checkers/make-unique-replace-new.hh"
#include <clang/AST/Decl.h>

#include <memory>
#include <string_view>

namespace ccat {

static constexpr std::string_view CheckName = "make-unique-replace-new";
static constexpr auto InitVar               = "initVar";

std::string_view MakeUniqueReplaceNewCheckFactory::Name() const {
  return CheckName;
}

CheckPtr MakeUniqueReplaceNewCheckFactory::CreateCheck() const {
  return std::make_unique<MakeUniqueReplaceNewCheck>();
}

bool MakeUniqueReplaceNewCheck::Enabled(CCatContext *ctx) const {
  (void)ctx;
  return true;
}

void MakeUniqueReplaceNewCheck::RegisterMatcher(MatchFinder *finder) {
  using namespace clang::ast_matchers;

  static const auto unique_ptr_decl =
      recordDecl(hasName("unique_ptr"), isInStdNamespace());
  static const auto unique_ptr_type = qualType(hasDeclaration(unique_ptr_decl));

  static const auto shared_ptr_decl =
      recordDecl(hasName("shared_ptr"), isInStdNamespace());
  static const auto shared_ptr_type = qualType(hasDeclaration(unique_ptr_decl));

  auto unique_ptr_var_decl =
      varDecl(hasType(unique_ptr_type),
              hasInitializer(cxxConstructExpr(hasAnyArgument(cxxNewExpr()))))
          .bind(InitVar);

  finder->addMatcher(unique_ptr_var_decl, this);
}

void MakeUniqueReplaceNewCheck::OnMatched(
    const MatchFinder::MatchResult& result, CCatContext *ctx) {
  (void)ctx;
  const auto unique_ptr_var_decl =
      result.Nodes.getNodeAs<clang::ValueDecl>(InitVar);

  Report(CheckName, unique_ptr_var_decl->getLocation(),
         "replace 'std::unique_ptr<T> foo(new T(...))' with 'auto foo = "
         "std::make_unique<T>(...)'");
}

}  // namespace ccat
