// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#include "ccat/action_factory.hh"

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/PreprocessorOptions.h>

#include <memory>
#include <vector>

#include "ccat/ast_consumer.hh"
#include "ccat/check_base.hh"

namespace ccat {

Action::Action(CCatContext *ctx) : ctx_(ctx) {
}

std::unique_ptr<clang::ASTConsumer> Action::CreateASTConsumer(
    clang::CompilerInstance& CI, llvm::StringRef InFile) {
  ctx_->SourceManager = &CI.getSourceManager();
  ctx_->File          = InFile;
  ctx_->AstContext    = &CI.getASTContext();
  ctx_->DiagEngine->setSourceManager(ctx_->SourceManager);
  ctx_->DiagConsumer->BeginSourceFile(CI.getLangOpts(), &CI.getPreprocessor());

  clang::ast_matchers::MatchFinder::MatchFinderOptions finder_opts;
  auto finder = std::make_unique<clang::ast_matchers::MatchFinder>(finder_opts);

  std::vector<CheckPtr> checks;
  for (const auto& f : CheckFactories::Instance().Factories) {
    auto check = f->CreateCheck();
    check->RegisterMatcher(finder.get());
    check->SetupContext(ctx_);
    checks.push_back(std::move(check));
  }

  std::vector<std::unique_ptr<clang::ASTConsumer>> consumers;
  if (!checks.empty()) {
    consumers.push_back(finder->newASTConsumer());
  }

  return std::make_unique<ASTConsumer>(std::move(consumers), std::move(finder),
                                       std::move(checks));
}

bool ActionFactory::runInvocation(
    std::shared_ptr<clang::CompilerInvocation> Invocation,
    clang::FileManager *Files,
    std::shared_ptr<clang::PCHContainerOperations> PCHContainerOps,
    clang::DiagnosticConsumer *DiagConsumer) {
  Invocation->getPreprocessorOpts().SetUpStaticAnalyzer = true;
  return FrontendActionFactory::runInvocation(Invocation, Files,
                                              PCHContainerOps, DiagConsumer);
}

std::unique_ptr<clang::FrontendAction> ActionFactory::create() {
  return std::make_unique<Action>(ctx_);
}

}  // namespace ccat
