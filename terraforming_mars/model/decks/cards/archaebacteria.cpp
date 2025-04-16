#include "archaebacteria.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
