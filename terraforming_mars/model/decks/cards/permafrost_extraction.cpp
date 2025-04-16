#include "permafrost_extraction.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
PermafrostExtraction::PermafrostExtraction( const GameModel& model ) noexcept :
    EventCard( model, CardID::PERMAFROST_EXTRACTION, 8 ) {
    AddTag( Tag::EVENT );
}

PermafrostExtraction::~PermafrostExtraction() noexcept {}

bool PermafrostExtraction::SatisfiesRequirements() const {
    return _model.Temperature() >= -8;
}

void PermafrostExtraction::ApplyImmediateEffects() {
    _owner->PlaceOcean();
}
}
