#include "pch.hpp"

#include "../model/decks/card.hpp"
#include "../model/decks/cards/_cards.hpp"

using namespace model;
using namespace model::decks;

namespace test
{
class CustomDeckProvider : public DeckProvider
{
public:
    std::vector<Card*> cards;

    ~CustomDeckProvider() {
        for ( Card* card : cards )
            delete card;
    }

    void AddCard( Card* card ) { cards.push_back( card ); }

    std::vector<Card*> GenerateDeck( const GameModel& ) override {
        std::vector<Card*> c = cards;
        cards.clear();
        return c;
    }
};

class DeckTest : public testing::Test
{
public:
    SoloGameModel model;
    CustomDeckProvider deck_provider;
    Card* card_1 = new cards::Comet( model );
    Card* card_2 = new cards::Bushes( model );

    DeckTest() : model( 3 ) {}
    ~DeckTest() {
        delete card_1;
        delete card_2;
    }
};

TEST_F( DeckTest, BaseFunctionalityTest ) {
    deck_provider.AddCard( card_1 );
    deck_provider.AddCard( card_2 );
    Deck deck( model, deck_provider, 5 );

    std::set<Card*> expected{ card_1, card_2 };
    std::set<Card*> actual;
    actual.insert( deck.DrawCard() );
    actual.insert( deck.DrawCard() );
    ASSERT_EQ( nullptr, deck.DrawCard() );
    ASSERT_EQ( expected, actual );

    deck.DiscardCard( card_1 );
    ASSERT_EQ( card_1, deck.DrawCard() );
}

TEST_F( DeckTest, ReshuffleOnlyOnEmptyTest ) {
    for ( int i = 0; i < 100; ++i ) {
        deck_provider.AddCard( card_1 );
        deck_provider.AddCard( card_2 );
        Deck deck( model, deck_provider, 5 );

        Card* first = deck.DrawCard();
        Card* second = first == card_1 ? card_2 : card_1;

        deck.DiscardCard( first );
        EXPECT_EQ( second, deck.DrawCard() );
        EXPECT_EQ( first, deck.DrawCard() );
        EXPECT_EQ( nullptr, deck.DrawCard() );
    }
}
}
