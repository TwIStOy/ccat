// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#include <string_view>

#include "ccat/check_base.hh"

namespace ccat {

class MakeUniqueReplaceNewCheck final : public CheckBase {
 public:
  bool Enabled(CCatContext *ctx) const final;

  void RegisterMatcher(MatchFinder *finder) final;

 private:
  void OnMatched(const MatchFinder::MatchResult& result,
                 CCatContext *ctx) final;
};

struct MakeUniqueReplaceNewCheckFactory final : public CheckFactoryBase {
  std::string_view Name() const final;

  CheckPtr CreateCheck() const final;
};

}  // namespace ccat
