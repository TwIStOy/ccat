// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#include "checkers/loop-reverse-unsigned-type-check.hh"

#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Lex/Lexer.h>
#include <iostream>
#include <string_view>

namespace ccat {

using namespace clang::ast_matchers;

static constexpr std::string_view CheckName = "loop-reverse-unsigned-type";

bool LoopReverseUnsignedTypeCheck::Enabled(CCatContext *ctx) const {
  // 20, 2a
  // return ctx->CXXStandard().starts_with('2');
  (void)ctx;
  return true;
}

static auto InitVar = "initVar";

static auto kLiteralZero = integerLiteral(equals(0));
static auto kLiteralOne  = integerLiteral(equals(1));

static const StatementMatcher incrementVarMatcher() {
  return declRefExpr(to(varDecl(equalsBoundNode(InitVar))));
}

static auto member_function(const char *name) {
  return cxxMemberCallExpr(callee(cxxMethodDecl(hasName(name))));
}

void LoopReverseUnsignedTypeCheck::RegisterMatcher(MatchFinder *finder) {
  auto size_minus_one =
      binaryOperation(hasOperatorName("-"), hasLHS(member_function("size")),
                      hasRHS(ignoringParenImpCasts(kLiteralOne)));

  auto loop_init = declStmt(hasSingleDecl(
      varDecl(hasType(isUnsignedInteger()), hasInitializer(size_minus_one))
          .bind(InitVar)));

  auto condition =
      binaryOperator(hasOperatorName(">="),
                     hasRHS(ignoringParenImpCasts(integerLiteral(equals(0)))));

  auto increment = unaryOperator(hasOperatorName("--"),
                                 hasUnaryOperand(incrementVarMatcher()));

  auto my_matcher = forStmt(hasLoopInit(loop_init), hasCondition(condition),
                            hasIncrement(increment));

  finder->addMatcher(my_matcher, this);
}

void LoopReverseUnsignedTypeCheck::OnMatched(
    const MatchFinder::MatchResult& result, CCatContext *ctx) {
  (void)result;
  (void)ctx;

  const auto init_var = result.Nodes.getNodeAs<clang::ValueDecl>(InitVar);

  Report(CheckName, init_var->getLocation(),
         "call signed version 'ssize' instead");
}

std::string_view LoopReverseUnsignedTypeCheckFactory::Name() const {
  return CheckName;
}

CheckPtr LoopReverseUnsignedTypeCheckFactory::CreateCheck() const {
  return std::make_unique<LoopReverseUnsignedTypeCheck>();
}

}  // namespace ccat
