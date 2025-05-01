#include "solo_game_view.hpp"

#include "constants.hpp"
#include "text_renderer.hpp"

#include "../model/constants.hpp"

namespace view
{
SoloGameView::SoloGameView( int seed ) : GameView( seed ) {}
SoloGameView::~SoloGameView() {}

GameOverVState* SoloGameView::CreateGameOverState() {
    return new SoloGameOverVState( *this );
}


SoloGameOverVState::SoloGameOverVState( GameView& view ) : GameOverVState( view ) {
    _won = _view._model->AreGlobalParametersFulfilled();

    _vps = _view._model->GetLocalPlayerVPs();

    _vps_text = std::format( "Final Victory Points: {}", _vps );
}

SoloGameOverVState::~SoloGameOverVState() {}

void SoloGameOverVState::Render() {
    if ( _won )
        TextRenderer::RenderTextCentered(
            "Congratulations, you won!",
            0.0f, 0.2f,
            BASE_TEXT_SCALE,
            LIGHT_TEXT_COLOR
        );
    else
        TextRenderer::RenderTextCentered(
            "You could not terraform Mars in 14 generations...",
            0.0f, 0.2f,
            BASE_TEXT_SCALE,
            LIGHT_TEXT_COLOR
        );

    TextRenderer::RenderTextCentered(
        _vps_text,
        0.0f, -0.2f,
        BASE_TEXT_SCALE,
        LIGHT_TEXT_COLOR
    );
}
}
