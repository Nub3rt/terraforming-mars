#include "view_state.h"

namespace view
{
#pragma region ViewState

ViewState::ViewState( View& view ) : _view( view ) {}
ViewState::~ViewState() {}

bool ViewState::CanHoverHand() { return false; }
bool ViewState::CanDragCardOut( CardWrapper& card ) { return false; }

void ViewState::PlayCard( CardWrapper& card ) {
}

#pragma endregion ViewState

#pragma region ResearchState

ResearchVState::ResearchVState( View& view ) : ViewState( view ) {}
ResearchVState::~ResearchVState() {}

#pragma endregion ResearchState

#pragma region IdleState

IdleVState::IdleVState( View& view ) : ViewState( view ) {}
IdleVState::~IdleVState() {}

bool IdleVState::CanHoverHand() { return true; }

bool IdleVState::CanDragCardOut( CardWrapper& card ) {
    return card->CanBePlayed();
}

void IdleVState::PlayCard( CardWrapper& card ) {
    _view._model->PlayCard( *card );

    ptrdiff_t index = &card - _view._hand.data();
    _view._hand.erase( _view._hand.begin() + index );
    _view.RefreshHandPositions();
}

#pragma endregion IdleState

#pragma region SellState

SellVState::SellVState( View& view ) : ViewState( view ) {}
SellVState::~SellVState() {}

#pragma endregion SellState

#pragma region PlacementConfirmationState

PlacementConfirmationVState::PlacementConfirmationVState( View& view ) : ViewState( view ) {}
PlacementConfirmationVState::~PlacementConfirmationVState() {}

bool PlacementConfirmationVState::CanHoverHand() {
    return false;
}

#pragma endregion PlacementConfirmationState

#pragma region PaymentConfirmationState

PaymentConfirmationVState::PaymentConfirmationVState( View& view ) : ViewState( view ) {}
PaymentConfirmationVState::~PaymentConfirmationVState() {}

#pragma endregion PaymentConfirmationState

#pragma region PostLastGenerationState

PostLastGenerationVState::PostLastGenerationVState( View& view ) : ViewState( view ) {}
PostLastGenerationVState::~PostLastGenerationVState() {}

#pragma endregion PostLastGenerationState

#pragma region GameOverState

GameOverVState::GameOverVState( View& view ) : ViewState( view ) {}
GameOverVState::~GameOverVState() {}

#pragma endregion GameOverState
}
