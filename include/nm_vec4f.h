#ifndef NM_VEC4F_H
#define NM_VEC4F_H

#include "nm_common.h"

namespace nmath {

  nmath_16b_align class vec4f {
  public:
    union {
      struct {
        float x, y, z, w;
      };
      __m128 packed;
    };
    nmath_inline vec4f()
    {
      packed = _mm_setzero_ps();
    }
    nmath_inline vec4f( const float value )
    {
      packed = _mm_broadcast_ss( &value );
    }
    nmath_inline vec4f( const float* __restrict values )
    {
      packed = _mm_load_ps( values );
    }
    nmath_inline vec4f( const __m128& rhs ): packed( rhs )
    {
    }
    nmath_inline vec4f( float _x, float _y, float _z, float _w )
    {
      packed = _mm_setr_ps( _x, _y, _z, _w );
    }
    //! Load four 16-byte boundary aligned sample values into vector
    nmath_inline void load( const float* __restrict values )
    {
      packed = _mm_load_ps( values );
    }
    //! Load four unaligned sample values into vector
    nmath_inline void loadUnaligned( const float* __restrict values )
    {
      packed = _mm_loadu_ps( values );
    }
    //! Store four sample values from vector to 32-byte boundary aligned memory
    //! using a non-temporal usage hint (data not reused, don't cache in SSB)
    //! treating nontemporal data as temporal would be cache pollution = bad
    nmath_inline void storeNontemporal( float* __restrict values )
    {
      _mm_stream_ps( values, packed );
    }
    //! Store four sample values from vector to 32-byte boundary aligned memory
    //! using a temporal usage hint (data is reused, maybe cache in SSB)
    //! subsequent stores of values cached in SSB can bypass L1, L2 & memctrl = nice
    nmath_inline void storeTemporal( float* __restrict values )
    {
      _mm_store_ps( values, packed );
    }
    //! Store four sample values from vector to unaligned memory
    nmath_inline void storeUnaligned( float* __restrict values )
    {
      _mm_storeu_ps( values, packed );
    }
    //! Set all vector members to single value
    nmath_inline void set( const float* value )
    {
      packed = _mm_broadcast_ss( value );
    }
    //! Set vector members individually
    nmath_inline void set( float _x, float _y, float _z, float _w )
    {
      packed = _mm_setr_ps( _x, _y, _z, _w );
    }
    //! v = a + b
    nmath_inline vec4f operator + ( const vec4f& rhs ) const
    {
      return _mm_add_ps( packed, rhs.packed );
    }
    //! v = a - b
    nmath_inline vec4f operator - ( const vec4f& rhs ) const
    {
      return _mm_sub_ps( packed, rhs.packed );
    }
    //! v = a & b (bitwise and)
    nmath_inline vec4f operator & ( const vec4f& rhs ) const
    {
      return _mm_and_ps( packed, rhs.packed );
    }
    //! v = a | b (bitwise or)
    nmath_inline vec4f operator | ( const vec4f& rhs ) const
    {
      return _mm_or_ps( packed, rhs.packed );
    }
    //! v = a ^ b (bitwise xor)
    nmath_inline vec4f operator ^ ( const vec4f& rhs ) const
    {
      return _mm_xor_ps( packed, rhs.packed );
    }
    //! v = a * b
    nmath_inline vec4f operator * ( const float scalar ) const
    {
      const auto im = _mm_set1_ps( scalar );
      return _mm_mul_ps( packed, im );
    }
    //! v = a * b
    nmath_inline vec4f operator * ( const vec4f& rhs ) const
    {
      return _mm_mul_ps( packed, rhs.packed );
    }
    //! v = a / b
    nmath_inline vec4f operator / ( const float scalar ) const
    {
      const auto im = _mm_set1_ps( scalar );
      return _mm_div_ps( packed, im );
    }
    //! v = a / b
    nmath_inline vec4f operator / ( const vec4f& rhs ) const
    {
      return _mm_div_ps( packed, rhs.packed );
    }
    //! a == b
    nmath_inline bool operator == ( const vec4f& rhs ) const
    {
      const auto ret = _mm_cmp_ps( packed, rhs.packed, _CMP_NEQ_OS );
      return ( _mm_movemask_ps( ret ) == 0 );
    }
    //! a != b
    nmath_inline bool operator != ( const vec4f& rhs ) const
    {
      const auto ret = _mm_cmp_ps( packed, rhs.packed, _CMP_NEQ_OS );
      return ( _mm_movemask_ps( ret ) != 0 );
    }
    //! v = round(a)
    nmath_inline vec4f round() const
    {
      return _mm_round_ps( packed, _MM_FROUND_NINT );
    }
    //! v = ceil(a)
    nmath_inline vec4f ceil() const
    {
      return _mm_round_ps( packed, _MM_FROUND_CEIL );
    }
    //! v = floor(a)
    nmath_inline vec4f floor() const
    {
      return _mm_round_ps( packed, _MM_FROUND_FLOOR );
    }
    //! v = trunc(a)
    nmath_inline vec4f trunc() const
    {
      return _mm_round_ps( packed, _MM_FROUND_TRUNC );
    }
    //! v = sqrt(a)
    nmath_inline vec4f sqrt() const
    {
      return _mm_sqrt_ps( packed );
    }
    //! f = x + y + z + w
    nmath_inline float sum() const
    {
      // hadd_ps used to be discouraged, dunno what the consensus is today.
      const auto tmp = _mm_hadd_ps( packed, packed );
      const auto ret = _mm_hadd_ps( tmp, tmp );
      return _mm_cvtss_f32( ret );
    }
    //! v = a * b + c (fused multiply & add)
    nmath_inline vec4f fma( const vec4f& a, const vec4f& b, const vec4f& c )
    {
      return _mm_fmadd_ps( a.packed, b.packed, c.packed );
    }
    //! v = a * b - c (fused multiply & subtract)
    nmath_inline vec4f fms( const vec4f& a, const vec4f& b, const vec4f& c )
    {
      return _mm_fmsub_ps( a.packed, b.packed, c.packed );
    }
  };

}

#endif