#pragma once

#include <glm/glm.hpp>

#include "../model/constants.hpp"

namespace view
{
inline constexpr int STARTING_WINDOW_WIDTH = 1600;
inline constexpr int STARTING_WINDOW_HEIGHT = 900;

inline constexpr float SKYBOX_ROTATE_SPEED = 0.01f;

inline constexpr glm::vec2 BASE_HINT_POS = glm::vec2( 0.0f, 0.9f );
inline constexpr float BASE_TEXT_SCALE = 1.5f;
inline constexpr float CREDIT_TEXT_SCALE = 1.0f;
inline constexpr glm::vec3 LIGHT_TEXT_COLOR = glm::vec3( 0xff, 0xff, 0xff ) / 255.0f;
inline constexpr glm::vec3 DARK_TEXT_COLOR = glm::vec3( 0x00, 0x00, 0x00 ) / 255.0f;
inline constexpr glm::vec3 POSITIVE_TEXT_COLOR = glm::vec3( 0x0f, 0x86, 0x08 ) / 255.0f;
inline constexpr glm::vec3 NEGATIVE_TEXT_COLOR = glm::vec3( 0xff, 0x08, 0x08 ) / 255.0f;
inline constexpr glm::vec3 PRODUCTION_TEXT_COLOR = glm::vec3( 0xA8, 0x74, 0x4B ) / 255.0f;

inline constexpr glm::vec3 TILE_COLOR_EMPTY    = glm::vec3( 0xff, 0x55, 0x55 ) / 255.0f;
inline constexpr glm::vec3 TILE_COLOR_OCEAN    = glm::vec3( 0x61, 0xa0, 0xcc ) / 255.0f;
inline constexpr glm::vec3 TILE_COLOR_GREENERY = glm::vec3( 0x4e, 0x92, 0x42 ) / 255.0f;
inline constexpr glm::vec3 TILE_COLOR_CITY     = glm::vec3( 0xe0, 0xe0, 0xe0 ) / 255.0f;
inline constexpr glm::vec3 TILE_BORDER_COLOR_IDLE           = glm::vec3( 0xff, 0x6b, 0x08 ) / 255.0f;
inline constexpr glm::vec3 TILE_BORDER_COLOR_FOR_OCEAN      = glm::vec3( 0x08, 0xe6, 0xff ) / 255.0f;
inline constexpr glm::vec3 TILE_BORDER_COLOR_FOR_NOCTIS     = glm::vec3( 0xbb, 0xbb, 0xbb ) / 255.0f;
inline constexpr glm::vec3 TILE_BORDER_COLOR_NON_SELECTABLE = glm::vec3( 0x60, 0x60, 0x60 ) / 255.0f;
inline constexpr glm::vec3 TILE_BORDER_COLOR_SELECTABLE     = glm::vec3( 0xf0, 0xf0, 0xf0 ) / 255.0f;
inline constexpr float TILE_CHANGE_DURATION = 0.5f;

inline constexpr float HAND_BASE_Y = -1.12f;
inline constexpr float CARD_BASE_SCALE = 0.25f;
inline constexpr float CARD_ADJUST_DURATION = 0.5;
inline constexpr float CARD_DRAW_IN_DURATION = 2.0f;
inline constexpr float CARD_DRAW_DOWN_DURATION = 1.5f;
inline constexpr float CARD_DRAW_TOTAL_DURATION = CARD_DRAW_IN_DURATION + CARD_DRAW_DOWN_DURATION;
inline constexpr float CARD_DRAW_LOCKOUT_DURATION = CARD_DRAW_IN_DURATION + 0.5f;
inline constexpr glm::vec2 CARD_DRAW_POS_START = { 1.2f, -0.4f };
inline constexpr float CARD_DRAW_SCALE_START = CARD_BASE_SCALE * 1.2f;
inline constexpr float CARD_DRAW_ROTATE_START = 0.0f;
inline constexpr glm::vec2 CARD_DRAW_POS_MIDDLE = { 0.5f, 0.0f };
inline constexpr float CARD_DRAW_SCALE_MIDDLE = CARD_BASE_SCALE * 1.8f;
inline constexpr float CARD_DRAW_ROTATE_MIDDLE = 0.0f;
inline constexpr float CARD_DRAG_OUT_LINE_Y = -0.2f;

inline constexpr glm::vec3 CARD_HIGHLIGHT_COLOR        = glm::vec3( 0x00, 0xff, 0x30 ) / 255.0f;
inline constexpr glm::vec3 CARD_ACTION_HIGHLIGHT_COLOR = glm::vec3( 0x00, 0x80, 0xff ) / 255.0f;
inline constexpr glm::vec3 CARD_SELL_HIGHLIGHT_COLOR   = glm::vec3( 0xa0, 0x00, 0x00 ) / 255.0f;

inline constexpr float RESEARCH_CARD_SCALE = CARD_DRAW_SCALE_MIDDLE * 0.8f;
inline constexpr float RESEARCH_CARD_ROTATE = 0.0f;
inline constexpr float CARD_DRAW_UP_Y = 1.5f;

inline constexpr float DEFAULT_LOCKOUT_DURATION = 1.0f;
inline constexpr float ATTRIBUTE_CHANGED_DURATION = 3.0f;
inline constexpr float TEXT_FLOAT_DISTANCE = 0.5f;

inline constexpr uint8_t STENCIL_NONE = 0xff;

inline constexpr uint8_t STENCIL_MENU      = 0x01;
inline constexpr uint8_t STENCIL_END       = 0x02;
inline constexpr uint8_t STENCIL_EVENTS    = 0x03;
inline constexpr uint8_t STENCIL_AUTOMATED = 0x04;
inline constexpr uint8_t STENCIL_EFFECTS   = 0x05;
inline constexpr uint8_t STENCIL_ACTIONS   = 0x06;
inline constexpr uint8_t STENCIL_SP_SELL_PATENTS   = 0x07;
inline constexpr uint8_t STENCIL_SP_POWER_PLANT    = 0x08;
inline constexpr uint8_t STENCIL_SP_ASTEROID       = 0x09;
inline constexpr uint8_t STENCIL_SP_AQUIFER        = 0x0a;
inline constexpr uint8_t STENCIL_SP_GREENERY       = 0x0b;
inline constexpr uint8_t STENCIL_SP_CITY           = 0x0c;
inline constexpr uint8_t STENCIL_SP_CONVERT_PLANTS = 0x0d;
inline constexpr uint8_t STENCIL_SP_CONVERT_HEAT   = 0x0e;
inline constexpr uint8_t STENCIL_STARTING_SP = STENCIL_SP_SELL_PATENTS;
inline constexpr uint8_t STENCIL_STARTING_RESEARCH = 0x0f;
inline constexpr uint8_t STENCIL_STARTING_BOARD = STENCIL_STARTING_RESEARCH + model::RESEARCH_CARD_NUM;


inline constexpr int CARD_TEXTURE_ROWS = 13;
inline constexpr int CARD_TEXTURE_COLUMNS = 16;
inline constexpr int CARD_TEXTURE_WIDTH = 556;
inline constexpr int CARD_TEXTURE_HEIGHT = 686;

inline constexpr int RESOURCE_TEXTURE_ROWS = 1;
inline constexpr int RESOURCE_TEXTURE_COLUMNS = 6;

inline constexpr float HUD_BASE_Z = 0.8f;
inline constexpr float MOUSE_HOVER_SIZE_MULTIPLIER = 1.1f;
inline constexpr glm::vec3 BUTTON_TEXT_COLOR = glm::vec3( 1280.f, 58.0f, 47.0f ) / 255.0f;
inline constexpr float PRODUCTION_RESOURCE_SHRINK = 0.6f;
}
