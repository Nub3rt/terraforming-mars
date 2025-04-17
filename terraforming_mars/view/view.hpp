#pragma once

#include "animation.fwd.hpp"
#include "view.fwd.hpp"
#include "view_state.fwd.hpp"

#include <array>
#include <optional>
#include <queue>
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
class View
{
    friend class ViewState;
    friend class ResearchVState;
    friend class IdleVState;
    friend class SellVState;
    friend class PlacementConfirmationVState;
    friend class PaymentConfirmationVState;
    friend class PostLastGenerationVState;
    friend class GameOverVState;

    friend class InstantAnimation;
    friend class TextAnimation;
    friend class CardAnimation;
    friend class CardDrawAnimation;

public:
    View();
    ~View();

    bool Init( Camera* camera, model::GameModel* model );
    void Clean();

    void Update( const UpdateInfo& update_info );
    void Render();
    void RenderGUI();

    void KeyboardDown( const SDL_KeyboardEvent& key );
    void KeyboardUp( const SDL_KeyboardEvent& key );
    void MouseMotion( const SDL_MouseMotionEvent& mouse );
    void MouseDown( const SDL_MouseButtonEvent& mouse );
    void MouseUp( const SDL_MouseButtonEvent& mouse );
    void MouseWheel( const SDL_MouseWheelEvent& wheel );
    void Resize( int w, int h );

    void OtherEvent( const SDL_Event& event );

protected:
    int _width = 0;
    int _height = 0;

    float _elapsed = 0.0f;

    Camera* _camera = nullptr;
    CameraManipulator* _active_camera_manipulator = nullptr;
    TMCameraManipulator* _tm_camera_manipulator = nullptr;
    SphericalCameraManipulator* _editorial_camera_manipulator = nullptr;


    ViewState* _state = nullptr;
    ViewState* _next_state = nullptr;

    virtual ResearchVState* CreateResearchState( std::array<const model::decks::Card*, model::RESEARCH_CARD_NUM> cards );
    virtual IdleVState* CreateIdleState();
    virtual SellVState* CreateSellState();
    virtual PlacementConfirmationVState* CreatePlacementConfirmationState( model::boards::TileType tile_type, std::vector<std::pair<int, int>> valid_positions );
    virtual PaymentConfirmationVState* CreatePaymentConfirmationState( int amount, model::Resource resource, int resource_value );
    virtual PostLastGenerationVState* CreatePostLastGenerationState();
    virtual GameOverVState* CreateGameOverState();

    void RequestStateChange( ViewState* state );
    void RequestInstantStateChange( ViewState* state );


    model::GameModel* _model = nullptr;

    int _generation = 0;
    int _temperature = 0;
    int _ocean_count = 0;
    int _oxygen_level = 0;
    int _tr = 0;
    std::array<int, +model::Resource::MAX + 1> _resources;
    std::array<int, +model::Resource::MAX + 1> _resource_productions;
    std::vector<CardWrapper*> _hand;
    std::vector<CardWrapper> _events;
    std::vector<CardWrapper> _automated;
    std::vector<CardWrapper> _effects;
    std::vector<CardWrapper> _actions;
    std::vector<TileWrapper> _tiles;
    std::vector<std::vector<TileWrapper*>> _indexable_tiles;

    int _stencil_starting_card = 0;
    float _hand_start_x = 0.0f;
    float _hand_end_x = 0.0f;
    float _hand_top_y = -1.0f + 1.0f / 9.0f;
    int _dragged_card_index = -1;
    uint8_t _mouse_hover_stencil = 0;
    uint8_t _mouse_down_stencil = 0;
    bool _menu_button_hovered = false;
    bool _end_button_hovered = false;


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
    void Model_OnGameEnd();

    void CreateParameterAnimation( int parameter, std::string text, std::function<void()> on_start );

    std::queue<Animation*> _animation_queue;
    std::vector<std::tuple<float, float, Animation*>> _timed_out_animations;
    std::optional<Animation*> _locking_animation = {};
    std::vector<Animation*> _ongoing_animations;

    void RenderAnimation( InstantAnimation* animation );
    void RenderAnimation( TextAnimation* animation );
    void RenderAnimation( CardAnimation* animation );
    void RenderAnimation( CardDrawAnimation* animation );


    void RefreshHandPositions();

    void RenderBoard();
    void RenderHexagon( TileWrapper& tile, int id );

    void RenderHUD();
    void RenderMenuButton();
    void RenderSP();
    void RenderGlobalParameters();
    void RenderEndButton();
    void RenderResources();
    void RenderHand();
    void RenderCard( CardWrapper& card, int index );
    void RenderDetail( float x, float y, glm::vec3 scale, float d_z = 0.0f );
    void RenderResource( float x, float y, glm::vec3 scale, int resource, float d_z = 0.0f );
    void RenderResourceProduction( float x, float y, glm::vec3 scale, int resource );

    uint8_t GetStencilValue( float mouse_x, float mouse_y );
    std::pair<float, float> CalculateMousePos( float mouse_x, float mouse_y );
    int CalculateHoveredCardByPos( float x, float y );
    std::tuple<float, float, glm::vec3> CalculateParameterPosition( int parameter, int type );
    std::tuple<float, float, glm::vec3> CalculateResourcePosition( int resource, int type );
    std::tuple<float, float, glm::vec3> CalculateSPPosition( int sp, int right );
    std::tuple<float, float, glm::vec3> CalculateSPButtonPosition( int sp );

    inline void SetStencilRef( GLint ref = STENCIL_NONE ) { glStencilFunc( GL_ALWAYS, ref, 0xff ); }


    GLuint _program_id = 0;
    GLuint _program_card_id = 0;
    GLuint _program_rectangle_id = 0;
    GLuint _program_sprite_sheet_id = 0;

    void InitShaders();
    void CleanShaders();

    OGLObject _hexagon_gpu = {};
    OGLObject _rectangle_gpu = {};

    void InitGeometry();
    void CleanGeometry();

    Texture _cards_texture = {};
    Texture _resources_texture = {};
    Texture _card_cover_texture = {};
    Texture _temperature_texture = {};
    Texture _ocean_texture = {};
    Texture _oxygen_texture = {};
    Texture _tr_texture = {};
    Texture _greenery_texture = {};
    Texture _city_texture = {};
    Texture _button_texture = {};
    Texture _production_box_texture = {};
    Texture _arrow_texture = {};

    void InitTextures();
    void CleanTextures();
    Texture LoadTexture( const std::filesystem::path& filename, GLint wrap_behaviour = GL_CLAMP_TO_EDGE );

    virtual const std::pair<float, float>& GetBoardOrigin();

    static const std::initializer_list<VertexAttributeDescriptor> _vertex_pos_tex_attribute_list;
    static const std::initializer_list<VertexAttributeDescriptor> _vertex_attribute_list;
    static const std::initializer_list<VertexAttributeDescriptor> _vertex_plus_attribute_list;
};
}
