#ifndef NM_VEC8F_H
#define NM_VEC8F_H

#include "nm_common.h"

namespace nmath {

  nmath_32b_align class vec8f {
  public:
    union {
      struct {
        float e0, e1, e2, e3, e4, e5, e6, e7;
      };
      __m256 packed;
    };
    nmath_inline vec8f()
    {
      packed = _mm256_setzero_ps();
    }
    nmath_inline vec8f( const float value )
    {
      packed = _mm256_broadcast_ss( &value );
    }
    nmath_inline vec8f( const float* __restrict values )
    {
      packed = _mm256_load_ps( values );
    }
    nmath_inline vec8f( const __m256& rhs ): packed( rhs )
    {
    }
    nmath_inline vec8f( float _e0, float _e1, float _e2, float _e3, float _e4, float _e5, float _e6, float _e7 )
    {
      packed = _mm256_setr_ps( _e0, _e1, _e2, _e3, _e4, _e5, _e6, _e7 );
    }
    nmath_inline vec8f( const vec4f& lo, const vec4f& hi )
    {
      packed = _mm256_insertf128_ps( _mm256_castps128_ps256( lo.packed ), ( hi.packed ), 1 );
    }
    //! Load eight 32-byte boundary aligned sample values into vector
    nmath_inline void load( const float* __restrict values )
    {
      packed = _mm256_load_ps( values );
    }
    //! Load eight unaligned sample values into vector
    nmath_inline void loadUnaligned( const float* __restrict values )
    {
      packed = _mm256_loadu_ps( values );
    }
    //! Store eight sample values from vector to 32-byte boundary aligned memory
    //! using a non-temporal usage hint (data not reused, don't cache in SSB)
    //! treating nontemporal data as temporal would be cache pollution = bad
    nmath_inline void storeNontemporal( float* __restrict values )
    {
      _mm256_stream_ps( values, packed );
    }
    //! Store eight sample values from vector to 32-byte boundary aligned memory
    //! using a temporal usage hint (data is reused, maybe cache in SSB)
    //! subsequent stores of values cached in SSB can bypass L1, L2 & memctrl = nice
    nmath_inline void storeTemporal( float* __restrict values )
    {
      _mm256_store_ps( values, packed );
    }
    //! Store eight sample values from vector to unaligned memory
    nmath_inline void storeUnaligned( float* __restrict values )
    {
      _mm256_storeu_ps( values, packed );
    }
    //! Set all vector members to single value
    nmath_inline void set( const float* value )
    {
      packed = _mm256_broadcast_ss( value );
    }
    //! Set vector members individually
    nmath_inline void set( float _e0, float _e1, float _e2, float _e3, float _e4, float _e5, float _e6, float _e7 )
    {
      packed = _mm256_setr_ps( _e0, _e1, _e2, _e3, _e4, _e5, _e6, _e7 );
    }
    //! v = a + b
    nmath_inline vec8f operator + ( const vec8f& rhs ) const
    {
      return _mm256_add_ps( packed, rhs.packed );
    }
    //! v = a - b
    nmath_inline vec8f operator - ( const vec8f& rhs ) const
    {
      return _mm256_sub_ps( packed, rhs.packed );
    }
    //! v = a & b (bitwise and)
    nmath_inline vec8f operator & ( const vec8f& rhs ) const
    {
      return _mm256_and_ps( packed, rhs.packed );
    }
    //! v = a | b (bitwise or)
    nmath_inline vec8f operator | ( const vec8f& rhs ) const
    {
      return _mm256_or_ps( packed, rhs.packed );
    }
    //! v = a ^ b (bitwise xor)
    nmath_inline vec8f operator ^ ( const vec8f& rhs ) const
    {
      return _mm256_xor_ps( packed, rhs.packed );
    }
    //! v = a * b
    nmath_inline vec8f operator * ( const float scalar ) const
    {
      const auto im = _mm256_set1_ps( scalar );
      return _mm256_mul_ps( packed, im );
    }
    //! v = a * b
    nmath_inline vec8f operator * ( const vec8f& rhs ) const
    {
      return _mm256_mul_ps( packed, rhs.packed );
    }
    //! v = a / b
    nmath_inline vec8f operator / ( const float scalar ) const
    {
      const auto im = _mm256_set1_ps( scalar );
      return _mm256_div_ps( packed, im );
    }
    //! v = a / b
    nmath_inline vec8f operator / ( const vec8f& rhs ) const
    {
      return _mm256_div_ps( packed, rhs.packed );
    }
    //! a == b
    nmath_inline bool operator == ( const vec8f& rhs ) const
    {
      const auto ret = _mm256_cmp_ps( packed, rhs.packed, _CMP_NEQ_OS );
      return ( _mm256_movemask_ps( ret ) == 0 );
    }
    //! a != b
    nmath_inline bool operator != ( const vec8f& rhs ) const
    {
      const auto ret = _mm256_cmp_ps( packed, rhs.packed, _CMP_NEQ_OS );
      return ( _mm256_movemask_ps( ret ) != 0 );
    }
    //! v = round(a)
    nmath_inline vec8f round() const
    {
      return _mm256_round_ps( packed, _MM_FROUND_NINT );
    }
    //! v = ceil(a)
    nmath_inline vec8f ceil() const
    {
      return _mm256_round_ps( packed, _MM_FROUND_CEIL );
    }
    //! v = floor(a)
    nmath_inline vec8f floor() const
    {
      return _mm256_round_ps( packed, _MM_FROUND_FLOOR );
    }
    //! v = trunc(a)
    nmath_inline vec8f trunc() const
    {
      return _mm256_round_ps( packed, _MM_FROUND_TRUNC );
    }
    //! v = sqrt(a)
    nmath_inline vec8f sqrt() const
    {
      return _mm256_sqrt_ps( packed );
    }
    //! f = e0 + e1 + e2 + e3 + e4 + e5 + e6 + e7
    nmath_inline float sum() const
    {
      // this would be much more compact using permute2f128_ps and hadd_ps,
      // but that combo is (or used to be?) less performant.
      const auto hi = _mm256_extractf128_ps( packed, 1 );
      const auto lo = _mm256_castps256_ps128( packed );
      const auto qsum = _mm_add_ps( lo, hi );
      const auto dlo = qsum;
      const auto dhi = _mm_movehl_ps( qsum, qsum );
      const auto dsum = _mm_add_ps( dlo, dhi );
      const auto rlo = dsum;
      const auto rhi = _mm_shuffle_ps( dsum, dsum, 1 );
      const auto rsum = _mm_add_ss( rlo, rhi );
      return _mm_cvtss_f32( rsum );
    }
    //! Distribute each of the 8 elements into blocks of four:
    //! [1,2,3,4,5,6,7,8]
    //! =>
    //! a [1,1,1,1,2,2,2,2]
    //! b [3,3,3,3,4,4,4,4]
    //! c [5,5,5,5,6,6,6,6]
    //! d [7,7,7,7,8,8,8,8]
    nmath_inline void unpack8x4( vec8f& a, vec8f& b, vec8f& c, vec8f& d )
    {
      const auto hipart = _mm256_extractf128_ps( packed, 1 );
      const auto lopart = _mm256_castps256_ps128( packed );
      auto d0 = _mm_permute_ps( lopart, 0b00000000 );
      auto d1 = _mm_permute_ps( lopart, 0b01010101 );
      auto dummy256 = _mm256_castps128_ps256( d0 );
      a.packed = _mm256_insertf128_ps( dummy256, d1, 1 );
      d0 = _mm_permute_ps( lopart, 0b10101010 );
      d1 = _mm_permute_ps( lopart, 0b11111111 );
      dummy256 = _mm256_castps128_ps256( d0 );
      b.packed = _mm256_insertf128_ps( dummy256, d1, 1 );
      d0 = _mm_permute_ps( hipart, 0b00000000 );
      d1 = _mm_permute_ps( hipart, 0b01010101 );
      dummy256 = _mm256_castps128_ps256( d0 );
      c.packed = _mm256_insertf128_ps( dummy256, d1, 1 );
      d0 = _mm_permute_ps( hipart, 0b10101010 );
      d1 = _mm_permute_ps( hipart, 0b11111111 );
      dummy256 = _mm256_castps128_ps256( d0 );
      d.packed = _mm256_insertf128_ps( dummy256, d1, 1 );
    }
    //! v = a * b + c (fused multiply & add)
    nmath_inline vec8f fma( const vec8f& a, const vec8f& b, const vec8f& c )
    {
      return _mm256_fmadd_ps( a.packed, b.packed, c.packed );
    }
    //! v = a * b - c (fused multiply & subtract)
    nmath_inline vec8f fms( const vec8f& a, const vec8f& b, const vec8f& c )
    {
      return _mm256_fmsub_ps( a.packed, b.packed, c.packed );
    }
    //! Extract the lower vec4
    nmath_inline vec4f low()
    {
      return vec4f( _mm256_castps256_ps128( packed ) );
    }
    //! Extract the higher vec4
    nmath_inline vec4f high()
    {
      return vec4f( _mm256_extractf128_ps( packed, 1 ) );
    }
  };

}

#endif