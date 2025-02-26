#include "ice_cap_melting.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
IceCapMelting::IceCapMelting( const GameModel& model ) noexcept :
    EventCard( model, CardID::ICE_CAP_MELTING, 5 ) {
    AddTag( Tag::EVENT );
}

IceCapMelting::~IceCapMelting() noexcept {}

bool IceCapMelting::SatisfiesRequirements() const {
    return _model.Temperature() >= 2;
}

void IceCapMelting::ApplyImmediateEffects() {
    _owner->PlaceOcean();
}
}
