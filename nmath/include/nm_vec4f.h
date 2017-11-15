#pragma once
#include <cstdint>
#include <intrin.h>

#define nmath_avx_align __declspec(align(32))

namespace nmath {

  using Real = double;

  nmath_avx_align class vec4f {
  public:
    union {
      struct {
        Real x, y, z, w;
      };
      __m256d packed;
    };
    inline vec4f()
    {
      packed = _mm256_setzero_pd();
    }
    inline vec4f( const Real value )
    {
      packed = _mm256_broadcast_sd( &value );
    }
    inline vec4f( const __m256d& rhs ): packed( rhs )
    {
    }
    inline vec4f( Real _x, Real _y, Real _z, Real _w )
    {
      packed = _mm256_setr_pd( _x, _y, _z, _w );
    }
    //! Load four 32-byte boundary aligned sample values into vector
    inline void load( const Real* values )
    {
      packed = _mm256_load_pd( values );
    }
    //! Load four unaligned sample values into vector
    inline void loadUnaligned( const Real* values )
    {
      packed = _mm256_loadu_pd( values );
    }
    //! Store four sample values from vector to 32-byte boundary aligned memory
    inline void store( Real* values )
    {
      _mm256_stream_pd( values, packed );
    }
    //! Store four sample values from vector to unaligned memory
    inline void storeUnaligned( Real* values )
    {
      _mm256_store_pd( values, packed );
    }
    //! Set all vector members to single value
    inline void set( const Real* value )
    {
      packed = _mm256_broadcast_sd( value );
    }
    //! Set vector members individually
    inline void set( Real _x, Real _y, Real _z, Real _w )
    {
      packed = _mm256_setr_pd( _x, _y, _z, _w );
    }
    //! v = a + b
    inline vec4f operator + ( const vec4f& rhs ) const
    {
      return _mm256_add_pd( packed, rhs.packed );
    }
    //! v = a - b
    inline vec4f operator - ( const vec4f& rhs ) const
    {
      return _mm256_sub_pd( packed, rhs.packed );
    }
    //! v = a & b (bitwise and)
    inline vec4f operator & ( const vec4f& rhs ) const
    {
      return _mm256_and_pd( packed, rhs.packed );
    }
    //! v = a | b (bitwise or)
    inline vec4f operator | ( const vec4f& rhs ) const
    {
      return _mm256_or_pd( packed, rhs.packed );
    }
    //! v = a ^ b (bitwise xor)
    inline vec4f operator ^ ( const vec4f& rhs ) const
    {
      return _mm256_xor_pd( packed, rhs.packed );
    }
    //! v = a * b
    inline vec4f operator * ( const Real scalar ) const
    {
      auto im = _mm256_set1_pd( scalar );
      return _mm256_mul_pd( packed, im );
    }
    //! v = a * b
    inline vec4f operator * ( const vec4f& rhs ) const
    {
      return _mm256_mul_pd( packed, rhs.packed );
    }
    //! v = a / b
    inline vec4f operator / ( const Real scalar ) const
    {
      auto im = _mm256_set1_pd( scalar );
      return _mm256_div_pd( packed, im );
    }
    //! v = a / b
    inline vec4f operator / ( const vec4f& rhs ) const
    {
      return _mm256_div_pd( packed, rhs.packed );
    }
    //! a == b
    inline bool operator == ( const vec4f& rhs ) const
    {
      auto ret = _mm256_cmp_pd( packed, rhs.packed, _CMP_NEQ_OS );
      return ( _mm256_movemask_pd( ret ) == 0 );
    }
    //! a != b
    inline bool operator != ( const vec4f& rhs ) const
    {
      auto ret = _mm256_cmp_pd( packed, rhs.packed, _CMP_NEQ_OS );
      return ( _mm256_movemask_pd( ret ) != 0 );
    }
    //! v = round(a)
    inline vec4f round() const
    {
      return _mm256_round_pd( packed, _MM_FROUND_NINT );
    }
    //! v = ceil(a)
    inline vec4f ceil() const
    {
      return _mm256_round_pd( packed, _MM_FROUND_CEIL );
    }
    //! v = floor(a)
    inline vec4f floor() const
    {
      return _mm256_round_pd( packed, _MM_FROUND_FLOOR );
    }
    //! v = trunc(a)
    inline vec4f trunc() const
    {
      return _mm256_round_pd( packed, _MM_FROUND_TRUNC );
    }
    //! v = sqrt(a)
    inline vec4f sqrt() const
    {
      return _mm256_sqrt_pd( packed );
    }
    //! f = x + y + z + w
    inline Real sum() const
    {
      auto hadd = _mm256_hadd_pd( packed, packed );
      auto hi = _mm256_extractf128_pd( hadd, 1 );
      auto lo = _mm256_castpd256_pd128( hadd );
      auto ret = _mm_add_sd( lo, hi );
      return _mm_cvtsd_f64( ret );
    }
    //! v = a * b + c (fused multiply & add)
    static inline vec4f fma( const vec4f& a, const vec4f& b, const vec4f& c )
    {
      return _mm256_fmadd_pd( a.packed, b.packed, c.packed );
    }
    //! v = a * b - c (fused multiply & subtract)
    static inline vec4f fms( const vec4f& a, const vec4f& b, const vec4f& c )
    {
      return _mm256_fmsub_pd( a.packed, b.packed, c.packed );
    }
  };

}