#pragma once

#include "card.fwd.h"
#include "player.fwd.h"
#include "game_model.fwd.h"

#include <array>
#include <string>
#include <functional>

#include "card_id.h"
#include "tag.h"

namespace model::decks
{
class Card
{
public:
    virtual ~Card() noexcept;

    virtual bool IsEvent() const noexcept;
    virtual bool IsAutomated() const noexcept;
    virtual bool IsActive() const noexcept;
    virtual bool IsActiveWithAction() const noexcept;
    virtual bool IsActiveWithEffect() const noexcept;

    inline virtual CardID get_card_id() const noexcept = 0;
    inline virtual const std::string& get_name() const noexcept = 0;

    void Buy( Player* player );
    void Sell();
    bool CanBePlayed( const GameModel& model ) const;
    void Play( const GameModel& model );
    int TagsOfType( Tag tag ) const;

protected:
    Card() noexcept;

    bool _is_building;
    bool _is_space;

    Player* _holder;
    Player* _owner;

    virtual int get_base_cost() const noexcept = 0;
    inline virtual int get_tag_count() const noexcept = 0;
    inline virtual const std::array<Tag, 3>& get_tags() const noexcept = 0;

    int GetCost() const;
    virtual bool SatisfiesRequirements( const GameModel& model ) const = 0;
    virtual void ApplyImmediateEffects() = 0;
};
}
