#pragma once

#include "active_card.h"

namespace model::decks
{
class ActiveCardWithEffect : public ActiveCard
{
public:
    virtual ~ActiveCardWithEffect() noexcept;

    bool IsActiveWithEffect() const noexcept override;

    virtual void AfterAnyonePlacesCity();
    virtual void AfterAnyonePlacecOcean();
    virtual void AfterYouPlaySpaceEvent();
    virtual int ModifyCardCost( int cost );

protected:
    ActiveCardWithEffect() noexcept;

};
}
