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
    inline vec4f()
    {
      packed = _mm_setzero_ps();
    }
    inline vec4f( const float value )
    {
      packed = _mm_broadcast_ss( &value );
    }
    inline vec4f( const __m128& rhs ): packed( rhs )
    {
    }
    inline vec4f( float _x, float _y, float _z, float _w )
    {
      packed = _mm_setr_ps( _x, _y, _z, _w );
    }
    //! Load four 16-byte boundary aligned sample values into vector
    inline void load( const float* values )
    {
      packed = _mm_load_ps( values );
    }
    //! Load four unaligned sample values into vector
    inline void loadUnaligned( const float* values )
    {
      packed = _mm_loadu_ps( values );
    }
    //! Store four sample values from vector to 16-byte boundary aligned memory
    inline void store( float* values )
    {
      _mm_stream_ps( values, packed );
    }
    //! Store four sample values from vector to unaligned memory
    inline void storeUnaligned( float* values )
    {
      _mm_store_ps( values, packed );
    }
    //! Set all vector members to single value
    inline void set( const float* value )
    {
      packed = _mm_broadcast_ss( value );
    }
    //! Set vector members individually
    inline void set( float _x, float _y, float _z, float _w )
    {
      packed = _mm_setr_ps( _x, _y, _z, _w );
    }
    //! v = a + b
    inline vec4f operator + ( const vec4f& rhs ) const
    {
      return _mm_add_ps( packed, rhs.packed );
    }
    //! v = a - b
    inline vec4f operator - ( const vec4f& rhs ) const
    {
      return _mm_sub_ps( packed, rhs.packed );
    }
    //! v = a & b (bitwise and)
    inline vec4f operator & ( const vec4f& rhs ) const
    {
      return _mm_and_ps( packed, rhs.packed );
    }
    //! v = a | b (bitwise or)
    inline vec4f operator | ( const vec4f& rhs ) const
    {
      return _mm_or_ps( packed, rhs.packed );
    }
    //! v = a ^ b (bitwise xor)
    inline vec4f operator ^ ( const vec4f& rhs ) const
    {
      return _mm_xor_ps( packed, rhs.packed );
    }
    //! v = a * b
    inline vec4f operator * ( const float scalar ) const
    {
      auto im = _mm_set1_ps( scalar );
      return _mm_mul_ps( packed, im );
    }
    //! v = a * b
    inline vec4f operator * ( const vec4f& rhs ) const
    {
      return _mm_mul_ps( packed, rhs.packed );
    }
    //! v = a / b
    inline vec4f operator / ( const float scalar ) const
    {
      auto im = _mm_set1_ps( scalar );
      return _mm_div_ps( packed, im );
    }
    //! v = a / b
    inline vec4f operator / ( const vec4f& rhs ) const
    {
      return _mm_div_ps( packed, rhs.packed );
    }
    //! a == b
    inline bool operator == ( const vec4f& rhs ) const
    {
      auto ret = _mm_cmp_ps( packed, rhs.packed, _CMP_NEQ_OS );
      return ( _mm_movemask_ps( ret ) == 0 );
    }
    //! a != b
    inline bool operator != ( const vec4f& rhs ) const
    {
      auto ret = _mm_cmp_ps( packed, rhs.packed, _CMP_NEQ_OS );
      return ( _mm_movemask_ps( ret ) != 0 );
    }
    //! v = round(a)
    inline vec4f round() const
    {
      return _mm_round_ps( packed, _MM_FROUND_NINT );
    }
    //! v = ceil(a)
    inline vec4f ceil() const
    {
      return _mm_round_ps( packed, _MM_FROUND_CEIL );
    }
    //! v = floor(a)
    inline vec4f floor() const
    {
      return _mm_round_ps( packed, _MM_FROUND_FLOOR );
    }
    //! v = trunc(a)
    inline vec4f trunc() const
    {
      return _mm_round_ps( packed, _MM_FROUND_TRUNC );
    }
    //! v = sqrt(a)
    inline vec4f sqrt() const
    {
      return _mm_sqrt_ps( packed );
    }
    //! f = x + y + z + w
    inline float sum() const
    {
      auto tmp = _mm_hadd_ps( packed, packed );
      auto ret = _mm_hadd_ps( tmp, tmp );
      return _mm_cvtss_f32( ret );
    }
    //! v = a * b + c (fused multiply & add)
    inline vec4f fma( const vec4f& a, const vec4f& b, const vec4f& c )
    {
      return _mm_fmadd_ps( a.packed, b.packed, c.packed );
    }
    //! v = a * b - c (fused multiply & subtract)
    inline vec4f fms( const vec4f& a, const vec4f& b, const vec4f& c )
    {
      return _mm_fmsub_ps( a.packed, b.packed, c.packed );
    }
  };

}

#endif