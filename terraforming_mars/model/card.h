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

    inline CardID get_card_id() const noexcept;
    inline const std::array<Tag, 3>& get_tags() const noexcept;
    inline int get_tag_count() const noexcept;
    inline bool get_is_building() const noexcept;
    inline bool get_is_space() const noexcept;

    void Buy( Player* player );
    void Sell();
    int GetCost() const;
    bool CanBePlayed() const;
    void Play();
    int TagsOfType( Tag tag ) const;
    int CountVPs() const;

protected:
    Card( const GameModel& model, CardID card_id, int base_cost, bool is_building, bool is_space ) noexcept;

    const CardID _card_id;
    const int _base_cost;
    const bool _is_building;
    const bool _is_space;

    const GameModel& _model;

    Player* _holder;
    Player* _owner;

    void AddTag( Tag tag );
    virtual bool SatisfiesRequirements() const;
    virtual void ApplyImmediateEffects();
    virtual int DoCountVPs() const;

private:
    std::array<Tag, 3> _tags;
    int _tag_count;
};
}
