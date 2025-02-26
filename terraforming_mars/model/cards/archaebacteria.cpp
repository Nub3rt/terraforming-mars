#include "archaebacteria.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
Archaebacteria::Archaebacteria( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::ARCHAEBACTERIA, 6 ) {
    AddTag( Tag::MICROBE );
}

Archaebacteria::~Archaebacteria() noexcept {}

bool Archaebacteria::SatisfiesRequirements() const {
    return _model.Temperature() <= -18;
}

void Archaebacteria::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::PLANTS, 1 );
}
}
