#include "artifical_lake.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
ArtificalLake::ArtificalLake( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::ARTIFICAL_LAKE, 15, true, false ) {
    AddTag( Tag::BUILDING );
}

ArtificalLake::~ArtificalLake() noexcept {}

bool ArtificalLake::SatisfiesRequirements() const {
    return _model.Temperature() >= -6 && _model.IsTilePlaceable();
}

void ArtificalLake::ApplyImmediateEffects() {
    _owner->PlaceOceanOnNonOcean();
}

int ArtificalLake::DoCountVPs() const {
    return 1;
}
}
