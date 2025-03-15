#include "zeppelins.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

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
