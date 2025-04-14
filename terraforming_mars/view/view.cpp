#include "view.h"

#include <algorithm>
#include <array>
#include <format>
#include <functional>
#include <iostream>
#include <stdexcept>

#include <glm/glm.hpp>
#include <imgui.h>

#include "animation.h"
#include "animatable.h"
#include "constants.h"
#include "text_renderer.h"
#include "view_state.h"

#include "../model/constants.h"
#include "../model/resource.h"

namespace view
{
View::View() : _resources(), _resource_productions() {}
View::~View() {}

bool View::Init( Camera* camera, model::GameModel* model ) {
    InitShaders();
    InitGeometry();
    InitTextures();


    _camera = camera;
    _camera_manipulator = new SphericalCameraManipulator();
    _camera_manipulator->SetCamera( camera );


    _state = CreateIdleState();


    _model = model;

    _model->SetOnDrawCard( std::bind_front( &View::Model_OnDrawCard, this ) );
    _model->SetOnDrawCards( std::bind_front( &View::Model_OnDrawCards, this ) );
    _model->SetOnPlayCard( std::bind_front( &View::Model_OnPlayCard, this ) );
    _model->SetOnRaiseTR( std::bind_front( &View::Model_OnRaiseTR, this ) );
    _model->SetOnRaiseTemperature( std::bind_front( &View::Model_OnRaiseTemperature, this ) );
    _model->SetOnRaiseOxygen( std::bind_front( &View::Model_OnRaiseOxygen, this ) );
    _model->SetOnPlaceTile( std::bind_front( &View::Model_OnPlaceTile, this ) );
    _model->SetOnResourceAmountChanged( std::bind_front( &View::Model_OnResourceAmountChanged, this ) );
    _model->SetOnResourceProductionAmountChanged( std::bind_front( &View::Model_OnResourceProductionAmountChanged, this ) );
    _model->SetOnResearchConfirmed( std::bind_front( &View::Model_OnResearchConfirmed, this ) );
    _model->SetOnConfirmResearch( std::bind_front( &View::Model_OnConfirmResearch, this ) );
    _model->SetOnConfirmPayment( std::bind_front( &View::Model_OnConfirmPayment, this ) );
    _model->SetOnConfirmPlacement( std::bind_front( &View::Model_OnConfirmPlacement, this ) );
    _model->SetOnConfirmDestroyResource( std::bind_front( &View::Model_OnConfirmDestroyResource, this ) );
    _model->SetOnConfirmDestroyResourceProduction( std::bind_front( &View::Model_OnConfirmDestroyResourceProduction, this ) );
    _model->SetOnGameEnd( std::bind_front( &View::Model_OnGameEnd, this ) );

    _temperature = _model->Temperature();
    _ocean_count = _model->OceanCount();
    _oxygen_level = _model->Oxygen();
    _tr = _model->get_local_player()->get_tr();
    _resources = std::array<int, +model::Resource::MAX + 1>( _model->get_local_player()->get_resources() );
    _resource_productions = std::array<int, +model::Resource::MAX + 1>( _model->get_local_player()->get_resource_productions() );

    _model->Start();

    int max_size = 0;
    for ( const model::boards::Tile& tile : *_model->get_board() ) {
        auto [q, r] = tile.get_indices();
        if ( q > max_size )
            max_size = q;
        if ( r > max_size )
            max_size = r;

        _tiles.emplace_back( tile );
    }
    _stencil_starting_card = STENCIL_STARTING_BOARD + (int)_tiles.size();

    _indexable_tiles = std::vector<std::vector<TileWrapper*>>( max_size + 1,
        std::vector<TileWrapper*>( max_size + 1, nullptr )
    );
    for ( TileWrapper& tile : _tiles ) {
        auto [q, r] = tile->get_indices();
        _indexable_tiles[ r ][ q ] = &tile;
    }

    return true;
}

void View::Clean() {
    CleanShaders();
    CleanGeometry();
    CleanTextures();

    delete _camera_manipulator;

    delete _state;

    delete _model;
}

void View::Update( const UpdateInfo& update_info ) {
    _elapsed = update_info.elapsed;

    _camera_manipulator->Update( update_info.delta );


    for ( TileWrapper& tile : _tiles )
        tile.Update( update_info.delta );

    for ( CardWrapper* card : _hand )
        card->Update( update_info.delta );

    for ( auto it = _ongoing_animations.begin(); it != _ongoing_animations.end(); ) {
        Animation* animation = *it;

        if ( animation->IsOver() ) {
            if ( animation->on_end )
                animation->on_end();

            it = _ongoing_animations.erase( it );
            delete animation;
        } else {
            animation->Update( update_info.delta );
            ++it;
        }
    }

    for ( auto it = _timed_out_animations.begin(); it != _timed_out_animations.end(); ) {
        auto& [elapsed, duration, animation] = *it;
        elapsed += update_info.delta;

        if ( duration <= elapsed ) {
            if ( animation->on_start )
                animation->on_start();

            _ongoing_animations.push_back( animation );
            it = _timed_out_animations.erase( it );
        } else {
            ++it;
        }
    }

    if ( _locking_animation ) {
        Animation* l_anim = *_locking_animation;

        if ( l_anim->HasLockout() ) {
            l_anim->Update( update_info.delta );
        } else {
            _locking_animation.reset();

            if ( l_anim->IsOver() ) {
                if ( l_anim->on_end )
                    l_anim->on_end();

                delete l_anim;
            } else {
                _ongoing_animations.push_back( l_anim );

                l_anim->Update( update_info.delta );
            }
        }
    }

    while ( !_locking_animation && _animation_queue.size() != 0 ) {
        Animation* animation = _animation_queue.front();
        _animation_queue.pop();

        if ( animation->on_start )
            animation->on_start();

        if ( animation->HasLockout() ) {
            _locking_animation = animation;
        } else {
            _ongoing_animations.push_back( animation );
        }
    }

    _model->Update();

    if ( _next_state != nullptr && !_locking_animation ) {
        delete _state;
        _state = _next_state;
        _next_state = nullptr;
        _state->Enter();
    }
}

void View::Render() {
    RenderBoard();

    glClear( GL_DEPTH_BUFFER_BIT );

    RenderHUD();

    for ( Animation* animation : _ongoing_animations )
        animation->Render( this );

    if ( _locking_animation )
        (*_locking_animation)->Render( this );

    _state->Render();
}

void View::RenderGUI() {
}

#pragma region Events

void View::KeyboardDown( const SDL_KeyboardEvent& key ) {
    _camera_manipulator->KeyboardDown( key );
}

void View::KeyboardUp( const SDL_KeyboardEvent& key ) {
    _camera_manipulator->KeyboardUp( key );
}

void View::MouseMotion( const SDL_MouseMotionEvent& mouse ) {
    auto [x, y] = CalculateMousePos( mouse.x, mouse.y );

    if ( _dragged_card_index == -1 ) {
        _camera_manipulator->MouseMove( mouse );

        int hovered_card_index = CalculateHoveredCardByPos( x, y );

        for ( int i = 0; i < _hand.size(); ++i ) {
            CardWrapper& card = *_hand[ i ];

            if ( card.state == CardWrapper::HOVERED && hovered_card_index != i ) {
                card.state = CardWrapper::IDLE;
                card.pos.y.SetAnim( -1.0f + (1.0f + *card.pos.y) / 2.0f, card.base_pos.y, CARD_ADJUST_DURATION );
                card.scale.Set( card.base_scale );
                card.rotate.Set( card.base_rotate );
            } else if ( _state->CanHoverHand() &&
                        card.state == CardWrapper::IDLE &&
                        hovered_card_index == i ) {
                card.state = CardWrapper::HOVERED;
                card.scale.Set( card.base_scale * 2.0f );
                card.pos.y.Set( card.base_pos.y + 0.6f );
                card.rotate.Set( 0.0f );
            }
        }
    } else {
        CardWrapper* dragged_card = _hand[ _dragged_card_index ];

        if ( y > CARD_DRAG_OUT_LINE_Y && !_state->CanDragCardOut( dragged_card ) ) {
            dragged_card->state = CardWrapper::TO_HAND;
            dragged_card->GoToBase( CARD_ADJUST_DURATION );
            _timed_out_animations.emplace_back( 0.0f, CARD_ADJUST_DURATION, new InstantAnimation( [ this, card = dragged_card ]() {
                card->state = CardWrapper::IDLE;
            } ) );

            _dragged_card_index = -1;
        } else {
            dragged_card->pos.Set( glm::vec2( x, y ) );
        }
    }
}

void View::MouseDown( const SDL_MouseButtonEvent& mouse ) {
    _mouse_down_stencil = GetStencilValue( mouse.x, mouse.y );
    std::cout << std::to_string( _mouse_down_stencil ) << std::endl;

    auto [x, y] = CalculateMousePos( mouse.x, mouse.y );
    int hovered_card_index = CalculateHoveredCardByPos( x, y );
    if ( hovered_card_index != -1 && _state->CanHoverHand() ) {
        _dragged_card_index = hovered_card_index;
        CardWrapper& dragged_card = *_hand[ _dragged_card_index ];
        dragged_card.state = CardWrapper::DRAGGING;
        dragged_card.pos.Set( glm::vec2( x, y ) );
        dragged_card.scale.Set( CARD_BASE_SCALE );
        dragged_card.rotate.Set( 0.0f );
    }
}

void View::MouseUp( const SDL_MouseButtonEvent& mouse ) {
    if ( _dragged_card_index != -1 ) {
        auto [x, y] = CalculateMousePos( mouse.x, mouse.y );
        CardWrapper* dragged_card = _hand[ _dragged_card_index ];

        if ( y <= CARD_DRAG_OUT_LINE_Y ) {
            dragged_card->state = CardWrapper::TO_HAND;
            dragged_card->GoToBase( CARD_ADJUST_DURATION );
            _timed_out_animations.emplace_back( 0.0f, CARD_ADJUST_DURATION, new InstantAnimation( [ this, card = dragged_card ]() {
                card->state = CardWrapper::IDLE;
            } ) );

            _dragged_card_index = -1;
        } else {
            _state->PlayCard( _dragged_card_index );
            _dragged_card_index = -1;
        }
    } else {
        uint8_t stencil = GetStencilValue( mouse.x, mouse.y );
        if ( stencil == _mouse_down_stencil ) {
            switch ( stencil ) {
                case STENCIL_NONE: break;
                case 0x00:
                    std::cerr << "View::MouseUp: Invalid stencil value received: 0!";
                    break;
                case STENCIL_MENU:

                    break;
                case STENCIL_END:

                    break;
                case STENCIL_EVENTS:

                    break;
                case STENCIL_AUTOMATED:

                    break;
                case STENCIL_EFFECTS:

                    break;
                case STENCIL_ACTIONS:

                    break;
                default: // tile/active card was clicked
                    if ( stencil < STENCIL_STARTING_BOARD + _tiles.size() ) {
                        _state->ClickedOnTile( _tiles[ stencil - STENCIL_STARTING_BOARD ] );
                    } else {
                        // TODO active cards
                    }
            }
        }
    }
}

void View::MouseWheel( const SDL_MouseWheelEvent& wheel ) {
    _camera_manipulator->MouseWheel( wheel );
}

void View::Resize( int w, int h ) {
    _width = w;
    _height = h;
}

void View::OtherEvent( const SDL_Event& event ) {
}

#pragma endregion Events

#pragma region State

ResearchVState* View::CreateResearchState() { return new ResearchVState( *this ); }
IdleVState* View::CreateIdleState() { return new IdleVState( *this ); }
SellVState* View::CreateSellState() { return new SellVState( *this ); }
PlacementConfirmationVState* View::CreatePlacementConfirmationState( model::boards::TileType tile_type, std::vector<std::pair<int, int>> valid_positions ) { return new PlacementConfirmationVState( *this, tile_type, std::move( valid_positions ) ); }
PaymentConfirmationVState* View::CreatePaymentConfirmationState() { return new PaymentConfirmationVState( *this ); }
PostLastGenerationVState* View::CreatePostLastGenerationState() { return new PostLastGenerationVState( *this ); }
GameOverVState* View::CreateGameOverState() { return new GameOverVState( *this ); }

void View::RequestStateChange( ViewState* state ) {
    if ( _next_state == nullptr )
        _next_state = state;
    else
        throw std::logic_error( "View::RequestStateChange: another state change was already requested!" );
}

void View::RequestInstantStateChange( ViewState* state ) {
    delete _state;
    _state = state;
    _state->Enter();
}

#pragma endregion State

#pragma region Animation Queue

void View::Model_OnDrawCard( const model::decks::Card* card ) {
    Model_OnDrawCardSpeed( card, 1.0f );
}

void View::Model_OnDrawCardSpeed( const model::decks::Card* card, float speed ) {
    CardWrapper* card_wrapper = new CardWrapper( card );
    card_wrapper->state = CardWrapper::DRAWING;

    CardDrawAnimation* animation = new CardDrawAnimation( card_wrapper, speed );
    animation->SetOnStart( [ this, card_wrapper ]() {
        _hand.push_back( card_wrapper );
        RefreshHandPositions();
    } );
    animation->SetOnCompleted( [ this, card_wrapper ]() {
        card_wrapper->state = CardWrapper::IDLE;
    } );

    _animation_queue.push( animation );
}

void View::Model_OnDrawCards( std::vector<const model::decks::Card*> cards ) {
    float speed = 1.0f;
    for ( int i = 0; i < cards.size(); ++i ) {
        speed *= 1.2f;
        Model_OnDrawCardSpeed( cards[ i ], speed );
    }
}

void View::Model_OnPlayCard( const model::decks::Card* card ) {
    // TODO
}

void View::Model_OnRaiseTR( int amount ) {
    CreateParameterAnimation( 3, std::format( "+{}", amount ), [ =, this ]() {
        _tr += amount;
    } );
}

void View::Model_OnRaiseTemperature() {
    CreateParameterAnimation( 0, "+2", [ this ]() {
        _temperature += 2;
    } );
}

void View::Model_OnRaiseOxygen() {
    CreateParameterAnimation( 2, "+1", [ this ]() {
        _oxygen_level += 1;
    } );
}

void View::Model_OnPlaceTile( std::pair<int, int> pos ) {
    auto& [q, r] = pos;
    TileWrapper* tile = _indexable_tiles[ r ][ q ];
    if ( tile == nullptr )
        throw std::logic_error( "View::Model_OnPlaceTile: received invalid indices!" );

    if ( (*tile)->get_type() == model::boards::TileType::OCEAN )
        CreateParameterAnimation( 1, "+1", [ =, this ]() {
            _ocean_count += 1;
            tile->OnTilePlaced();
        } );
    else
        _animation_queue.push( new InstantAnimation( DEFAULT_LOCKOUT_DURATION, [ =, this ]() {
            tile->OnTilePlaced();
        } ) );
}

void View::Model_OnResourceAmountChanged( model::Resource resource, int amount ) {
    std::function<void()> on_start = [ =, this ]() {
        _resources[ +resource ] += amount;
    };

    auto [x, y, _] = CalculateResourcePosition( +resource, 2 );

    if ( amount >= 0 )
        _animation_queue.push( (new TextAnimation(
            DEFAULT_LOCKOUT_DURATION,
            ATTRIBUTE_CHANGED_DURATION,
            std::format( "+{}", amount ),
            glm::vec2( x, y ), glm::vec2( x - TEXT_FLOAT_DISTANCE, y ),
            glm::vec4( POSITIVE_TEXT_COLOR, 1.0f ), glm::vec4( POSITIVE_TEXT_COLOR, 0.0f ),
            BASE_TEXT_SCALE
        ))->SetOnStart( on_start ) );
    else
        _animation_queue.push( (new TextAnimation(
            DEFAULT_LOCKOUT_DURATION,
            ATTRIBUTE_CHANGED_DURATION,
            std::to_string( amount ),
            glm::vec2( x, y ), glm::vec2( x - TEXT_FLOAT_DISTANCE, y ),
            glm::vec4( NEGATIVE_TEXT_COLOR, 1.0f ), glm::vec4( NEGATIVE_TEXT_COLOR, 0.0f ),
            BASE_TEXT_SCALE
        ))->SetOnStart( on_start ) );
}

void View::Model_OnResourceProductionAmountChanged( model::Resource resource, int amount ) {
    std::function<void()> on_start = [ =, this ]() {
        _resource_productions[ +resource ] += amount;
    };

    auto [x, y, _] = CalculateResourcePosition( +resource, 0 );

    if ( amount >= 0 )
        _animation_queue.push( (new TextAnimation(
            DEFAULT_LOCKOUT_DURATION,
            ATTRIBUTE_CHANGED_DURATION,
            std::format( "+{}", amount ),
            glm::vec2( x, y ), glm::vec2( x - TEXT_FLOAT_DISTANCE, y ),
            glm::vec4( PRODUCTION_TEXT_COLOR, 1.0f ), glm::vec4( PRODUCTION_TEXT_COLOR, 0.0f ),
            BASE_TEXT_SCALE
        ))->SetOnStart( on_start ) );
    else
        _animation_queue.push( (new TextAnimation(
            DEFAULT_LOCKOUT_DURATION,
            ATTRIBUTE_CHANGED_DURATION,
            std::to_string( amount ),
            glm::vec2( x, y ), glm::vec2( x - TEXT_FLOAT_DISTANCE, y ),
            glm::vec4( PRODUCTION_TEXT_COLOR, 1.0f ), glm::vec4( PRODUCTION_TEXT_COLOR, 0.0f ),
            BASE_TEXT_SCALE
        ))->SetOnStart( on_start ) );
}

void View::Model_OnResearchConfirmed( std::array<bool, model::RESEARCH_CARD_NUM> selected ) { throw "not implemented"; }
void View::Model_OnConfirmResearch( std::array<const model::decks::Card*, model::RESEARCH_CARD_NUM> cards ) { throw "not implemented"; }
void View::Model_OnConfirmPayment( int amount, model::Resource resource, int resource_value ) { throw "not implemented"; }

void View::Model_OnConfirmPlacement( model::boards::TileType tile_type, std::vector<std::pair<int, int>> valid_positions ) {
    RequestStateChange( CreatePlacementConfirmationState( tile_type, std::move( valid_positions ) ) );
}

void View::Model_OnConfirmDestroyResource( model::Resource resource, int amount ) { throw "not implemented"; }
void View::Model_OnConfirmDestroyResourceProduction( model::Resource resource, int amount ) { throw "not implemented"; }
void View::Model_OnGameEnd() { throw "not implemented"; }

void View::CreateParameterAnimation( int parameter, std::string text, std::function<void()> on_start ) {
    auto [x, y, _] = CalculateParameterPosition( parameter, 1 );

    _animation_queue.push( (new TextAnimation(
        DEFAULT_LOCKOUT_DURATION,
        ATTRIBUTE_CHANGED_DURATION,
        text,
        glm::vec2( x, y ), glm::vec2( x - TEXT_FLOAT_DISTANCE, y ),
        glm::vec4( POSITIVE_TEXT_COLOR, 1.0f ), glm::vec4( POSITIVE_TEXT_COLOR, 0.0f ),
        BASE_TEXT_SCALE
    ))->SetOnStart( on_start ) );
}

void View::RenderAnimation( InstantAnimation* animation ) {
    animation->perform();
}

void View::RenderAnimation( TextAnimation* animation ) {
    TextRenderer::RenderTextCentered(
        animation->text,
        *animation->pos.x,
        *animation->pos.y,
        *animation->scale,
        *animation->color
    );
}

void View::RenderAnimation( CardAnimation* animation ) {
    CardWrapper& card = *animation->card;

    if ( animation->start_pos )
        card.pos.SetAnim( *animation->start_pos, animation->end_pos, animation->duration );
    else
        card.pos.UpdateAnim( animation->end_pos, animation->duration, animation->force_time );

    if ( animation->start_scale )
        card.scale.SetAnim( *animation->start_scale, animation->end_scale, animation->duration );
    else
        card.scale.UpdateAnim( animation->end_scale, animation->duration, animation->force_time );

    if ( animation->start_rotate )
        card.rotate.SetAnim( *animation->start_rotate, animation->end_rotate, animation->duration );
    else
        card.rotate.UpdateAnim( animation->end_rotate, animation->duration, animation->force_time );

    if ( animation->ease )
        card.SetEase( animation->ease );
}

void View::RenderAnimation( CardDrawAnimation* animation ) {
    if ( animation->elapsed < CARD_DRAW_IN_DURATION ) {
        static const std::function<float( float )> ease = glm::quarticEaseOut<float>;
        float t = animation->elapsed / CARD_DRAW_IN_DURATION;
        animation->card->pos.Set( CARD_DRAW_POS_START +
            (CARD_DRAW_POS_MIDDLE - CARD_DRAW_POS_START) * ease( t ) );
        animation->card->scale.Set( CARD_DRAW_SCALE_START +
            (CARD_DRAW_SCALE_MIDDLE - CARD_DRAW_SCALE_START) * ease( t ) );
        animation->card->rotate.Set( CARD_DRAW_ROTATE_START +
            (CARD_DRAW_ROTATE_MIDDLE - CARD_DRAW_ROTATE_START) * ease( t ) );
    } else {
        static const std::function<float( float )> ease = glm::quarticEaseIn<float>;
        float t = (animation->elapsed - CARD_DRAW_IN_DURATION) / CARD_DRAW_DOWN_DURATION;
        animation->card->pos.Set( CARD_DRAW_POS_MIDDLE +
            (animation->card->base_pos - CARD_DRAW_POS_MIDDLE) * ease( t ) );
        animation->card->scale.Set( CARD_DRAW_SCALE_MIDDLE +
            (animation->card->base_scale - CARD_DRAW_SCALE_MIDDLE) * ease( t ) );
        animation->card->rotate.Set( CARD_DRAW_ROTATE_MIDDLE +
            (animation->card->base_rotate - CARD_DRAW_ROTATE_MIDDLE) * ease( t ) );
    }
}

#pragma endregion Animation Queue

void View::RefreshHandPositions() {
    static const float card_ratio = (float)CARD_TEXTURE_WIDTH / CARD_TEXTURE_HEIGHT;
    static const float min_x = -0.6f;
    static const float max_x =  0.6f;
    static const float mid_x = (min_x + max_x) / 2.0f;
    static const float base_spacing = 0.1f;
    float card_width = CARD_BASE_SCALE * card_ratio / _width * _height;

    if ( _hand.size() == 0 )
        return;

    _hand_start_x = fmaxf( min_x, mid_x - (_hand.size() - 1) / 2.0f * base_spacing );
    _hand_end_x   = fminf( max_x, mid_x + (_hand.size() - 1) / 2.0f * base_spacing );

    if ( _hand.size() == 1 ) {
        CardWrapper& card = *_hand[ 0 ];
        card.base_pos.x = mid_x;
        card.base_pos.y = HAND_BASE_Y;
        card.base_scale = CARD_BASE_SCALE;
        card.base_rotate = 0.0f;

        if ( card.state == CardWrapper::DRAGGING ||
             card.state == CardWrapper::DRAWING )
            return;

        if ( card.state == CardWrapper::IDLE ) {
            card.GoToBase( CARD_ADJUST_DURATION );
            return;
        }

        card.pos.x.UpdateAnim( card.base_pos.x, CARD_ADJUST_DURATION );
        return;
    }

    float spacing = (_hand_end_x - _hand_start_x) / (_hand.size() - 1);
    for ( int i = 0; i < _hand.size(); ++i ) {
        CardWrapper& card = *_hand[ i ];
        card.base_pos.x = _hand_start_x + i * spacing;
        card.base_pos.y = HAND_BASE_Y;
        card.base_scale = CARD_BASE_SCALE;
        card.base_rotate = 0.0f;

        if ( card.state == CardWrapper::DRAGGING ||
             card.state == CardWrapper::DRAWING )
            continue;

        if ( card.state == CardWrapper::IDLE ) {
            card.GoToBase( CARD_ADJUST_DURATION );
            continue;
        }

        card.pos.x.UpdateAnim( card.base_pos.x, CARD_ADJUST_DURATION );
    }
}

#pragma region Rendering

void View::RenderBoard() {
    glUseProgram( _program_id );
    glBindVertexArray( _hexagon_gpu.vao_id );

    for ( int i = 0; i < _tiles.size(); ++i) {
        RenderHexagon( _tiles[i], STENCIL_STARTING_BOARD + i );
    }

    glBindVertexArray( 0 );
    glUseProgram( 0 );
}

void View::RenderHexagon( TileWrapper& tile, int id ) {
    /*
    *  *----> q         Ʌ y
    *   \               |
    *    \      ----->  |
    *     \             |
    *      V r          *----> x
    */

    auto& [q_o, r_o] = GetBoardOrigin();
    float q = tile->q - q_o;
    float r = tile->r - r_o;

    float x = glm::root_three<float>() * q + glm::root_three<float>() / 2.0f * r;
    float y = 3.0f / 2.0f * -r;
    glm::mat4 world = glm::translate( glm::vec3( x, y, 0.0f ) );

    glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );
    glUniformMatrix4fv( ul( "world_it" ), 1, GL_FALSE, glm::value_ptr( glm::transpose( glm::inverse( world ) ) ) );
    glUniformMatrix4fv( ul( "view_proj" ), 1, GL_FALSE, glm::value_ptr( _camera->GetViewProj() ) );

