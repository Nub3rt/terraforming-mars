#include "noctis_farming.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
NoctisFarming::NoctisFarming( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::NOCTIS_FARMING, 10 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::PLANT );
}

NoctisFarming::~NoctisFarming() noexcept {}

bool NoctisFarming::SatisfiesRequirements() const {
    return _model.Temperature() >= -20;
}

void NoctisFarming::ApplyImmediateEffects() {
    _owner->GainResource( Resource::PLANTS, 2 );
    _owner->GainResourceProduction( Resource::CREDIT, 1 );
}

int NoctisFarming::DoCountVPs() const {
    return 1;
}
}
