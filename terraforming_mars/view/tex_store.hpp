#pragma once

#include <glm/glm.hpp>

#include "gl_utils/gl_utils.hpp"

namespace view::TexStore
{
bool Init();
void Clean();

inline GLuint program_id = 0;
inline GLuint program_card_id = 0;
inline GLuint program_rectangle_id = 0;
inline GLuint program_sprite_sheet_id = 0;

void InitShaders();
void CleanShaders();

inline OGLObject hexagon_gpu = {};
inline OGLObject rectangle_gpu = {};

void InitGeometry();
void CleanGeometry();

inline Texture cards_texture = {};
inline Texture resources_texture = {};

inline Texture temperature_texture = {};
inline Texture oxygen_texture = {};
inline Texture tr_texture = {};

inline Texture ocean_texture = {};
inline Texture greenery_texture = {};
inline Texture city_texture = {};

inline Texture button_short_texture = {};
inline Texture button_long_texture = {};
inline Texture production_box_texture = {};
inline Texture arrow_texture = {};
inline Texture player_icon_texture = {};
inline Texture card_cover_texture = {};

inline Texture action_closed_texture = {};
inline Texture action_open_texture = {};
inline Texture event_closed_texture = {};
inline Texture event_open_texture = {};
inline Texture automated_closed_texture = {};
inline Texture automated_open_texture = {};
inline Texture effect_closed_texture = {};
inline Texture effect_open_texture = {};

void InitTextures();
void CleanTextures();
}
