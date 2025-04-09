#pragma once

#include <glm/glm.hpp>

namespace view
{
inline constexpr int STARTING_WINDOW_WIDTH = 1600;
inline constexpr int STARTING_WINDOW_HEIGHT = 900;

inline constexpr float SKYBOX_ROTATE_SPEED = 0.01f;

inline constexpr float BASE_TEXT_SCALE = 1.5f;
inline constexpr glm::vec3 POSITIVE_TEXT_COLOR = glm::vec3( 0x0f, 0x86, 0x08 ) / 255.0f;
inline constexpr glm::vec3 NEGATIVE_TEXT_COLOR = glm::vec3( 0xff, 0x08, 0x08 ) / 255.0f;
inline constexpr glm::vec3 PRODUCTION_TEXT_COLOR = glm::vec3( 0xA8, 0x74, 0x4B ) / 255.0f;

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
inline constexpr float CARD_DRAG_OUT_LINE_Y = -0.5f;

inline constexpr float ATTRIBUTE_CHANGED_LOCKOUT = 1.0f;
inline constexpr float ATTRIBUTE_CHANGED_DURATION = 3.0f;
inline constexpr float TEXT_FLOAT_DISTANCE = 0.5f;

inline constexpr uint8_t STENCIL_NONE = 0xff;

inline constexpr uint8_t STENCIL_MENU      = 0x01;
inline constexpr uint8_t STENCIL_END       = 0x02;
inline constexpr uint8_t STENCIL_EVENTS    = 0x03;
inline constexpr uint8_t STENCIL_AUTOMATED = 0x04;
inline constexpr uint8_t STENCIL_EFFECTS   = 0x05;
inline constexpr uint8_t STENCIL_ACTIONS   = 0x06;
inline constexpr uint8_t STENCIL_STARTING_BOARD = 0x07;


inline constexpr int CARD_TEXTURE_ROWS = 13;
inline constexpr int CARD_TEXTURE_COLUMNS = 16;
inline constexpr int CARD_TEXTURE_WIDTH = 556;
inline constexpr int CARD_TEXTURE_HEIGHT = 686;

inline constexpr int RESOURCE_TEXTURE_ROWS = 1;
inline constexpr int RESOURCE_TEXTURE_COLUMNS = 6;
}
