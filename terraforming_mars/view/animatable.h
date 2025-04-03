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

    inline const T* operator->() const noexcept { return &_value; }
    inline T* operator->() noexcept { return &_value; }
    inline const T& operator*() const noexcept { return _value; }
    inline T& operator*() noexcept { return _value; }

    inline bool Animating() const {
        return _a.has_value();
    }

    inline void Set( T value ) {
        _a.reset();
        _value = value;
    }

    inline void UpdateAnim( T value, float duration, bool force_time = false ) {
        if ( _a ) {
            _a->end = value;
            if ( force_time ) {
                _a->elapsed = 0;
                _a->duration = duration;
            }
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

    inline const glm::vec2* operator->() const noexcept { return &_value; }
    inline glm::vec2* operator->() noexcept { return &_value; }
    inline const glm::vec2& operator*() const noexcept { return _value; }
    inline glm::vec2& operator*() noexcept { return _value; }

    inline bool Animating() const {
        return x.Animating() || y.Animating();
    }

    inline void Set( glm::vec2 value ) {
        x.Set( value.x );
        y.Set( value.y );
        _value = value;
    }

    inline void UpdateAnim( glm::vec2 value, float duration, bool force_time = false ) {
        x.UpdateAnim( value.x, duration, force_time );
        y.UpdateAnim( value.y, duration, force_time );
        _value.x = *x;
        _value.y = *y;
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

private:
    glm::vec2 _value;
};

template<>
class Animatable<glm::vec3>
{
public:
    using ease_t = std::function<float( float )>;

    Animatable( glm::vec3 value ) : x( value.x ), y( value.y ), z( value.z ), _value( value ) {}
    Animatable() : x(), y(), z(), _value() {}

    inline const glm::vec3* operator->() const noexcept { return &_value; }
    inline glm::vec3* operator->() noexcept { return &_value; }
    inline const glm::vec3& operator*() const noexcept { return _value; }
    inline glm::vec3& operator*() noexcept { return _value; }

    inline bool Animating() const {
        return x.Animating() || y.Animating() || z.Animating();
    }

    inline void Set( glm::vec3 value ) {
        x.Set( value.x );
        y.Set( value.y );
        z.Set( value.z );
        _value = value;
    }

    inline void UpdateAnim( glm::vec3 value, float duration, bool force_time = false ) {
        x.UpdateAnim( value.x, duration, force_time );
        y.UpdateAnim( value.y, duration, force_time );
        z.UpdateAnim( value.z, duration, force_time );
        _value.x = *x;
        _value.y = *y;
        _value.z = *z;
    }

    inline void SetAnim( glm::vec3 start, glm::vec3 end, float duration ) {
        x.SetAnim( start.x, end.x, duration );
        y.SetAnim( start.y, end.y, duration );
        z.SetAnim( start.z, end.z, duration );
    }

    inline void SetEase( ease_t func ) {
        x.ease = func;
        y.ease = func;
        z.ease = func;
    }

    inline void Update( float delta ) {
        x.Update( delta );
        y.Update( delta );
        z.Update( delta );
        _value.x = *x;
        _value.y = *y;
        _value.z = *z;
    }

    Animatable<float> x;
    Animatable<float> y;
    Animatable<float> z;

private:
    glm::vec3 _value;
};

template<>
class Animatable<glm::vec4>
{
public:
    using ease_t = std::function<float( float )>;

    Animatable( glm::vec4 value ) : x( value.x ), y( value.y ), z( value.z ), w( value.w ), _value( value ) {}
    Animatable() : x(), y(), z(), w(), _value() {}

    inline const glm::vec4* operator->() const noexcept { return &_value; }
    inline glm::vec4* operator->() noexcept { return &_value; }
    inline const glm::vec4& operator*() const noexcept { return _value; }
    inline glm::vec4& operator*() noexcept { return _value; }

    inline bool Animating() const {
        return x.Animating() || y.Animating() || z.Animating() || w.Animating();
    }

    inline void Set( glm::vec4 value ) {
        x.Set( value.x );
        y.Set( value.y );
        z.Set( value.z );
        w.Set( value.w );
        _value = value;
    }

    inline void UpdateAnim( glm::vec4 value, float duration, bool force_time = false ) {
        x.UpdateAnim( value.x, duration, force_time );
        y.UpdateAnim( value.y, duration, force_time );
        z.UpdateAnim( value.z, duration, force_time );
        w.UpdateAnim( value.w, duration, force_time );
        _value.x = *x;
        _value.y = *y;
        _value.z = *z;
        _value.w = *w;
    }

    inline void SetAnim( glm::vec4 start, glm::vec4 end, float duration ) {
        x.SetAnim( start.x, end.x, duration );
        y.SetAnim( start.y, end.y, duration );
        z.SetAnim( start.z, end.z, duration );
        w.SetAnim( start.w, end.w, duration );
    }

    inline void SetEase( ease_t func ) {
        x.ease = func;
        y.ease = func;
        z.ease = func;
        w.ease = func;
    }

    inline void Update( float delta ) {
        x.Update( delta );
        y.Update( delta );
        z.Update( delta );
        w.Update( delta );
        _value.x = *x;
        _value.y = *y;
        _value.z = *z;
        _value.w = *w;
    }

    Animatable<float> x;
    Animatable<float> y;
    Animatable<float> z;
    Animatable<float> w;

private:
    glm::vec4 _value;
};
}