    glUniform1i( ul( "image" ), 0 );
    glUniform3f( ul( "color" ), tile.color->r, tile.color->g, tile.color->b );
    glUniform3f( ul( "border_color" ), tile.border_color.r, tile.border_color.g, tile.border_color.b );

    SetStencilRef( id );

    glDrawElements( GL_TRIANGLES, _hexagon_gpu.count, GL_UNSIGNED_INT, nullptr );

    SetStencilRef();
}

void View::RenderHUD() {
    RenderMenuButton();
    RenderGlobalParameters();
    RenderEndButton();
    RenderResources();
    RenderHand();
}

void View::RenderMenuButton() {
    // TODO
}

void View::RenderGlobalParameters() {
    glUseProgram( _program_rectangle_id );
    glBindVertexArray( _rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( ul( "image" ), 0 );


    std::array<GLuint, 4> to_draw = {
        _temperature_texture.id,
        _ocean_texture.id,
        _oxygen_texture.id,
        _tr_texture.id,
    };
    for ( int i = 0; i < to_draw.size(); ++i ) {
        glBindTexture( GL_TEXTURE_2D, to_draw[ i ] );

        auto [x, y, scale] = CalculateParameterPosition( i, 0 );

        glm::mat4 world = glm::translate( glm::vec3( x, y, 0.0f ) ) * glm::scale( scale );
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

        glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
    }

    glBindTexture( GL_TEXTURE_2D, 0 );

    glBindVertexArray( 0 );
    glUseProgram( 0 );

    std::array<std::pair<int, int>, 4> text_to_draw = {{
        { _temperature, model::MAX_TEMPERATURE },
        { _ocean_count, model::MAX_OCEAN_COUNT },
        { _oxygen_level, model::MAX_OXYGEN_LEVEL },
        { _tr, -1 },
    }};
    for ( int i = 0; i < text_to_draw.size(); ++i ) {
        auto& [current, max] = text_to_draw[ i ];
        glm::vec3 color = current == max ? POSITIVE_TEXT_COLOR : BASE_TEXT_COLOR;

        auto [x, y, _] = CalculateParameterPosition( i, 1 );
        TextRenderer::RenderTextCentered(
            std::to_string( current ),
            x,
            y,
            BASE_TEXT_SCALE,
            color
        );
    }
}

void View::RenderEndButton() {
    static const float button_ratio = (float)_button_texture.height / _temperature_texture.width;
    static const float size = 0.06f;
    static const float spacing = size * 2.5f;
    static const float ratio_modifier = 0.8f;

    float x = 1.0f - size - spacing / button_ratio;
    float y = 0.0f;
    glm::vec3 scale( size * button_ratio, size * _width / _height * ratio_modifier, 1.0f );

    glUseProgram( _program_rectangle_id );
    glBindVertexArray( _rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( ul( "image" ), 0 );

    glBindTexture( GL_TEXTURE_2D, _button_texture.id );

    glm::mat4 world = glm::translate( glm::vec3( x, y, 0.0f ) ) * glm::scale( scale );
    glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

    SetStencilRef( STENCIL_END );

    glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );

    SetStencilRef();

    glBindTexture( GL_TEXTURE_2D, 0 );

    glBindVertexArray( 0 );
    glUseProgram( 0 );

    static const glm::vec3 color = glm::vec3( 1280.f, 58.0f, 47.0f ) / 255.0f;
    TextRenderer::RenderTextCentered(
        "End Generation",
        x,
        y,
        1.0f,
        color
    );
}

void View::RenderResources() {
    glUseProgram( _program_rectangle_id );
    glBindVertexArray( _rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( ul( "image" ), 0 );

    glBindTexture( GL_TEXTURE_2D, _production_box_texture.id );
    for ( int i = 0; i <= +model::Resource::MAX; ++i ) {
        auto [x, y, scale] = CalculateResourcePosition( i, 0 );

        glm::mat4 world = glm::translate( glm::vec3( x, y, 0.0f ) ) * glm::scale( scale );
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

        glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
    }


    glUseProgram( _program_sprite_sheet_id );

    glUniform1i( ul( "image" ), 0 );

    glBindTexture( GL_TEXTURE_2D, _resources_texture.id );
    for ( int i = 0; i <= +model::Resource::MAX; ++i ) {
        static const float stride_x = 1.0f / RESOURCE_TEXTURE_COLUMNS;
        static const float stride_y = 1.0f / RESOURCE_TEXTURE_ROWS;
        //int res_index = +model::Resource::MAX - i;
        //int index_x = res_index % RESOURCE_TEXTURE_COLUMNS;
        //int index_y = res_index / RESOURCE_TEXTURE_COLUMNS;
        int index_x = i % RESOURCE_TEXTURE_COLUMNS;
        int index_y = i / RESOURCE_TEXTURE_COLUMNS;

        auto [x, y, scale] = CalculateResourcePosition( i, 1 );

        glm::mat4 world = glm::translate( glm::vec3( x, y, 0.0f ) ) * glm::scale( scale );
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

        glUniform1f( ul( "stride_x" ), stride_x );
        glUniform1f( ul( "stride_y" ), stride_y );
        glUniform1i( ul( "index_x" ), index_x );
        glUniform1i( ul( "index_y" ), index_y );

        glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
    }

    glBindTexture( GL_TEXTURE_2D, 0 );

    glBindVertexArray( 0 );
    glUseProgram( 0 );


    for ( int i = 0; i <= +model::Resource::MAX; ++i ) {

        auto [x, y, _] = CalculateResourcePosition( i, 0 );
        TextRenderer::RenderTextCentered(
            std::to_string( _resource_productions[ i ] ),
            x,
            y,
            BASE_TEXT_SCALE,
            glm::vec3( 0.0f )
        );

        std::tie( x, y, _ ) = CalculateResourcePosition( i, 2 );
        TextRenderer::RenderTextCentered(
            std::to_string( _resources[ i ] ),
            x,
            y,
            BASE_TEXT_SCALE,
            glm::vec3( 1.0f )
        );
    }
}

void View::RenderHand() {
    glUseProgram( _program_sprite_sheet_id );
    glBindVertexArray( _rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, _cards_texture.id );
    glUniform1i( ul( "image" ), 0 );

    for ( int i = 0; i < _hand.size(); ++i )
        RenderCard( *_hand[ i ], i );

    glBindTexture( GL_TEXTURE_2D, 0 );

    glBindVertexArray( 0 );
    glUseProgram( 0 );
}

void View::RenderCard( CardWrapper& card, int index ) {
    static const float stride_x = 1.0f / CARD_TEXTURE_COLUMNS;
    static const float stride_y = 1.0f / CARD_TEXTURE_ROWS;
    int corrected_card_id = +card->get_card_id() - 1;
    int index_x = corrected_card_id % CARD_TEXTURE_COLUMNS;
    int index_y = corrected_card_id / CARD_TEXTURE_COLUMNS;

    static const float card_ratio = (float)CARD_TEXTURE_WIDTH / CARD_TEXTURE_HEIGHT;
    float card_width = card_ratio / _width * _height;

    float z = card.state == CardWrapper::HOVERED ||
              card.state == CardWrapper::DRAGGING ?
        -0.1f : (index + 1) / -10000.0f;

    glm::vec3 scale( *card.scale * card_width, *card.scale, 1.0f );
    glm::vec3 translate( *card.pos, z );

    glm::mat4 world = glm::translate( translate ) * glm::scale( scale );
    glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

    glUniform1f( ul( "stride_x" ), stride_x );
    glUniform1f( ul( "stride_y" ), stride_y );
    glUniform1i( ul( "index_x" ), index_x );
    glUniform1i( ul( "index_y" ), index_y );

    glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
}

uint8_t View::GetStencilValue( float mouse_x, float mouse_y ) {
    uint8_t value;
    glReadPixels( (GLint)mouse_x, _height - (GLint)mouse_y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &value );
    return value;
}

std::pair<float, float> View::CalculateMousePos( float mouse_x, float mouse_y ) {
    return {
                   mouse_x  / _width  * 2.0f - 1.0f,
        (_height - mouse_y) / _height * 2.0f - 1.0f
    };
}

int view::View::CalculateHoveredCardByPos( float x, float y ) {
    static const float card_ratio = (float)CARD_TEXTURE_WIDTH / CARD_TEXTURE_HEIGHT;
    float card_width = CARD_BASE_SCALE * card_ratio / _width * _height;

    if ( y > _hand_top_y ||
         x < _hand_start_x - card_width ||
         x >= _hand_end_x + card_width )
        return -1;

    float interval_length = (_hand_end_x - _hand_start_x) / (_hand.size() - 1);
    float at = (x - _hand_start_x + interval_length / 2.0f) / interval_length;
    return glm::clamp<int>( static_cast<int>( at ), 0, (int)_hand.size() - 1 );
}

std::tuple<float, float, glm::vec3> View::CalculateParameterPosition( int parameter, int type ) {
    static const float temperature_ratio = (float)_temperature_texture.height / _temperature_texture.width;
    static const float ocean_ratio = (float)_ocean_texture.height / _ocean_texture.width;
    static const float oxygen_ratio = (float)_oxygen_texture.height / _oxygen_texture.width;
    static const float tr_ratio = (float)_tr_texture.height / _tr_texture.width;
    static const float size = 0.06f;
    static const float spacing = size * 2.5f;
    static const float padding = size * 0.5f;

    float x = 1.0f - size - type * spacing / 2.0f;
    float size_x = size / _width * _height;

    float temperature_y = size * temperature_ratio;
    if ( parameter == 0 ) return {
        x,
        1.0f - temperature_y - padding,
        glm::vec3( size_x, size * temperature_ratio, 1.0f )
    };

    float ocean_y = size * ocean_ratio;
    if ( parameter == 1 ) return {
        x,
        1.0f - temperature_y * 2.0f - ocean_y - padding * 2.0f,
        glm::vec3( size_x, size * ocean_ratio, 1.0f )
    };

    float oxygen_y = size * oxygen_ratio;
    if ( parameter == 2 ) return {
        x,
        1.0f - temperature_y * 2.0f - ocean_y * 2.0f - oxygen_y - padding * 3.0f,
        glm::vec3( size_x, size * oxygen_ratio, 1.0f )
    };

    float tr_y = size * tr_ratio;
    if ( parameter == 3 ) return {
        x,
        1.0f - temperature_y * 2.0f - ocean_y * 2.0f - oxygen_y * 2.0f - tr_y - padding * 4.0f,
        glm::vec3( size_x, size * tr_ratio, 1.0f )
    };

    throw std::logic_error( "View::CalculateParameterPosition: received invalid parameter!" );
}

std::tuple<float, float, glm::vec3> View::CalculateResourcePosition( int resource, int type ) {
    static const float size = 0.055f;
    static const float spacing = size * 2.5f;
    return {
        1.0f - size - type * spacing / 2.0f,
        (+model::Resource::MAX - resource) * spacing + size * 2.0f - 1.0f,
        glm::vec3( size / _width * _height , size, 1.0f )
    };
}

#pragma endregion Rendering

#pragma region Init and Clean

void View::InitShaders() {
    _program_id = glCreateProgram();
    AttachShader( _program_id, GL_VERTEX_SHADER, "shaders/pos_norm_tex.vert" );
    AttachShader( _program_id, GL_FRAGMENT_SHADER, "shaders/lighting.frag" );
    LinkProgram( _program_id );

    _program_rectangle_id = glCreateProgram();
    AttachShader( _program_rectangle_id, GL_VERTEX_SHADER, "shaders/rectangle.vert" );
    AttachShader( _program_rectangle_id, GL_FRAGMENT_SHADER, "shaders/rectangle.frag" );
    LinkProgram( _program_rectangle_id );

    _program_sprite_sheet_id = glCreateProgram();
    AttachShader( _program_sprite_sheet_id, GL_VERTEX_SHADER, "shaders/sprite_sheet.vert" );
    AttachShader( _program_sprite_sheet_id, GL_FRAGMENT_SHADER, "shaders/sprite_sheet.frag" );
    LinkProgram( _program_sprite_sheet_id );
}

void View::CleanShaders() {
    glDeleteProgram( _program_id );
    glDeleteProgram( _program_rectangle_id );
    glDeleteProgram( _program_sprite_sheet_id );
}

void View::InitGeometry() {
    MeshObject<VertexF> hexagon_cpu;

    glm::vec3 position( 0.0f, 1.0f, 0.0f );
    glm::vec3 normal( 0.0f, 0.0f, 1.0f );
    glm::vec2 texcoord( 0.0f, 0.0f );
    float on_edge = 1.0f;
    hexagon_cpu.vertex_array.emplace_back( glm::vec3( 0.0f ), normal, texcoord, 0.0f );

    for ( int i = 0; i < 6; ++i ) {
        hexagon_cpu.vertex_array.emplace_back( position, normal, texcoord, on_edge );

        static const glm::mat4 rotate_sixth = glm::rotate( glm::pi<float>() / 3.0f, glm::vec3( 0.0f, 0.0f, 1.0f ) );
        position = (rotate_sixth * glm::vec4( position, 1.0f )).xyz;
    }
    for ( int i = 1; i <= 6; ++i ) {
        hexagon_cpu.index_array.push_back( 0 );
        hexagon_cpu.index_array.push_back( i );
        hexagon_cpu.index_array.push_back( i % 6 + 1 );
    }

    _hexagon_gpu = CreateGLObjectFromMesh( hexagon_cpu, _vertex_plus_attribute_list );


    MeshObject<VertexPosTex> rectangle_cpu = {
        std::vector<VertexPosTex> {
            { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
            { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
            { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } },
            { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } },
        },
        std::vector<GLuint> {
            0, 1, 2,
            2, 1, 3,
        }
    };

    _rectangle_gpu = CreateGLObjectFromMesh( rectangle_cpu, _vertex_pos_tex_attribute_list );
}

void View::CleanGeometry() {
    CleanOGLObject( _hexagon_gpu );
    CleanOGLObject( _rectangle_gpu );
}

void View::InitTextures() {
    _cards_texture = LoadTexture( "assets/cards.png" );
    _resources_texture = LoadTexture( "assets/resources.png" );
    _card_cover_texture = LoadTexture( "assets/card_cover.png" );
    _temperature_texture = LoadTexture( "assets/temperature.png" );
    _ocean_texture = LoadTexture( "assets/ocean.png" );
    _oxygen_texture = LoadTexture( "assets/oxygen.png" );
    _tr_texture = LoadTexture( "assets/tr.png" );
    _button_texture = LoadTexture( "assets/button.png" );
    _production_box_texture = LoadTexture( "assets/production_box.png" );
}

void View::CleanTextures() {
    glDeleteTextures( 1, &_cards_texture.id );
    glDeleteTextures( 1, &_resources_texture.id );
    glDeleteTextures( 1, &_card_cover_texture.id );
    glDeleteTextures( 1, &_temperature_texture.id );
    glDeleteTextures( 1, &_ocean_texture.id );
    glDeleteTextures( 1, &_oxygen_texture.id );
    glDeleteTextures( 1, &_tr_texture.id );
    glDeleteTextures( 1, &_production_box_texture.id );
}

Texture View::LoadTexture( const std::filesystem::path& filename, GLint wrap_behaviour ) {
    ImageRGBA cards = ImageFromFile( filename );
    Texture tex = { 0, cards.width, cards.height };

    glGenTextures( 1, &tex.id );
    glBindTexture( GL_TEXTURE_2D, tex.id );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, cards.width, cards.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cards.data() );
    glGenerateMipmap( GL_TEXTURE_2D );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_behaviour );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_behaviour );

    return tex;
}

