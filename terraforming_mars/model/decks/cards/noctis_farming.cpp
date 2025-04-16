#include "noctis_farming.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
