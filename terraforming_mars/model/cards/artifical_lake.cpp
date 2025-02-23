#include "artifical_lake.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
ArtificalLake::ArtificalLake() noexcept :
    AutomatedCard( CardID::ARTIFICAL_LAKE, 15, true, false ) {
    AddTag( Tag::BUILDING );
}

ArtificalLake::~ArtificalLake() noexcept {}

bool ArtificalLake::SatisfiesRequirements( const GameModel& model ) const {
    return model.Temperature() >= -6 && model.IsTilePlaceable();
}

void ArtificalLake::ApplyImmediateEffects( const GameModel& model ) {
    _owner->PlaceOceanOnNonOcean();
}

int ArtificalLake::DoCountVPs( const GameModel& model ) const {
    return 1;
}
}
