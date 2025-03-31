#pragma once

#include <glm/glm.hpp>

namespace view
{
inline constexpr int STARTING_WINDOW_WIDTH = 1600;
inline constexpr int STARTING_WINDOW_HEIGHT = 900;

inline constexpr float SKYBOX_ROTATE_SPEED = 0.01f;

inline constexpr float HAND_BASE_Y = -1.12f;
inline constexpr glm::vec2 CARD_BASE_SCALE = glm::vec2( 0.25f );

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
