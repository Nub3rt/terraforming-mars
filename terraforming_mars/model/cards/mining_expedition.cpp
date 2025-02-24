#include "mining_expedition.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
MiningExpedition::MiningExpedition( const GameModel& model ) noexcept :
    EventCard( model, CardID::MINING_EXPEDITION, 12, false, false ) {
    AddTag( Tag::EVENT );
}

MiningExpedition::~MiningExpedition() noexcept {}

void MiningExpedition::ApplyImmediateEffects() {
    _owner->RaiseOxygen();
    _owner->GainResource( Resource::STEEL, 2 );
    _owner->DestroyResource( Resource::PLANTS, 2 );
}
}
