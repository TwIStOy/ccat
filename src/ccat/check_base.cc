// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#include "ccat/check_base.hh"

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

}  // namespace ccat
