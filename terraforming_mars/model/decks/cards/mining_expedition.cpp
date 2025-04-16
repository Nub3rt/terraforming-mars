#include "mining_expedition.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
MiningExpedition::MiningExpedition( const GameModel& model ) noexcept :
    EventCard( model, CardID::MINING_EXPEDITION, 12 ) {
    AddTag( Tag::EVENT );
}

MiningExpedition::~MiningExpedition() noexcept {}

void MiningExpedition::ApplyImmediateEffects() {
    _owner->RaiseOxygen();
    _owner->GainResource( Resource::STEEL, 2 );
    _owner->DestroyResource( Resource::PLANTS, 2 );
}
}
