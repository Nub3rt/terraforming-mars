#include "immigration_shuttles.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
ImmigrationShuttles::ImmigrationShuttles( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::IMMIGRATION_SHUTTLES, 31 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EARTH );
}

ImmigrationShuttles::~ImmigrationShuttles() noexcept {}

void ImmigrationShuttles::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::CREDIT, 5 );
}

int ImmigrationShuttles::DoCountVPs() const {
    return _model.CityCount() / 3;
}
}
