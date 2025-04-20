#pragma once

#include <string>

#include "game_view.hpp"
#include "game_view_state.hpp"

namespace view
{
class SoloGameView : public GameView
{
public:
    SoloGameView();
    virtual ~SoloGameView();

protected:
    GameOverVState* CreateGameOverState() override;
};

class SoloGameOverVState : public GameOverVState
{
public:
    SoloGameOverVState( GameView& view );
    virtual ~SoloGameOverVState();

    void Render() override;

protected:
    bool _won;
    int _vps;

    std::string _vps_text;
};
}
