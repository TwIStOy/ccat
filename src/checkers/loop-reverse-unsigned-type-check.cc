// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#include "checkers/loop-reverse-unsigned-type-check.hh"

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Lex/Lexer.h>
#include <iostream>

namespace ccat {

using namespace clang::ast_matchers;

bool LoopReverseUnsignedTypeCheck::Enabled(CCatContext *ctx) const {
  // 20, 2a
  // return ctx->CXXStandard().starts_with('2');
  (void)ctx;
  return true;
}

static const StatementMatcher incrementVarMatcher() {
  return declRefExpr(to(varDecl(equalsBoundNode("initVar"))));
}

void LoopReverseUnsignedTypeCheck::RegisterMatcher(MatchFinder *finder) {
  auto zero_literal = integerLiteral(equals(0));
  auto loop_init    = declStmt(hasSingleDecl(
      varDecl(hasType(isUnsignedInteger()),
                 hasInitializer(ignoringParenImpCasts(
                  cxxMemberCallExpr(callee(cxxMethodDecl(hasName("size")))))))
          .bind("initVar")));

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

  std::cout << "FUCK" << std::endl;
}

std::string_view LoopReverseUnsignedTypeCheckFactory::Name() const {
  return "loop-reverse-unsigned-type";
}

CheckPtr LoopReverseUnsignedTypeCheckFactory::CreateCheck() const {
  return std::make_unique<LoopReverseUnsignedTypeCheck>();
}

}  // namespace ccat
