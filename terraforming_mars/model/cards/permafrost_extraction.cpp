#include "permafrost_extraction.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
PermafrostExtraction::PermafrostExtraction( const GameModel& model ) noexcept :
    EventCard( model, CardID::PERMAFROST_EXTRACTION, 8, false, false ) {
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