#pragma endregion Init and Clean

#pragma region Constants

const std::pair<float, float>& View::GetBoardOrigin() {
    static const std::pair<float, float> origin( 4.0f, 4.0f );
    return origin;
}

const std::initializer_list<VertexAttributeDescriptor> View::_vertex_pos_tex_attribute_list =
{
    { 0, offsetof( VertexPosTex, position ), 3, GL_FLOAT },
    { 1, offsetof( VertexPosTex, texcoord ), 2, GL_FLOAT },
};

const std::initializer_list<VertexAttributeDescriptor> View::_vertex_attribute_list =
{
    { 0, offsetof( Vertex, position ), 3, GL_FLOAT },
    { 1, offsetof( Vertex, normal   ), 3, GL_FLOAT },
    { 2, offsetof( Vertex, texcoord ), 2, GL_FLOAT },  
};

const std::initializer_list<VertexAttributeDescriptor> View::_vertex_plus_attribute_list =
{
    { 0, offsetof( VertexF, position ), 3, GL_FLOAT },
    { 1, offsetof( VertexF, normal   ), 3, GL_FLOAT },
    { 2, offsetof( VertexF, texcoord ), 2, GL_FLOAT },
    { 3, offsetof( VertexF, plus     ), 1, GL_FLOAT },
};

#pragma endregion Constants
}
