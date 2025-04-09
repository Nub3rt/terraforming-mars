#pragma once

#include "animation.fwd.h"
#include "view.fwd.h"
#include "view_state.fwd.h"

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

#include "animatable.h"
#include "card_wrapper.h"
#include "constants.h"
#include "tile_wrapper.h"

#include "gl_utils/camera.h"
#include "gl_utils/spherical_camera_manipulator.h"
#include "gl_utils/gl_utils.h"

#include "../model/constants.h"
#include "../model/game_model.h"
#include "../model/resource.h"
#include "../model/boards/tile_type.h"

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
    SphericalCameraManipulator* _camera_manipulator = nullptr;


    ViewState* _state = nullptr;

    virtual ResearchVState* CreateResearchState();
    virtual IdleVState* CreateIdleState();
    virtual SellVState* CreateSellState();
    virtual PlacementConfirmationVState* CreatePlacementConfirmationState();
    virtual PaymentConfirmationVState* CreatePaymentConfirmationState();
    virtual PostLastGenerationVState* CreatePostLastGenerationState();
    virtual GameOverVState* CreateGameOverState();

    void ChangeState( ViewState* state );


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

    int _stencil_starting_card = 0;
    float _hand_start_x = 0.0f;
    float _hand_end_x = 0.0f;
    float _hand_top_y = -1.0f + 1.0f / 9.0f;
    int _dragged_card_index = -1;


    void Model_OnDrawCard( const model::decks::Card* card );
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

    void CreateParameterAnimation( int parameter, std::string text );

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
    void RenderGlobalParameters();
    void RenderEndButton();
    void RenderResources();
    void RenderHand();
    void RenderCard( CardWrapper& card, int index );

    uint8_t GetStencilValue( float mouse_x, float mouse_y );
    std::pair<float, float> CalculateMousePos( float mouse_x, float mouse_y );
    int CalculateHoveredCardByPos( float x, float y );
    std::tuple<float, float, glm::vec3> CalculateParameterPosition( int parameter, int type );
    std::tuple<float, float, glm::vec3> CalculateResourcePosition( int resource, int type );

    inline void SetStencilRef( GLint ref = STENCIL_NONE ) { glStencilFunc( GL_ALWAYS, ref, 0xff ); }


    GLuint _program_id = 0;
    GLuint _program_rectangle_id = 0;
    GLuint _program_sprite_sheet_id = 0;

    void InitShaders();
    void CleanShaders();

    OGLObject _hexagon_gpu = {};
    OGLObject _rectangle_gpu = {};

    void InitGeometry();
    void CleanGeometry();

    GLuint _orange_texture_id = 0;
    Texture _cards_texture = {};
    Texture _resources_texture = {};
    Texture _card_cover_texture = {};
    Texture _temperature_texture = {};
    Texture _ocean_texture = {};
    Texture _oxygen_texture = {};
    Texture _tr_texture = {};
    Texture _button_texture = {};
    Texture _production_box_texture = {};

    void InitTextures();
    void CleanTextures();
    Texture LoadTexture( const std::filesystem::path& filename, GLint wrap_behaviour = GL_CLAMP_TO_EDGE );

    virtual const std::pair<float, float>& GetBoardOrigin();

    static const std::initializer_list<VertexAttributeDescriptor> _vertex_pos_tex_attribute_list;
    static const std::initializer_list<VertexAttributeDescriptor> _vertex_attribute_list;
    static const std::initializer_list<VertexAttributeDescriptor> _vertex_plus_attribute_list;
};
}
