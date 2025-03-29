#include "card_wrapper.h"

namespace view
{
CardWrapper::CardWrapper( model::decks::Card* card ) : _card( card ) {}
CardWrapper::~CardWrapper() {}
}
