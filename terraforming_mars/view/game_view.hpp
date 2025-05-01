#pragma once

#include "animation.fwd.hpp"
#include "game_view.fwd.hpp"
#include "game_view_state.fwd.hpp"

#include <array>
#include <optional>
#include <queue>
#include <random>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <GL/glew.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "animatable.hpp"
#include "card_wrapper.hpp"
#include "constants.hpp"
#include "panel.hpp"
#include "view.hpp"
#include "tile_wrapper.hpp"

#include "gl_utils/camera.hpp"
#include "gl_utils/camera_manipulator.hpp"
#include "gl_utils/spherical_camera_manipulator.hpp"
#include "gl_utils/tm_camera_manipulator.hpp"
#include "gl_utils/gl_utils.hpp"

#include "../model/constants.hpp"
#include "../model/game_model.hpp"
#include "../model/resource.hpp"
#include "../model/boards/tile_type.hpp"

namespace view
{
class GameView : public View
{
    friend class GameViewState;
    friend class ResearchVState;
    friend class IdleVState;
    friend class SellVState;
    friend class PlacementConfirmationVState;
    friend class PaymentConfirmationVState;
    friend class PostLastGenerationVState;
    friend class GameOverVState;

    friend class SoloGameOverVState;


    friend class InstantAnimation;
    friend class TextAnimation;
    friend class CardAnimation;
    friend class CardDrawAnimation;
    friend class CardPlayAnimation;

public:
    GameView( int seed );
    virtual ~GameView();

    bool Init( Camera* camera, model::GameModel* model );
    void Clean() override;

    void Update( const UpdateInfo& update_info ) override;
    void Render() override;
    void RenderGUI() override;
    void RenderMars( float elapsed );

    void KeyboardDown( const SDL_KeyboardEvent& key ) override;
    void KeyboardUp( const SDL_KeyboardEvent& key ) override;
    void MouseMotion( const SDL_MouseMotionEvent& mouse ) override;
    void MouseDown( const SDL_MouseButtonEvent& mouse ) override;
    void MouseUp( const SDL_MouseButtonEvent& mouse ) override;
    void MouseWheel( const SDL_MouseWheelEvent& wheel ) override;
    void Resize( int w, int h ) override;

    void OtherEvent( const SDL_Event& event ) override;

    inline void SetToMenu( std::function<void()> callback ) { _to_menu.SetCallback( callback ); }

protected:
    int _width = 0;
    int _height = 0;

    float _elapsed = 0.0f;

    std::mt19937 _random;

    Camera* _camera = nullptr;
    CameraManipulator* _active_camera_manipulator = nullptr;
    TMCameraManipulator* _tm_camera_manipulator = nullptr;
    SphericalCameraManipulator* _editorial_camera_manipulator = nullptr;


    GameViewState* _state = nullptr;
    GameViewState* _next_state = nullptr;

    virtual ResearchVState* CreateResearchState( std::array<const model::decks::Card*, model::RESEARCH_CARD_NUM> cards );
    virtual IdleVState* CreateIdleState();
    virtual SellVState* CreateSellState();
    virtual PlacementConfirmationVState* CreatePlacementConfirmationState( model::boards::TileType tile_type, std::vector<std::pair<int, int>> valid_positions );
    virtual PaymentConfirmationVState* CreatePaymentConfirmationState( int amount, model::Resource resource, int resource_value );
    virtual PostLastGenerationVState* CreatePostLastGenerationState();
    virtual GameOverVState* CreateGameOverState();

    void RequestStateChange( GameViewState* state );
    void RequestInstantStateChange( GameViewState* state );

    model::Event<> _to_menu;


    model::GameModel* _model = nullptr;

    int _generation = 0;
    int _temperature = 0;
    int _ocean_count = 0;
    int _oxygen_level = 0;
    int _tr = 0;
    std::array<int, +model::Resource::MAX + 1> _resources;
    std::array<int, +model::Resource::MAX + 1> _resource_productions;
    std::vector<CardWrapper*> _hand;
    std::vector<CardWrapper*> _action_cards;
    std::vector<CardWrapper*> _event_cards;
    std::vector<CardWrapper*> _automated_cards;
    std::vector<CardWrapper*> _effect_cards;
    std::vector<TileWrapper> _tiles;
    std::vector<std::vector<TileWrapper*>> _indexable_tiles;

    Panel _panel = Panel::NONE;
    std::array<int, 4> _page_nums = { 0, 0, 0, 0 };

    std::queue<glm::vec2> _card_play_positions;

    int _stencil_starting_misc = 0;
    float _hand_start_x = 0.0f;
    float _hand_end_x = 0.0f;
    float _hand_top_y = -1.0f + 1.0f / 9.0f;
    int _dragged_card_index = -1;
    uint8_t _mouse_hover_stencil = 0;
    uint8_t _mouse_down_stencil = 0;
    bool _menu_button_hovered = false;
    bool _end_button_hovered = false;
    glm::vec4 _mars_terrain_ranges = {};


