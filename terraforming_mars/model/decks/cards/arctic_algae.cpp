#include "arctic_algae.hpp"

#include "../active_card_with_effect.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
ArcticAlgae::ArcticAlgae( const GameModel& model ) noexcept :
    ActiveCardWithEffect( model, CardID::ARCTIC_ALGAE, 12 ) {
    AddTag( Tag::PLANT );
}

ArcticAlgae::~ArcticAlgae() noexcept {}

bool ArcticAlgae::SatisfiesRequirements() const {
    return _model.Temperature() <= -12;
}

void ArcticAlgae::ApplyImmediateEffects() {
    _owner->GainResource( Resource::PLANTS, 1 );
}

void ArcticAlgae::DoAfterAnyonePlacesOcean() {
    _owner->GainResource( Resource::PLANTS, 2 );
}
}
