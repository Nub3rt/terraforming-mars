#pragma once

#include "active_card.h"
#include "card_id.h"

namespace model::decks
{
class ActiveCardWithEffect : public ActiveCard
{
public:
    virtual ~ActiveCardWithEffect() noexcept;

    bool IsActiveWithEffect() const noexcept override;

    void AfterAnyonePlacesCity();
    void AfterAnyonePlacesOcean();
    void AfterYouPlaySpaceEvent();
    int ModifyCardCost( const Card* card, int cost );

protected:
    ActiveCardWithEffect( const GameModel& model, CardID card_id, int base_cost, bool is_building, bool is_space ) noexcept;

    virtual void DoAfterAnyonePlacesCity();
    virtual void DoAfterAnyonePlacesOcean();
    virtual void DoAfterYouPlaySpaceEvent();
    virtual int DoModifyCardCost( const Card* card, int cost );
};
}
