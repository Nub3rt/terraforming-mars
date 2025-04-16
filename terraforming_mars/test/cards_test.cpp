#include "pch.hpp"

#include <string>

#include "../model/decks/card.hpp"
#include "../model/decks/cards/_cards.hpp"
#include "../model/decks/availability.hpp"

using namespace model;
using namespace model::decks;

namespace test
{
class CardsTest : public testing::Test
{
public:
    SoloGameModel model;
    Player player;
    std::string wrong_event_called = "";

    CardsTest() : model( 3 ), player( 20, 0 ) {
        SetAllCallback( [ this ]( std::string event_name ) { wrong_event_called = event_name; } );
    }

    void SetAllCallback( std::function<void( std::string )> callback ) {
        player.SetOnDrawCardCallback( [ callback ]( Player* ) { callback( "OnDrawCard" ); } );
        player.SetOnRaiseTRCallback( [ callback ]( Player*, int ) { callback( "OnRaiseTR" ); } );
        player.SetOnRaiseTemperatureCallback( [ callback ]( Player* ) { callback( "OnRaiseTemperature" ); } );
        player.SetOnPlaceOceanCallback( [ callback ]( Player* ) { callback( "OnPlaceOcean" ); } );
        player.SetOnPlaceOceanOnNonOceanCallback( [ callback ]( Player* ) { callback( "OnPlaceOceanOnNonOcean" ); } );
        player.SetOnRaiseOxygenCallback( [ callback ]( Player* ) { callback( "OnRaiseOxygen" ); } );
        player.SetOnPlaceGreeneryCallback( [ callback ]( Player* ) { callback( "OnPlaceGreenery" ); } );
        player.SetOnPlaceGreeneryOnOceanCallback( [ callback ]( Player* ) { callback( "OnPlaceGreeneryOnOcean" ); } );
        player.SetOnPlaceCityCallback( [ callback ]( Player* ) { callback( "OnPlaceCity" ); } );
        player.SetOnPlaceNoctisCityCallback( [ callback ]( Player* ) { callback( "OnPlaceNoctisCity" ); } );
        player.SetOnPlaceLonelyCityCallback( [ callback ]( Player* ) { callback( "OnPlaceLonelyCity" ); } );
        player.SetOnPlaceUrbanizedAreaCallback( [ callback ]( Player* ) { callback( "OnPlaceUrbanizedArea" ); } );
        player.SetOnResourceAmountChangedCallback( [ callback ]( Player*, Resource, int ) { callback( "OnResourceAmountChanged" ); } );
        player.SetOnResourceProductionAmountChangedCallback( [ callback ]( Player*, Resource, int ) { callback( "OnResourceProductionAmountChanged" ); } );
        player.SetOnDestroyResourceCallback( [ callback ]( Player*, Resource, int ) { callback( "OnDestroyResource" ); } );
        player.SetOnDestroyResourceProductionCallback( [ callback ]( Player*, Resource, int ) { callback( "OnDestroyResourceProduction" ); } );
        player.SetOnConfirmSteelPaymentCallback( [ callback ]( Player*, int, std::function<void()> ) { callback( "OnConfirmSteelPayment" ); } );
        player.SetOnConfirmTitaniumPaymentCallback( [ callback ]( Player*, int, std::function<void()> ) { callback( "OnConfirmTitaniumPayment" ); } );
    }
};

TEST_F( CardsTest, OwnerLogicTest ) {
    player.GainResource( Resource::CREDIT, 100 );
    cards::Comet* comet = new cards::Comet( model );

    EXPECT_THROW( { player.SellCard( comet ); }, std::logic_error ); // Sell
    EXPECT_THROW( { player.PlayCard( comet ); }, std::logic_error ); // Play

    player.GetCard( comet ); // Buy
    EXPECT_THROW( { player.GetCard( comet ); }, std::logic_error ); // Buy

    player.SellCard( comet ); // Sell
    EXPECT_THROW( { player.SellCard( comet ); }, std::logic_error ); // Sell
    EXPECT_THROW( { player.PlayCard( comet ); }, std::logic_error ); // Play

    player.GetCard( comet ); // Buy
    EXPECT_THROW( { player.GetCard( comet ); }, std::logic_error ); // Buy

    player.PlayCard( comet ); // Play
    EXPECT_THROW( { player.GetCard( comet ); }, std::logic_error ); // Buy
    EXPECT_THROW( { player.SellCard( comet ); }, std::logic_error ); // Sell
    EXPECT_THROW( { player.PlayCard( comet ); }, std::logic_error ); // Plays
}

TEST_F( CardsTest, EventTest ) {
    cards::Comet* comet = new cards::Comet( model );

    EXPECT_FALSE( comet->IsActive() );
    EXPECT_FALSE( comet->IsAutomated() );
    EXPECT_TRUE ( comet->IsEvent() );
    EXPECT_EQ( CardID::COMET, comet->get_card_id() );
    EXPECT_FALSE( comet->HasTag( Tag::BUILDING ) );
    EXPECT_TRUE ( comet->HasTag( Tag::SPACE ) );
    EXPECT_FALSE( comet->HasTag( Tag::POWER ) );
    EXPECT_FALSE( comet->HasTag( Tag::SCIENCE ) );
    EXPECT_FALSE( comet->HasTag( Tag::JOVIAN ) );
    EXPECT_FALSE( comet->HasTag( Tag::EARTH ) );
    EXPECT_FALSE( comet->HasTag( Tag::PLANT ) );
    EXPECT_FALSE( comet->HasTag( Tag::MICROBE ) );
    EXPECT_FALSE( comet->HasTag( Tag::ANIMAL ) );
    EXPECT_FALSE( comet->HasTag( Tag::CITY ) );
    EXPECT_TRUE ( comet->HasTag( Tag::EVENT ) );

    int resource_count = 0;
    int temperature_count = 0;
    int destroy_count = 0;
    int ocean_count = 0;

    player.SetOnResourceAmountChangedCallback( [ &resource_count ]( Player*, Resource resource, int amount ) {
        ++resource_count;
        EXPECT_EQ( Resource::CREDIT, resource );
        if ( amount != -21 && amount != 100 )
            FAIL();
    } );
    player.SetOnRaiseTemperatureCallback( [ &temperature_count ]( Player* ) { ++temperature_count; } );
    player.SetOnDestroyResourceCallback( [ &destroy_count ]( Player*, Resource resource, int amount ) {
        ++destroy_count;
        EXPECT_EQ( Resource::PLANTS, resource );
        EXPECT_EQ( 3, amount );
    } );
    player.SetOnPlaceOceanCallback( [ &ocean_count ]( Player* ) { ++ocean_count; } );

    player.GetCard( comet );

    EXPECT_EQ( 0, resource_count );
    EXPECT_EQ( 0, temperature_count );
    EXPECT_EQ( 0, destroy_count );
    EXPECT_EQ( 0, ocean_count );

    EXPECT_FALSE( comet->CanBePlayed() );
    player.GainResource( Resource::CREDIT, 100 );
    EXPECT_TRUE( comet->CanBePlayed() );

    player.PlayCard( comet );

    EXPECT_EQ( 2, resource_count );
    EXPECT_EQ( 1, temperature_count );
    EXPECT_EQ( 1, destroy_count );
    EXPECT_EQ( 1, ocean_count );
    EXPECT_EQ( "", wrong_event_called );
}

TEST_F( CardsTest, AutomatedTest ) {
    cards::SolarWindPower* swp = new cards::SolarWindPower( model );
    
    player.SetOnResourceAmountChangedCallback( []( Player*, Resource resource, int amount ) {} );
    player.GainResource( Resource::CREDIT, 100 );

    EXPECT_FALSE( swp->IsActive() );
    EXPECT_TRUE ( swp->IsAutomated() );
    EXPECT_FALSE( swp->IsEvent() );
    EXPECT_EQ( CardID::SOLAR_WIND_POWER, swp->get_card_id() );
    EXPECT_FALSE( swp->HasTag( Tag::BUILDING ) );
    EXPECT_TRUE ( swp->HasTag( Tag::SPACE ) );
    EXPECT_TRUE ( swp->HasTag( Tag::POWER ) );
    EXPECT_TRUE ( swp->HasTag( Tag::SCIENCE ) );
    EXPECT_FALSE( swp->HasTag( Tag::JOVIAN ) );
    EXPECT_FALSE( swp->HasTag( Tag::EARTH ) );
    EXPECT_FALSE( swp->HasTag( Tag::PLANT ) );
    EXPECT_FALSE( swp->HasTag( Tag::MICROBE ) );
    EXPECT_FALSE( swp->HasTag( Tag::ANIMAL ) );
    EXPECT_FALSE( swp->HasTag( Tag::CITY ) );
    EXPECT_FALSE( swp->HasTag( Tag::EVENT ) );

    int resource_count = 0;
    int resource_production_count = 0;

    player.SetOnResourceAmountChangedCallback( [ &resource_count ]( Player*, Resource resource, int amount ) {
        ++resource_count;
        if ( resource == Resource::CREDIT )
            EXPECT_EQ( -11, amount );
        else if ( resource == Resource::TITANIUM )
            EXPECT_EQ( 2, amount );
        else
            FAIL();
    } );
    player.SetOnResourceProductionAmountChangedCallback( [ &resource_production_count ]( Player*, Resource resource, int amount ) {
        ++resource_production_count;
        EXPECT_EQ( Resource::ENERGY, resource );
        EXPECT_EQ( 1, amount );
    } );

    player.GetCard( swp );

    EXPECT_EQ( 0, resource_count );
    EXPECT_EQ( 0, resource_production_count );

    player.PlayCard( swp );

    EXPECT_EQ( 2, resource_count );
    EXPECT_EQ( 1, resource_production_count );
    EXPECT_EQ( "", wrong_event_called );
}

TEST_F( CardsTest, ActiveWithActionTest ) {
    cards::EquatorialMagnetizer* eqmag = new cards::EquatorialMagnetizer( model );

    player.SetOnResourceAmountChangedCallback( []( Player*, Resource resource, int amount ) {} );
    player.SetOnResourceProductionAmountChangedCallback( []( Player*, Resource resource, int amount ) {} );
    player.GainResource( Resource::CREDIT, 100 );
    player.GainResourceProduction( Resource::ENERGY, 2 );

    EXPECT_TRUE ( eqmag->IsActive() );
    EXPECT_TRUE ( eqmag->IsActiveWithAction() );
    EXPECT_FALSE( eqmag->IsActiveWithEffect() );
    EXPECT_FALSE( eqmag->IsAutomated() );
    EXPECT_FALSE( eqmag->IsEvent() );
    EXPECT_EQ( CardID::EQUATORIAL_MAGNETIZER, eqmag->get_card_id() );
    EXPECT_TRUE ( eqmag->HasTag( Tag::BUILDING ) );
    EXPECT_FALSE( eqmag->HasTag( Tag::SPACE ) );
    EXPECT_FALSE( eqmag->HasTag( Tag::POWER ) );
    EXPECT_FALSE( eqmag->HasTag( Tag::SCIENCE ) );
    EXPECT_FALSE( eqmag->HasTag( Tag::JOVIAN ) );
    EXPECT_FALSE( eqmag->HasTag( Tag::EARTH ) );
    EXPECT_FALSE( eqmag->HasTag( Tag::PLANT ) );
    EXPECT_FALSE( eqmag->HasTag( Tag::MICROBE ) );
    EXPECT_FALSE( eqmag->HasTag( Tag::ANIMAL ) );
    EXPECT_FALSE( eqmag->HasTag( Tag::CITY ) );
    EXPECT_FALSE( eqmag->HasTag( Tag::EVENT ) );

    int resource_count = 0;
    int resource_production_count = 0;
    int tr_count = 0;

    auto resource_callback = [ &resource_count ]( Player*, Resource resource, int amount ) {
        ++resource_count;
        EXPECT_EQ( Resource::CREDIT, resource );
        EXPECT_EQ( -11, amount );
    };
    player.SetOnResourceAmountChangedCallback( resource_callback );
    player.SetOnResourceProductionAmountChangedCallback( [ &resource_production_count ]( Player*, Resource resource, int amount ) {
        ++resource_production_count;
        EXPECT_EQ( Resource::ENERGY, resource );
        EXPECT_EQ( -1, amount );
    } );
    player.SetOnRaiseTRCallback( [ &tr_count ]( Player*, int amount ) {
        ++tr_count;
        EXPECT_EQ( 1, amount );
    } );

    player.GetCard( eqmag );

    EXPECT_EQ( 0, resource_count );
    EXPECT_EQ( 0, resource_production_count );
    EXPECT_EQ( 0, tr_count );

    player.PlayCard( eqmag );

    EXPECT_EQ( 1, resource_count );
    EXPECT_EQ( 0, resource_production_count );
    EXPECT_EQ( 0, tr_count );

    ASSERT_EQ( Availability::CAN_BE_USED, eqmag->Availability() );

    player.UseAction( eqmag );

    EXPECT_EQ( 1, resource_count );
    EXPECT_EQ( 1, resource_production_count );
    EXPECT_EQ( 1, tr_count );

    ASSERT_EQ( Availability::USED, eqmag->Availability() );

    player.SetOnResourceAmountChangedCallback( []( Player*, Resource resource, int amount ) {} );
    player.PerformProductionPhase();
    player.SetOnResourceAmountChangedCallback( resource_callback );

    EXPECT_EQ( 1, resource_count );
    EXPECT_EQ( 1, resource_production_count );
    EXPECT_EQ( 1, tr_count );

    ASSERT_EQ( Availability::CAN_BE_USED, eqmag->Availability() );

    player.UseAction( eqmag );

    EXPECT_EQ( 1, resource_count );
    EXPECT_EQ( 2, resource_production_count );
    EXPECT_EQ( 2, tr_count );

    ASSERT_EQ( Availability::USED, eqmag->Availability() );

    player.SetOnResourceAmountChangedCallback( []( Player*, Resource resource, int amount ) {} );
    player.PerformProductionPhase();
    player.SetOnResourceAmountChangedCallback( resource_callback );

    EXPECT_EQ( 1, resource_count );
    EXPECT_EQ( 2, resource_production_count );
    EXPECT_EQ( 2, tr_count );

    ASSERT_EQ( Availability::NOT_USABLE, eqmag->Availability() );

    EXPECT_EQ( "", wrong_event_called );
}

TEST_F( CardsTest, ActiveWithEffectTest ) {
    cards::RoverConstruction* rovcon = new cards::RoverConstruction( model );

    player.SetOnResourceAmountChangedCallback( []( Player*, Resource resource, int amount ) {} );
    player.GainResource( Resource::CREDIT, 100 );

    EXPECT_TRUE ( rovcon->IsActive() );
    EXPECT_FALSE( rovcon->IsActiveWithAction() );
    EXPECT_TRUE ( rovcon->IsActiveWithEffect() );
    EXPECT_FALSE( rovcon->IsAutomated() );
    EXPECT_FALSE( rovcon->IsEvent() );
    EXPECT_EQ( CardID::ROVER_CONSTRUCTION, rovcon->get_card_id() );
    EXPECT_TRUE ( rovcon->HasTag( Tag::BUILDING ) );
    EXPECT_FALSE( rovcon->HasTag( Tag::SPACE ) );
    EXPECT_FALSE( rovcon->HasTag( Tag::POWER ) );
    EXPECT_FALSE( rovcon->HasTag( Tag::SCIENCE ) );
    EXPECT_FALSE( rovcon->HasTag( Tag::JOVIAN ) );
    EXPECT_FALSE( rovcon->HasTag( Tag::EARTH ) );
    EXPECT_FALSE( rovcon->HasTag( Tag::PLANT ) );
    EXPECT_FALSE( rovcon->HasTag( Tag::MICROBE ) );
    EXPECT_FALSE( rovcon->HasTag( Tag::ANIMAL ) );
    EXPECT_FALSE( rovcon->HasTag( Tag::CITY ) );
    EXPECT_FALSE( rovcon->HasTag( Tag::EVENT ) );

    player.GetCard( rovcon );
    player.PlayCard( rovcon );

    int resource_count = 0;

    player.SetOnResourceAmountChangedCallback( [ &resource_count ]( Player*, Resource resource, int amount ) {
        ++resource_count;
        EXPECT_EQ( Resource::CREDIT, resource );
        EXPECT_EQ( 2, amount );
    } );

    EXPECT_EQ( 0, resource_count );

    player.OnEffect( &ActiveCardWithEffect::AfterAnyonePlacesCity );

    EXPECT_EQ( 1, resource_count );

    player.OnEffect( &ActiveCardWithEffect::AfterAnyonePlacesOcean );
    player.OnEffect( &ActiveCardWithEffect::AfterYouPlaySpaceEvent );

    EXPECT_EQ( 1, resource_count );

    player.OnEffect( &ActiveCardWithEffect::AfterAnyonePlacesCity );

    EXPECT_EQ( 2, resource_count );
    EXPECT_EQ( "", wrong_event_called );

    EXPECT_EQ( 1, rovcon->CountVPs() );
}
}
