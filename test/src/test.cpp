#include <bandit/bandit.h>
#include "nm_vec4f.h"
#include "nm_vec4d.h"
#include "nm_vec8f.h"

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
      it( "can store values to aligned array (temporal)", [&]()
      {
        nmath_16b_align float vals[4] = { 0.0f };
        vec4f v( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f );
        v.storeTemporal( vals );
        nm_assert_float( vals[0], 9.9999999f );
        nm_assert_float( vals[1], -435745.11f );
        nm_assert_float( vals[2], 3482922.5434f );
        nm_assert_float( vals[3], -11111111.0f );
      } );
      it( "can store values to aligned array (nontemporal)", [&]()
      {
        nmath_16b_align float vals[4] = { 0.0f };
        vec4f v( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f );
        v.storeNontemporal( vals );
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
      it( "can store values to aligned array (temporal)", [&]()
      {
        nmath_32b_align double vals[4] = { 0 };
        vec4d v( 9.9999999, -435745.11, 3482922.5434, -11111111 );
        v.storeTemporal( vals );
        nm_assert_double( vals[0], 9.9999999 );
        nm_assert_double( vals[1], -435745.11 );
        nm_assert_double( vals[2], 3482922.5434 );
        nm_assert_double( vals[3], -11111111 );
      } );
      it( "can store values to aligned array (nontemporal)", [&]()
      {
        nmath_32b_align double vals[4] = { 0 };
        vec4d v( 9.9999999, -435745.11, 3482922.5434, -11111111 );
        v.storeNontemporal( vals );
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

    // VEC8F ------------------------------------------------------------------

    describe( "vec8f", []()
    {
      it( "defaults to zero constructor", [&]()
      {
        vec8f v;
        nm_assert_float_zero( v.e0 );
        nm_assert_float_zero( v.e1 );
        nm_assert_float_zero( v.e2 );
        nm_assert_float_zero( v.e3 );
        nm_assert_float_zero( v.e4 );
        nm_assert_float_zero( v.e5 );
        nm_assert_float_zero( v.e6 );
        nm_assert_float_zero( v.e7 );
      } );
      it( "supports single scalar constructor", [&]()
      {
        vec8f v( 13.37f );
        nm_assert_float( v.e0, 13.37f );
        nm_assert_float( v.e1, 13.37f );
        nm_assert_float( v.e2, 13.37f );
        nm_assert_float( v.e3, 13.37f );
        nm_assert_float( v.e4, 13.37f );
        nm_assert_float( v.e5, 13.37f );
        nm_assert_float( v.e6, 13.37f );
        nm_assert_float( v.e7, 13.37f );
      } );
      it( "supports individual scalar constructor", [&]()
      {
        vec8f v( 13.37f, 9344.01f, 12211.59f, -340.007f, 3849.513f, -1.00045f, 12312.456f, 5834.19800051f );
        nm_assert_float( v.e0, 13.37f );
        nm_assert_float( v.e1, 9344.01f );
        nm_assert_float( v.e2, 12211.59f );
        nm_assert_float( v.e3, -340.007f );
        nm_assert_float( v.e4, 3849.513f );
        nm_assert_float( v.e5, -1.00045f );
        nm_assert_float( v.e6, 12312.456f );
        nm_assert_float( v.e7, 5834.19800051f );
      } );
      it( "can load values from aligned array", [&]()
      {
        nmath_32b_align float vals[8] = {
          0.0001f,
          956.112f,
          -4350.555f,
          -1.11111111111f,
          8934.123f,
          -0.000153111f,
          41191994524.0f,
          -1.935f
        };
        vec8f v;
        nm_assert_float_zero( v.e0 );
        v.load( vals );
        nm_assert_float( v.e0, vals[0] );
        nm_assert_float( v.e1, vals[1] );
        nm_assert_float( v.e2, vals[2] );
        nm_assert_float( v.e3, vals[3] );
        nm_assert_float( v.e4, vals[4] );
        nm_assert_float( v.e5, vals[5] );
        nm_assert_float( v.e6, vals[6] );
        nm_assert_float( v.e7, vals[7] );
      } );
      it( "can load values from unaligned array", [&]()
      {
        float vals[8] = {
          0.0001f,
          956.112f,
          -4350.555f,
          -1.11111111111f,
          8934.123f,
          -0.000153111f,
          41191994524.0f,
          -1.935f
        };
        vec8f v;
        nm_assert_float_zero( v.e0 );
        v.loadUnaligned( vals );
        nm_assert_float( v.e0, vals[0] );
        nm_assert_float( v.e1, vals[1] );
        nm_assert_float( v.e2, vals[2] );
        nm_assert_float( v.e3, vals[3] );
        nm_assert_float( v.e4, vals[4] );
        nm_assert_float( v.e5, vals[5] );
        nm_assert_float( v.e6, vals[6] );
        nm_assert_float( v.e7, vals[7] );
      } );
      it( "can extract low and high parts as vec4", [&]()
      {
        float vals[8] = {
          0.0001f,
          956.112f,
          -4350.555f,
          -1.11111111111f,
          8934.123f,
          -0.000153111f,
          41191994524.0f,
          -1.935f
        };
        vec8f v;
        nm_assert_float_zero( v.e0 );
        v.loadUnaligned( vals );
        const auto lo = v.low();
        const auto hi = v.high();
        nm_assert_float( lo.x, vals[0] );
        nm_assert_float( lo.y, vals[1] );
        nm_assert_float( lo.z, vals[2] );
        nm_assert_float( lo.w, vals[3] );
        nm_assert_float( hi.x, vals[4] );
        nm_assert_float( hi.y, vals[5] );
        nm_assert_float( hi.z, vals[6] );
        nm_assert_float( hi.w, vals[7] );
      } );
      it( "can store values to aligned array (temporal)", [&]()
      {
        nmath_32b_align float vals[8] = { 0.0f };
        vec8f v( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f, 8934.123f, -0.000153111f, -1.935f, 9999.99999999f );
        v.storeTemporal( vals );
        nm_assert_float( vals[0], 9.9999999f );
        nm_assert_float( vals[1], -435745.11f );
        nm_assert_float( vals[2], 3482922.5434f );
        nm_assert_float( vals[3], -11111111.0f );
        nm_assert_float( vals[4], 8934.123f );
        nm_assert_float( vals[5], -0.000153111f );
        nm_assert_float( vals[6], -1.935f );
        nm_assert_float( vals[7], 9999.99999999f );
      } );
      it( "can store values to aligned array (nontemporal)", [&]()
      {
        nmath_32b_align float vals[8] = { 0.0f };
        vec8f v( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f, 8934.123f, -0.000153111f, -1.935f, 9999.99999999f );
        v.storeNontemporal( vals );
        nm_assert_float( vals[0], 9.9999999f );
        nm_assert_float( vals[1], -435745.11f );
        nm_assert_float( vals[2], 3482922.5434f );
        nm_assert_float( vals[3], -11111111.0f );
        nm_assert_float( vals[4], 8934.123f );
        nm_assert_float( vals[5], -0.000153111f );
        nm_assert_float( vals[6], -1.935f );
        nm_assert_float( vals[7], 9999.99999999f );
      } );
      it( "can store values to unaligned array", [&]()
      {
        float vals[8] = { 0.0f };
        vec8f v( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f, 8934.123f, -0.000153111f, -1.935f, 9999.99999999f );
        v.storeUnaligned( vals );
        nm_assert_float( vals[0], 9.9999999f );
        nm_assert_float( vals[1], -435745.11f );
        nm_assert_float( vals[2], 3482922.5434f );
        nm_assert_float( vals[3], -11111111.0f );
        nm_assert_float( vals[4], 8934.123f );
        nm_assert_float( vals[5], -0.000153111f );
        nm_assert_float( vals[6], -1.935f );
        nm_assert_float( vals[7], 9999.99999999f );
      } );
      it( "can set all members at once", [&]()
      {
        vec8f v( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f, 8934.123f, -0.000153111f, -1.935f, 9999.99999999f );
        float val = 73.313f;
        v.set( &val );
        nm_assert_float( v.e0, 73.313f );
        nm_assert_float( v.e1, 73.313f );
        nm_assert_float( v.e2, 73.313f );
        nm_assert_float( v.e3, 73.313f );
        nm_assert_float( v.e4, 73.313f );
        nm_assert_float( v.e5, 73.313f );
        nm_assert_float( v.e6, 73.313f );
        nm_assert_float( v.e7, 73.313f );
      } );
      it( "can set all members individually at once", [&]()
      {
        vec8f v( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f, 8934.123f, -0.000153111f, -1.935f, 9999.99999999f );
        v.set( 34.13f, -43.0f, 846943.13601f, -0.0000001f, -340.007f, 3849.513f, -1.00045f, 12312.456f );
        nm_assert_float( v.e0, 34.13f );
        nm_assert_float( v.e1, -43.0f );
        nm_assert_float( v.e2, 846943.13601f );
        nm_assert_float( v.e3, -0.0000001f );
        nm_assert_float( v.e4, -340.007f );
        nm_assert_float( v.e5, 3849.513f );
        nm_assert_float( v.e6, -1.00045f );
        nm_assert_float( v.e7, 12312.456f );
      } );
      it( "compares correctly (equality)", [&]()
      {
        vec8f a( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f, 8934.123f, -0.000153111f, -1.935f, 9999.99999999f );
        vec8f b( 9.9999999f, -435745.11f, 3482922.5434f, -11111111.0f, 8934.123f, -0.000153111f, -1.935f, 9999.99999999f );
        vec8f c( 9.9999999f, -435745.11f, 3483922.5434f, -11111111.0f, 8934.123f, -0.000153111f, -1.935f, 9999.99999999f );
        AssertThat( a == b, Is().True() );
        AssertThat( a == c, Is().False() );
      } );
      it( "compares correctly (inequality)", [&]()
      {
        vec8f a( 1.0f, 2.0f, 3.0f, 4.5f, 6.7f, 7.8f, 9.1001f, 34968.32781f );
        vec8f b( 1.0f, 2.0f, 3.0f, 4.5f, 6.7f, 7.8f, 9.1001f, 34968.32781f );
        vec8f nequals[8] = {
          ( 2.0f, 2.0f, 3.0f, 4.5f, 6.7f, 7.8f, 9.1001f, 34968.32781f ),
          ( 1.0f, 3.0f, 3.0f, 4.5f, 6.7f, 7.8f, 9.1001f, 34968.32781f ),
          ( 1.0f, 2.0f, 4.0f, 4.5f, 6.7f, 7.8f, 9.1001f, 34968.32781f ),
          ( 1.0f, 2.0f, 3.0f, 5.0f, 6.7f, 7.8f, 9.1001f, 34968.32781f ),
          ( 1.0f, 3.0f, 3.0f, 4.5f, 6.7f, 7.6f, 9.1001f, 34968.32781f ),
          ( 1.0f, 2.0f, 4.0f, 4.5f, 6.7f, 7.8f, 9.1000f, 34968.32781f ),
          ( 1.0f, 2.0f, 3.0f, 5.0f, 6.7f, 7.8f, 9.1001f, -34968.32781f )
        };
        AssertThat( a != b, Is().False() );
        for ( const auto& nequal : nequals )
          AssertThat( a != nequal, Is().True() );
      } );
      it( "adds correctly (vec8f)", [&]()
      {
        vec8f a( 0.0001f, -2355.5f, 42872.0f, 0.29343f, 1.0f, 5.0f, -5.0f, 2.5f );
        vec8f b( 0.0064f, 64.5f, 9125.0f, -34965.0f, 3.0f, -2.0f, 1.0f, 2.5f );
        auto c = a + b;
        nm_assert_float( c.e0, 0.0065f );
        nm_assert_float( c.e1, -2291.0f );
        nm_assert_float( c.e2, 51997.0f );
        nm_assert_float( c.e3, -34964.70657f );
        nm_assert_float( c.e4, 4.0f );
        nm_assert_float( c.e5, 3.0f );
        nm_assert_float( c.e6, -4.0f );
        nm_assert_float( c.e7, 5.0f );
      } );
      it( "adds correctly (scalar)", [&]()
      {
        vec8f a( 0.0001f, -2355.5f, 42872.0f, 0.29343f, 93456.4102f, 1111.0f, -34634.5f, 1953.0f );
        float b = 3.1337f;
        auto c = a + b;
        nm_assert_float( c.e0, 3.1338f );
        nm_assert_float( c.e1, -2352.3663f );
        nm_assert_float( c.e2, 42875.1337f );
        nm_assert_float( c.e3, 3.42713f );
        nm_assert_float( c.e4, 93459.5439f );
        nm_assert_float( c.e5, 1114.1337f );
        nm_assert_float( c.e6, -34631.3663f );
        nm_assert_float( c.e7, 1956.1337f );
      } );
      it( "subtracts correctly (vec8f)", [&]()
      {
        vec8f a( 0.0001f, -2355.5f, 42872.0f, 0.29343f, 1.0f, 5.0f, -5.0f, 3.5f );
        vec8f b( 0.0064f, 64.5f, 9125.0f, -34965.0f, 3.0f, -2.0f, 1.0f, 2.5f );
        auto c = a - b;
        nm_assert_float( c.e0, -0.0063f );
        nm_assert_float( c.e1, -2420.0f );
        nm_assert_float( c.e2, 33747.0f );
        nm_assert_float( c.e3, 34965.29343f );
        nm_assert_float( c.e4, -2.0f );
        nm_assert_float( c.e5, 7.0f );
        nm_assert_float( c.e6, -6.0f );
        nm_assert_float( c.e7, 1.0f );
      } );
      it( "subtracts correctly (scalar)", [&]()
      {
        vec8f a( 0.0001f, -2355.5f, 42872.0f, 0.29343f, 1.0f, 5.0f, -5.0f, 2.5f );
        float b = -10.0f;
        auto c = a - b;
        nm_assert_float( c.e0, 10.0001f );
        nm_assert_float( c.e1, -2345.5f );
        nm_assert_float( c.e2, 42882.0f );
        nm_assert_float( c.e3, 10.29343f );
        nm_assert_float( c.e4, 11.0f );
        nm_assert_float( c.e5, 15.0f );
        nm_assert_float( c.e6, 5.0f );
        nm_assert_float( c.e7, 12.5f );
      } );
      it( "multiplies correctly (vec8f)", [&]()
      {
        vec8f a( 0.0001f, -2355.5f, 42872.0f, 0.29343f, 93456.4102f, 1111.0f, -34634.5f, 1953.0f );
        vec8f b( 0.0064f, 64.5f, 9125.0f, -34965.0f, 1.0f, 5.0f, -5.0f, 3.5f );
        auto c = a * b;
        nm_assert_float( c.e0, 0.00000064f );
        nm_assert_float( c.e1, -151929.75f );
        nm_assert_float( c.e2, 391207000.0f );
        nm_assert_float( c.e3, -10259.77995f );
        nm_assert_float( c.e4, 93456.4102f );
        nm_assert_float( c.e5, 5555.0f );
        nm_assert_float( c.e6, 173172.5f );
        nm_assert_float( c.e7, 6835.5f );
      } );
      it( "divides correctly (vec8f)", [&]()
      {
        vec8f a( 0.0064f, -2355.5f, 42872.0f, -34965.0f, 93456.4102f, 1111.0f, -34634.5f, 1953.0f );
        vec8f b( 0.0001f, 64.5f, 9125.0f, 0.29343f, 1.0f, 5.0f, -5.0f, 3.5f );
        auto c = a / b;
        nm_assert_float( c.e0, 64.0f );
        nm_assert_float( c.e1, -36.5193798449612f );
        nm_assert_float( c.e2, 4.698301369863014f );
        nm_assert_float( c.e3, -119159.5951334219f );
        nm_assert_float( c.e4, 93456.4102f );
        nm_assert_float( c.e5, 222.2f );
        nm_assert_float( c.e6, 6926.9f );
        nm_assert_float( c.e7, 558.0f );
      } );
      it( "divides correctly (scalar)", [&]()
      {
        vec8f a( 0.0064f, -2355.5f, 42872.0f, -34965.0f, 1.0f, 5.0f, -5.0f, 3.5f );
        float b = 2.0f;
        auto c = a / b;
        nm_assert_float( c.e0, 0.0032f );
        nm_assert_float( c.e1, -1177.75f );
        nm_assert_float( c.e2, 21436.0f );
        nm_assert_float( c.e3, -17482.5f );
        nm_assert_float( c.e4, 0.5f );
        nm_assert_float( c.e5, 2.5f );
        nm_assert_float( c.e6, -2.5f );
        nm_assert_float( c.e7, 1.75f );
      } );
      it( "sums correctly (horizontal)", [&]()
      {
        vec8f a( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f );
        nm_assert_float( a.sum(), 36.0f );
        vec8f b( 0.0001f, -2355.5f, 42872.0f, 0.29343f, 346.2f, -93.1f, 11.1111f, -999.999f );
        nm_assert_float( b.sum(), 39781.00563f );
      } );
      it( "unpacks 8 to 4x4 correctly", [&]()
      {
        float values[8] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
        vec8f v( values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7] );
        vec8f parts[4];
        v.unpack8x4( parts[0], parts[1], parts[2], parts[3] );
        for ( int i = 0; i < 4; ++i )
        {
          nm_assert_float( parts[i].e0, values[i * 2] );
          nm_assert_float( parts[i].e1, values[i * 2] );
          nm_assert_float( parts[i].e2, values[i * 2] );
          nm_assert_float( parts[i].e3, values[i * 2] );
          nm_assert_float( parts[i].e4, values[i * 2 + 1] );
          nm_assert_float( parts[i].e5, values[i * 2 + 1] );
          nm_assert_float( parts[i].e6, values[i * 2 + 1] );
          nm_assert_float( parts[i].e7, values[i * 2 + 1] );
        }
      } );
    } );

  } );
} );

int main( int argc, char* argv[] )
{
  return bandit::run( argc, argv );
}