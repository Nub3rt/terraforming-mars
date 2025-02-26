#include "asteroid_mining.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
AsteroidMining::AsteroidMining( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::ASTEROID_MINING, 30 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::JOVIAN );
}

AsteroidMining::~AsteroidMining() noexcept {}

void AsteroidMining::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::TITANIUM, 2 );
}

int AsteroidMining::DoCountVPs() const {
    return 2;
}
}
