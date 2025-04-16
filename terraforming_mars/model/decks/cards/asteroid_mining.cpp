#include "asteroid_mining.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
