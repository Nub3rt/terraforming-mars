#include "windmills.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Windmills::Windmills( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::WINDMILLS, 6 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

Windmills::~Windmills() noexcept {}

bool Windmills::SatisfiesRequirements() const {
    return _model.Oxygen() >= 7;
}

void Windmills::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 1 );
}

int Windmills::DoCountVPs() const {
    return 1;
}
}
