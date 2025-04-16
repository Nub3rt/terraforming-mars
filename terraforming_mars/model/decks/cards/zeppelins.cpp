#include "zeppelins.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Zeppelins::Zeppelins( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::ZEPPELINS, 13 ) {}

Zeppelins::~Zeppelins() noexcept {}

bool Zeppelins::SatisfiesRequirements() const {
    return _model.Oxygen() >= 5;
}

void Zeppelins::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::CREDIT, _model.CityCount() );
}

int Zeppelins::DoCountVPs() const {
    return 1;
}
}
