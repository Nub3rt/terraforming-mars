#include "arctic_algae.h"

#include "../active_card_with_effect.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
ArcticAlgae::ArcticAlgae( const GameModel& model ) noexcept :
    ActiveCardWithEffect( model, CardID::ARCTIC_ALGAE, 12, false, false ) {
    AddTag( Tag::PLANT );
}

ArcticAlgae::~ArcticAlgae() noexcept {}

bool ArcticAlgae::SatisfiesRequirements() const {
    return _model.Temperature() <= -12;
}

void ArcticAlgae::ApplyImmediateEffects() {
    _owner->GainPlants( 1 );
}

void ArcticAlgae::DoAfterAnyonePlacesOcean() {
    _owner->GainPlants( 2 );
}
}
