#include "pch.h"

#include <string>

#include "../model//event.h"

using namespace model;

namespace test
{
TEST( EventTest, TestNoParam ) {
    Event<> e;
    e.Invoke();

    int call_count = 0;
    e.SetCallback( [ &call_count ]() { ++call_count; } );

    e.Invoke();
    EXPECT_EQ( 1, call_count );

    e.Invoke();
    EXPECT_EQ( 2, call_count );

    e.ClearCallback();
    e.Invoke();
    EXPECT_EQ( 2, call_count );
}

TEST( EventTest, TestWithParams ) {
    Event<double, std::string> e;
    e.Invoke( 2.0, "a" );;

    int call_count = 0;
    double p_d;
    std::string p_s;
    e.SetCallback( [ & ]( double d, std::string s ) { ++call_count; p_d = d; p_s = s; } );

    e.Invoke( 3.0, "b" );
    EXPECT_EQ( 1, call_count );
    EXPECT_EQ( 3.0, p_d );
    EXPECT_EQ( "b", p_s );
    
    e.Invoke( 4.0, "c" );
    EXPECT_EQ( 2, call_count );
    EXPECT_EQ( 4.0, p_d );
    EXPECT_EQ( "c", p_s );

    e.ClearCallback();
    e.Invoke( 5.0, "d" );
    EXPECT_EQ( 2, call_count );
    EXPECT_EQ( 4.0, p_d );
    EXPECT_EQ( "c", p_s );
}
}
