#pragma once

#include <functional>
#include <optional>
#include <glm/glm.hpp>
#include <glm/gtx/easing.hpp>

namespace view
{
template<typename T>
struct AnimationParams
{
    T start;
    T end;
    float elapsed = 0.0f;
    float duration;
};

template<typename T>
class Animatable
{
public:
    using ease_t = std::function<float( float )>;

    Animatable( T value ) : _value( value ) {}
    Animatable() : _value() {}

    inline T* operator->() noexcept { return &_value; }
    inline T& operator*() noexcept { return _value; }

    inline bool Animating() {
        return _a.has_value();
    }

    inline void Set( T value ) {
        _a.reset();
        _value = value;
    }

    inline void UpdateAnim( T value, float duration ) {
        if ( _a ) {
            _a->end = value;
        } else {
            _a = { _value , value, 0.0f, duration };
        }
    }

    inline void SetAnim( T start, T end, float duration ) {
        _a = { start, end, 0.0f, duration };
    }

    inline void SetEase( ease_t func ) { ease = func; }

    inline void Update( float delta ) {
        if ( !_a )
            return;

        _a->elapsed += delta;
        if ( _a->elapsed >= _a->duration ) {
            _value = _a->end;
            _a.reset();
        } else {
            float t = _a->elapsed / _a->duration;
            _value = _a->start + (_a->end - _a->start) * ease( t );
        }
    }

    ease_t ease = &glm::linearInterpolation<float>;

    friend bool operator==( const Animatable<T>& lhs, const Animatable<T>& rhs ) {
        return lhs._value == rhs._value;
    }

private:
    T _value;

    std::optional<AnimationParams<T>> _a = {};
};

template<>
class Animatable<glm::vec2>
{
public:
    using ease_t = std::function<float( float )>;

    Animatable( glm::vec2 value ) : x( value.x ), y( value.y ), _value( value ) {}
    Animatable() : x(), y(), _value() {}

    inline glm::vec2* operator->() noexcept { return &_value; }
    inline glm::vec2& operator*() noexcept { return _value; }

    inline bool Animating() {
        return x.Animating() || y.Animating();
    }

    inline void Set( glm::vec2 value ) {
        x.Set( value.x );
        y.Set( value.y );
    }

    inline void UpdateAnim( glm::vec2 value, float duration ) {
        x.UpdateAnim( value.x, duration );
        y.UpdateAnim( value.y, duration );
    }

    inline void SetAnim( glm::vec2 start, glm::vec2 end, float duration ) {
        x.SetAnim( start.x, end.x, duration );
        y.SetAnim( start.y, end.y, duration );
    }

    inline void SetEase( ease_t func ) {
        x.ease = func ;
        y.ease = func ;
    }

    inline void Update( float delta ) {
        x.Update( delta );
        y.Update( delta );
        _value.x = *x;
        _value.y = *y;
    }

    Animatable<float> x;
    Animatable<float> y;

    friend bool operator==( const Animatable<glm::vec2>& lhs, const Animatable<glm::vec2>& rhs ) {
        return lhs._value == rhs._value;
    }

private:
    glm::vec2 _value;
};
}