    void Model_OnDrawCard( const model::decks::Card* card );
    void Model_OnDrawCardSpeed( const model::decks::Card* card, float speed );
    void Model_OnDrawCards( std::vector<const model::decks::Card*> cards );
    void Model_OnPlayCard( const model::decks::Card* card );
    void Model_OnRaiseTR( int amount );
    void Model_OnRaiseTemperature();
    void Model_OnRaiseOxygen();
    void Model_OnPlaceTile( std::pair<int, int> pos );
    void Model_OnResourceAmountChanged( model::Resource resource, int amount );
    void Model_OnResourceProductionAmountChanged( model::Resource resource, int amount );
    void Model_OnResearchConfirmed( std::array<bool, model::RESEARCH_CARD_NUM> selected );
    void Model_OnConfirmResearch( std::array<const model::decks::Card*, model::RESEARCH_CARD_NUM> cards );
    void Model_OnConfirmPayment( int amount, model::Resource resource, int resource_value );
    void Model_OnConfirmPlacement( model::boards::TileType tile_type, std::vector<std::pair<int, int>> valid_positions );
    void Model_OnConfirmDestroyResource( model::Resource resource, int amount );
    void Model_OnConfirmDestroyResourceProduction( model::Resource resource, int amount );
    virtual void Model_OnGameEnd();

    void CreateParameterAnimation( int parameter, std::string text, std::function<void()> on_start );

    std::queue<Animation*> _animation_queue;
    std::vector<std::tuple<float, float, Animation*>> _timed_out_animations;
    std::optional<Animation*> _locking_animation = {};
    std::vector<Animation*> _ongoing_animations;

    void RenderAnimation( InstantAnimation* animation );
    void RenderAnimation( TextAnimation* animation );
    void RenderAnimation( CardAnimation* animation );
    void RenderAnimation( CardDrawAnimation* animation );
    void RenderAnimation( CardPlayAnimation* animation );


    void RefreshHandPositions();

    void RenderBoard( bool clickable = true );
    void RenderHexagon( TileWrapper& tile, int id, bool clickable );

    glm::vec3 _la = glm::vec3( 0.0, 0.0, 0.0 );
    glm::vec3 _ld = glm::vec3( 1.0, 1.0, 1.0 );
    glm::vec3 _ls = glm::vec3( 1.0, 1.0, 1.0 );
    float _shininess = 1.0f;

    void SetLightUniforms();

    void RenderHUD();
    void RenderMenuButton();
    void RenderSP();
    void RenderGlobalParameters();
    void RenderEndButton();
    void RenderResources();
    void RenderHand();
    void RenderPanels();
    void RenderPanelCards( std::vector<CardWrapper*>& cards, int page_num, bool actions );

    void RenderCard( CardWrapper& card, int index, float d_z = 0.0f );
    void RenderDetail( float x, float y, glm::vec3 scale, float d_z = 0.0f );
    void RenderResource( float x, float y, glm::vec3 scale, int resource, float d_z = 0.0f );
    void RenderResourceProduction( float x, float y, glm::vec3 scale, int resource );

    void TurnPanelPageLeft();
    void TurnPanelPageRight();

    void DoTurnPanelPageLeft( int card_count, int& page_num );
    void DoTurnPanelPageRight( int card_count, int& page_num );

    std::pair<float, float> CalculateMousePos( float mouse_x, float mouse_y );
    int CalculateHoveredCardByPos( float x, float y );
    std::tuple<float, float, glm::vec3> CalculateParameterPosition( int parameter, int type );
    std::tuple<float, float, glm::vec3> CalculateResourcePosition( int resource, int type );
    std::tuple<float, float, glm::vec3> CalculateSPPosition( int sp, int right );
    std::tuple<float, float, glm::vec3> CalculateSPButtonPosition( int sp );

    uint8_t GetStencilValue( float mouse_x, float mouse_y );
    virtual const std::pair<float, float>& GetBoardOrigin();
    int GetLeftmostPageNum( int card_count );
    int GetRightmostPageNum( int card_count );

    inline void SetStencilRef( GLint ref = STENCIL_NONE ) { glStencilFunc( GL_ALWAYS, ref, 0xff ); }
    void SetPlayedCardParams( std::vector<CardWrapper*>& cards );

    void ClickedAction();
    void ClickedEvent();
    void ClickedAutomated();
    void ClickedEffect();
    void ClickedLeft();
    void ClickedRight();

    void InitBoard();


    bool _debug = false;
    float _debug_light_speed = 1.0f;
};
}
