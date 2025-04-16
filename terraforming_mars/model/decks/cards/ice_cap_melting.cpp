#include "ice_cap_melting.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
