#include "artifical_lake.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
ArtificalLake::ArtificalLake( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::ARTIFICAL_LAKE, 15 ) {
    AddTag( Tag::BUILDING );
}

ArtificalLake::~ArtificalLake() noexcept {}

bool ArtificalLake::SatisfiesRequirements() const {
    return _model.Temperature() >= -6 && _model.IsTilePlaceable( _holder );
}

void ArtificalLake::ApplyImmediateEffects() {
    _owner->PlaceOceanOnNonOcean();
}

int ArtificalLake::DoCountVPs() const {
    return 1;
}
}
