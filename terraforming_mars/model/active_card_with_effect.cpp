#include "active_card_with_effect.h"

#include "active_card.h"

namespace model::decks
{
ActiveCardWithEffect::ActiveCardWithEffect() noexcept : ActiveCard() {}
ActiveCardWithEffect::~ActiveCardWithEffect() noexcept {}

bool ActiveCardWithEffect::IsActiveWithEffect() const noexcept { return true; }

void ActiveCardWithEffect::AfterAnyonePlacesCity() {}
void ActiveCardWithEffect::AfterAnyonePlacecOcean() {}
void ActiveCardWithEffect::AfterYouPlaySpaceEvent() {}
int ActiveCardWithEffect::ModifyCardCost( int cost ) { return cost; }
}
