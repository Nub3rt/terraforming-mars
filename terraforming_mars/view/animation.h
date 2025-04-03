#pragma once

#include "animation.fwd.h"
#include "view.fwd.h"

#include <string>
#include <functional>

#include <glm/glm.hpp>

#include "animatable.h"

namespace view
{
class Animation
{
public:
    virtual ~Animation() = default;

    inline bool HasLockout() const noexcept { return _lockout_time != 0.0f; }

    void Update( float delta );
    virtual void Render( View* view ) = 0;
    virtual bool IsOver() const noexcept = 0;

protected:
    float _lockout_time;

    Animation( float lockout_time = 0.0f );

    virtual void DoUpdate( float delta ) = 0;
};

class InstantAnimation : public Animation
{
public:
    InstantAnimation( std::function<void()> perform );
    InstantAnimation( float lockout_time, std::function<void()> perform );

    void Render( View* view ) override;
    bool IsOver() const noexcept override;

    std::function<void()> perform;

protected:
    bool _performed = false;

    void DoUpdate( float delta ) override;
};

class TextAnimation : public Animation
{
public:
    TextAnimation( float duration, std::string text, glm::vec2 start_pos, glm::vec2 end_pos,
        glm::vec4 start_color, float start_scale );
    TextAnimation( float lockout_time, float duration, std::string text, glm::vec2 start_pos, glm::vec2 end_pos,
        glm::vec4 start_color, float start_scale );
    TextAnimation( float duration, std::string text, glm::vec2 start_pos, glm::vec2 end_pos,
        glm::vec4 start_color, glm::vec4 end_color, float start_scale );
    TextAnimation( float lockout_time, float duration, std::string text, glm::vec2 start_pos, glm::vec2 end_pos,
        glm::vec4 start_color, glm::vec4 end_color, float start_scale );
    TextAnimation( float duration, std::string text, glm::vec2 start_pos, glm::vec2 end_pos,
        glm::vec4 start_color, glm::vec4 end_color, float start_scale, float end_scale );
    TextAnimation( float lockout_time, float duration, std::string text, glm::vec2 start_pos, glm::vec2 end_pos,
        glm::vec4 start_color, glm::vec4 end_color, float start_scale, float end_scale );

    void Render( View* view ) override;
    bool IsOver() const noexcept override;

    std::string text;
    Animatable<glm::vec2> pos;
    Animatable<glm::vec4> color;
    Animatable<float> scale;

protected:
    void DoUpdate( float delta ) override;
};
}
