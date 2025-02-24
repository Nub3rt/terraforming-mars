#include "colonizer_training_camp.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
ColonizerTrainingCamp::ColonizerTrainingCamp( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::COLONIZER_TRAINING_CAMP, 8, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::JOVIAN );
}

ColonizerTrainingCamp::~ColonizerTrainingCamp() noexcept {}

bool ColonizerTrainingCamp::SatisfiesRequirements() const {
    return _model.Oxygen() <= 5;
}

void ColonizerTrainingCamp::ApplyImmediateEffects() {
}

int ColonizerTrainingCamp::DoCountVPs() const {
    return 2;
}
}
