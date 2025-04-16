#include "colonizer_training_camp.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
ColonizerTrainingCamp::ColonizerTrainingCamp( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::COLONIZER_TRAINING_CAMP, 8 ) {
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
