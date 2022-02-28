#include <bandit/bandit.h>
#include "nm_vec4f.h"
#include "nm_vec4d.h"

using namespace snowhouse;
using namespace bandit;
using namespace nmath;

#define nm_assert_float(target, value) AssertThat(target,EqualsWithDelta(value,0.000001)); \
  AssertThat(target,!Equals(-(value))); \
  AssertThat(target,!Equals(value+0.00001));

#define nm_assert_float_zero(target) AssertThat(target,EqualsWithDelta(0.0f,0.0000001f)); \
  AssertThat(target,Equals(-0.0f)); \
  AssertThat(target,!Equals(0.0000001f));

#define nm_assert_double(target, value) AssertThat(target,EqualsWithDelta(value,0.000001)); \
  AssertThat(target,!Equals(-(value))); \
  AssertThat(target,!Equals(value+0.00001));

#define nm_assert_double_zero(target) AssertThat(target,EqualsWithDelta(0.0,0.0000001)); \
  AssertThat(target,Equals(-0.0)); \
  AssertThat(target,!Equals(0.0000001));

go_bandit( []()
{
  describe( "nmath", []()
  {
    // VEC4F ------------------------------------------------------------------
    describe( "vec4f", []()
    {
      it( "defaults to zero constructor", [&]()
      {
        vec4f v;
        nm_assert_float_zero( v.x );
        nm_assert_float_zero( v.y );
        nm_assert_float_zero( v.z );
        nm_assert_float_zero( v.w );
      } );
      it( "supports single scalar constructor", [&]()
      {
        vec4f v( 13.37f );
        nm_assert_float( v.x, 13.37f );
        nm_assert_float( v.y, 13.37f );
        nm_assert_float( v.z, 13.37f );
        nm_assert_float( v.w, 13.37f );
      } );
      it( "supports individual scalar constructor", [&]()
      {
        vec4f v( 13.37f, 9344.01f, 12211.59f, -340.007f );
        nm_assert_float( v.x, 13.37f );
        nm_assert_float( v.y, 9344.01f );
        nm_assert_float( v.z, 12211.59f );
        nm_assert_float( v.w, -340.007f );
      } );
      it( "can load values from aligned array", [&]()
      {
        nmath_16b_align float vals[4] = {
          0.0001f,
          956.112f,
          -4350.555f,
          -1.11111111111f
        };
        vec4f v;
        nm_assert_float_zero( v.x );
        v.load( vals );
        nm_assert_float( v.x, vals[0] );
        nm_assert_float( v.y, vals[1] );
        nm_assert_float( v.z, vals[2] );
        nm_assert_float( v.w, vals[3] );
      } );
      it( "can load values from unaligned array", [&]()
      {
        float vals[4] = {
          0.0001f,
          956.112f,
          -4350.555f,
          -1.11111111111f
        };
        vec4f v;
        nm_assert_float_zero( v.x );
        v.loadUnaligned( vals );
        nm_assert_float( v.x, vals[0] );
        nm_assert_float( v.y, vals[1] );
        nm_assert_float( v.z, vals[2] );
        nm_assert_float( v.w, vals[3] );
      } );
      it( "can store values to aligned array", [&]()
      {
        nmath_16b_align float vals[4] = { 0.0f };
        vec4f v( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f );
        v.store( vals );
        nm_assert_float( vals[0], 9.9999999f );
        nm_assert_float( vals[1], -435745.11f );
        nm_assert_float( vals[2], 3482922.5434f );
        nm_assert_float( vals[3], -11111111.0f );
      } );
      it( "can store values to unaligned array", [&]()
      {
        float vals[4] = { 0.0f };
        vec4f v( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f );
        v.storeUnaligned( vals );
        nm_assert_float( vals[0], 9.9999999f );
        nm_assert_float( vals[1], -435745.11f );
        nm_assert_float( vals[2], 3482922.5434f );
        nm_assert_float( vals[3], -11111111.0f );
      } );
      it( "can set all members at once", [&]()
      {
        vec4f v( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f );
        float val = 73.313f;
        v.set( &val );
        nm_assert_float( v.x, 73.313f );
        nm_assert_float( v.y, 73.313f );
        nm_assert_float( v.z, 73.313f );
        nm_assert_float( v.w, 73.313f );
      } );
      it( "can set all members individually at once", [&]()
      {
        vec4f v( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f );
        v.set( 34.13f, -43.0f, 846943.13601f, -0.0000001f );
        nm_assert_float( v.x, 34.13f );
        nm_assert_float( v.y, -43.0f );
        nm_assert_float( v.z, 846943.13601f );
        nm_assert_float( v.w, -0.0000001f );
      } );
      it( "compares correctly (equality)", [&]()
      {
        vec4f a( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f );
        vec4f b( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f );
        vec4f c( 9.9999999f, -435745.11f, 3483922.5434f, -11111111.0f );
        AssertThat( a == b, Is().True() );
        AssertThat( a == c, Is().False() );
      } );
      it("compares correctly (inequality)", [&]()
      {
        vec4f a( 1.0f, 2.0f, 3.0f, 4.5f );
        vec4f b( 1.0f, 2.0f, 3.0f, 4.5f );
        vec4f nequals[4] = {
          ( 2.0f, 2.0f, 3.0f, 4.5f ),
          ( 1.0f, 3.0f, 3.0f, 4.5f ),
          ( 1.0f, 2.0f, 4.0f, 4.5f ),
          ( 1.0f, 2.0f, 3.0f, 5.0f )
        };
        AssertThat( a != b, Is().False() );
        for ( const auto& nequal : nequals )
          AssertThat( a != nequal, Is().True() );
      } );
      it( "adds correctly (vec4f)", [&]()
      {
        vec4f a( 0.0001f, -2355.5f, 42872.0f, 0.29343f );
        vec4f b( 0.0064f, 64.5f, 9125.0f, -34965.0f );
        auto c = a + b;
        nm_assert_float( c.x, 0.0065f );
        nm_assert_float( c.y, -2291.0f );
        nm_assert_float( c.z, 51997.0f );
        nm_assert_float( c.w, -34964.70657f );
      } );
      it( "adds correctly (scalar)", [&]()
      {
        vec4f a( 0.0001f, -2355.5f, 42872.0f, 0.29343f );
        float b = 3.1337f;
        auto c = a + b;
        nm_assert_float( c.x, 3.1338f );
        nm_assert_float( c.y, -2352.3663f );
        nm_assert_float( c.z, 42875.1337f );
        nm_assert_float( c.w, 3.42713f );
      } );
      it( "subtracts correctly (vec4f)", [&]()
      {
        vec4f a( 0.0001f, -2355.5f, 42872.0f, 0.29343f );
        vec4f b( 0.0064f, 64.5f, 9125.0f, -34965.0f );
        auto c = a - b;
        nm_assert_float( c.x, -0.0063f );
        nm_assert_float( c.y, -2420.0f );
        nm_assert_float( c.z, 33747.0f );
        nm_assert_float( c.w, 34965.29343f );
      } );
      it( "subtracts correctly (scalar)", [&]()
      {
        vec4f a( 0.0001f, -2355.5f, 42872.0f, 0.29343f );
        float b = -10.0f;
        auto c = a - b;
        nm_assert_float( c.x, 10.0001f );
        nm_assert_float( c.y, -2345.5f );
        nm_assert_float( c.z, 42882.0f );
        nm_assert_float( c.w, 10.29343f );
      } );
      it( "multiplies correctly (vec4f)", [&]()
      {
        vec4f a( 0.0001f, -2355.5f, 42872.0f, 0.29343f );
        vec4f b( 0.0064f, 64.5f, 9125.0f, -34965.0f );
        auto c = a * b;
        nm_assert_float( c.x, 0.00000064f );
        nm_assert_float( c.y, -151929.75f );
        nm_assert_float( c.z, 391207000.0f );
        nm_assert_float( c.w, -10259.77995f );
      } );
      it( "divides correctly (vec4f)", [&]()
      {
        vec4f a( 0.0064f, -2355.5f, 42872.0f, -34965.0f );
        vec4f b( 0.0001f, 64.5f, 9125.0f, 0.29343f );
        auto c = a / b;
        nm_assert_float( c.x, 64.0f );
        nm_assert_float( c.y, -36.5193798449612f );
        nm_assert_float( c.z, 4.698301369863014f );
        nm_assert_float( c.w, -119159.5951334219f );
      } );
      it( "divides correctly (scalar)", [&]()
      {
        vec4f a( 0.0064f, -2355.5f, 42872.0f, -34965.0f );
        float b = 2.0f;
        auto c = a / b;
        nm_assert_float( c.x, 0.0032f );
        nm_assert_float( c.y, -1177.75f );
        nm_assert_float( c.z, 21436.0f );
        nm_assert_float( c.w, -17482.5f );
      } );
      it( "sums correctly (horizontal)", [&]()
      {
        vec4f a( 1.0f, 2.0f, 3.0f, 4.0f );
        nm_assert_float( a.sum(), 10.0f );
        vec4f b( 0.0001f, -2355.5f, 42872.0f, 0.29343f );
        nm_assert_float( b.sum(), 40516.79353f );
      } );
    } );

    // VEC4D ------------------------------------------------------------------
    describe( "vec4d", []()
    {
      it( "defaults to zero constructor", [&]()
      {
        vec4d v;
        nm_assert_double_zero( v.x );
        nm_assert_double_zero( v.y );
        nm_assert_double_zero( v.z );
        nm_assert_double_zero( v.w );
      } );
      it( "supports single scalar constructor", [&]()
      {
        vec4d v( 13.37 );
        nm_assert_double( v.x, 13.37 );
        nm_assert_double( v.y, 13.37 );
        nm_assert_double( v.z, 13.37 );
        nm_assert_double( v.w, 13.37 );
      } );
      it( "supports individual scalar constructor", [&]()
      {
        vec4d v( 13.37, 9344.01, 12211.59, -340.007 );
        nm_assert_double( v.x, 13.37 );
        nm_assert_double( v.y, 9344.01 );
        nm_assert_double( v.z, 12211.59 );
        nm_assert_double( v.w, -340.007 );
      } );
      it( "can load values from aligned array", [&]()
      {
        nmath_32b_align double vals[4] = {
          0.0001,
          956.112,
          -4350.555,
          -1.11111111111
        };
        vec4d v;
        nm_assert_double_zero( v.x );
        v.load( vals );
        nm_assert_double( v.x, vals[0] );
        nm_assert_double( v.y, vals[1] );
        nm_assert_double( v.z, vals[2] );
        nm_assert_double( v.w, vals[3] );
      } );
      it( "can load values from unaligned array", [&]()
      {
        double vals[4] = {
          0.0001,
          956.112,
          -4350.555,
          -1.11111111111
        };
        vec4d v;
        nm_assert_double_zero( v.x );
        v.loadUnaligned( vals );
        nm_assert_double( v.x, vals[0] );
        nm_assert_double( v.y, vals[1] );
        nm_assert_double( v.z, vals[2] );
        nm_assert_double( v.w, vals[3] );
      } );
      it( "can store values to aligned array", [&]()
      {
        nmath_32b_align double vals[4] = { 0 };
        vec4d v( 9.9999999, -435745.11, 3482922.5434, -11111111 );
        v.store( vals );
        nm_assert_double( vals[0], 9.9999999 );
        nm_assert_double( vals[1], -435745.11 );
        nm_assert_double( vals[2], 3482922.5434 );
        nm_assert_double( vals[3], -11111111 );
      } );
      it( "can store values to unaligned array", [&]()
      {
        double vals[4] = { 0 };
        vec4d v( 9.9999999, -435745.11, 3482922.5434, -11111111 );
        v.storeUnaligned( vals );
        nm_assert_double( vals[0], 9.9999999 );
        nm_assert_double( vals[1], -435745.11 );
        nm_assert_double( vals[2], 3482922.5434 );
        nm_assert_double( vals[3], -11111111 );
      } );
      it( "can set all members at once", [&]()
      {
        vec4d v( 9.9999999, -435745.11, 3482922.5434, -11111111 );
        double val = 73.313;
        v.set( &val );
        nm_assert_double( v.x, 73.313 );
        nm_assert_double( v.y, 73.313 );
        nm_assert_double( v.z, 73.313 );
        nm_assert_double( v.w, 73.313 );
      } );
      it( "can set all members individually at once", [&]()
      {
        vec4d v( 9.9999999, -435745.11, 3482922.5434, -11111111 );
        v.set( 34.13, -43, 846943.13601, -0.0000001 );
        nm_assert_double( v.x, 34.13 );
        nm_assert_double( v.y, -43 );
        nm_assert_double( v.z, 846943.13601 );
        nm_assert_double( v.w, -0.0000001 );
      } );
      it( "compares correctly (equality)", [&]()
      {
        vec4d a( 9.9999999, -435745.11, 3482922.5434, -11111111 );
        vec4d b( 9.9999999, -435745.11, 3482922.5434, -11111111 );
        vec4d c( 9.9999999, -435745.11, 3483922.5434, -11111111 );
        AssertThat( a == b, Is().True() );
        AssertThat( a == c, Is().False() );
      } );
      it( "compares correctly (inequality)", [&]()
      {
        vec4d a( 1.0, 2.0, 3.0, 4.5 );
        vec4d b( 1.0, 2.0, 3.0, 4.5 );
        vec4d nequals[4] = {
          ( 2.0, 2.0, 3.0, 4.5 ),
          ( 1.0, 3.0, 3.0, 4.5 ),
          ( 1.0, 2.0, 4.0, 4.5 ),
          ( 1.0, 2.0, 3.0, 5.0 )
        };
        AssertThat( a != b, Is().False() );
        for ( const auto& nequal : nequals )
          AssertThat( a != nequal, Is().True() );
      } );
      it( "adds correctly (vec4d)", [&]()
      {
        vec4d a( 0.0001, -2355.5, 42872.0, 0.29343 );
        vec4d b( 0.0064, 64.5, 9125.0, -34965.0 );
        auto c = a + b;
        nm_assert_double( c.x, 0.0065 );
        nm_assert_double( c.y, -2291.0 );
        nm_assert_double( c.z, 51997.0 );
        nm_assert_double( c.w, -34964.70657 );
      } );
      it( "adds correctly (scalar)", [&]()
      {
        vec4d a( 0.0001, -2355.5, 42872.0, 0.29343 );
        double b = 3.1337;
        auto c = a + b;
        nm_assert_double( c.x, 3.1338 );
        nm_assert_double( c.y, -2352.3663 );
        nm_assert_double( c.z, 42875.1337 );
        nm_assert_double( c.w, 3.42713 );
      } );
      it( "subtracts correctly (vec4d)", [&]()
      {
        vec4d a( 0.0001, -2355.5, 42872.0, 0.29343 );
        vec4d b( 0.0064, 64.5, 9125.0, -34965.0 );
        auto c = a - b;
        nm_assert_double( c.x, -0.0063 );
        nm_assert_double( c.y, -2420.0 );
        nm_assert_double( c.z, 33747.0 );
        nm_assert_double( c.w, 34965.29343 );
      } );
      it( "subtracts correctly (scalar)", [&]()
      {
        vec4d a( 0.0001, -2355.5, 42872.0, 0.29343 );
        double b = -10.0;
        auto c = a - b;
        nm_assert_double( c.x, 10.0001 );
        nm_assert_double( c.y, -2345.5 );
        nm_assert_double( c.z, 42882.0 );
        nm_assert_double( c.w, 10.29343 );
      } );
      it( "multiplies correctly (vec4d)", [&]()
      {
        vec4d a( 0.0001, -2355.5, 42872.0, 0.29343 );
        vec4d b( 0.0064, 64.5, 9125.0, -34965.0 );
        auto c = a * b;
        nm_assert_double( c.x, 0.00000064 );
        nm_assert_double( c.y, -151929.75 );
        nm_assert_double( c.z, 391207000 );
        nm_assert_double( c.w, -10259.77995 );
      } );
      it( "divides correctly (vec4d)", [&]()
      {
        vec4d a( 0.0064, -2355.5, 42872.0, -34965.0 );
        vec4d b( 0.0001, 64.5, 9125.0, 0.29343 );
        auto c = a / b;
        nm_assert_double( c.x, 64.0 );
        nm_assert_double( c.y, -36.5193798449612 );
        nm_assert_double( c.z, 4.698301369863014 );
        nm_assert_double( c.w, -119159.5951334219 );
      } );
      it( "divides correctly (scalar)", [&]()
      {
        vec4d a( 0.0064, -2355.5, 42872.0, -34965.0 );
        double b = 2.0;
        auto c = a / b;
        nm_assert_double( c.x, 0.0032 );
        nm_assert_double( c.y, -1177.75 );
        nm_assert_double( c.z, 21436.0 );
        nm_assert_double( c.w, -17482.5 );
      } );
      it( "sums correctly (horizontal)", [&]()
      {
        vec4d a( 1.0, 2.0, 3.0, 4.0 );
        nm_assert_double( a.sum(), 10.0 );
        vec4d b( 0.0001, -2355.5, 42872.0, 0.29343 );
        nm_assert_double( b.sum(), 40516.79353 );
      } );
    } );

  } );
} );

int main( int argc, char* argv[] )
{
  return bandit::run( argc, argv );
}