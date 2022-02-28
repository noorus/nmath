#ifndef NM_VEC4D_H
#define NM_VEC4D_H

#include "nm_common.h"

namespace nmath {

  nmath_32b_align class vec4d {
  public:
    union {
      struct {
        double x, y, z, w;
      };
      __m256d packed;
    };
    inline vec4d()
    {
      packed = _mm256_setzero_pd();
    }
    inline vec4d( const double value )
    {
      packed = _mm256_broadcast_sd( &value );
    }
    inline vec4d( const __m256d& rhs ): packed( rhs )
    {
    }
    inline vec4d( double _x, double _y, double _z, double _w )
    {
      packed = _mm256_setr_pd( _x, _y, _z, _w );
    }
    //! Load four 32-byte boundary aligned sample values into vector
    inline void load( const double* values )
    {
      packed = _mm256_load_pd( values );
    }
    //! Load four unaligned sample values into vector
    inline void loadUnaligned( const double* values )
    {
      packed = _mm256_loadu_pd( values );
    }
    //! Store four sample values from vector to 32-byte boundary aligned memory
    inline void store( double* values )
    {
      _mm256_stream_pd( values, packed );
    }
    //! Store four sample values from vector to unaligned memory
    inline void storeUnaligned( double* values )
    {
      _mm256_store_pd( values, packed );
    }
    //! Set all vector members to single value
    inline void set( const double* value )
    {
      packed = _mm256_broadcast_sd( value );
    }
    //! Set vector members individually
    inline void set( double _x, double _y, double _z, double _w )
    {
      packed = _mm256_setr_pd( _x, _y, _z, _w );
    }
    //! v = a + b
    inline vec4d operator + ( const vec4d& rhs ) const
    {
      return _mm256_add_pd( packed, rhs.packed );
    }
    //! v = a - b
    inline vec4d operator - ( const vec4d& rhs ) const
    {
      return _mm256_sub_pd( packed, rhs.packed );
    }
    //! v = a & b (bitwise and)
    inline vec4d operator & ( const vec4d& rhs ) const
    {
      return _mm256_and_pd( packed, rhs.packed );
    }
    //! v = a | b (bitwise or)
    inline vec4d operator | ( const vec4d& rhs ) const
    {
      return _mm256_or_pd( packed, rhs.packed );
    }
    //! v = a ^ b (bitwise xor)
    inline vec4d operator ^ ( const vec4d& rhs ) const
    {
      return _mm256_xor_pd( packed, rhs.packed );
    }
    //! v = a * b
    inline vec4d operator * ( const double scalar ) const
    {
      auto im = _mm256_set1_pd( scalar );
      return _mm256_mul_pd( packed, im );
    }
    //! v = a * b
    inline vec4d operator * ( const vec4d& rhs ) const
    {
      return _mm256_mul_pd( packed, rhs.packed );
    }
    //! v = a / b
    inline vec4d operator / ( const double scalar ) const
    {
      auto im = _mm256_set1_pd( scalar );
      return _mm256_div_pd( packed, im );
    }
    //! v = a / b
    inline vec4d operator / ( const vec4d& rhs ) const
    {
      return _mm256_div_pd( packed, rhs.packed );
    }
    //! a == b
    inline bool operator == ( const vec4d& rhs ) const
    {
      auto ret = _mm256_cmp_pd( packed, rhs.packed, _CMP_NEQ_OS );
      return ( _mm256_movemask_pd( ret ) == 0 );
    }
    //! a != b
    inline bool operator != ( const vec4d& rhs ) const
    {
      auto ret = _mm256_cmp_pd( packed, rhs.packed, _CMP_NEQ_OS );
      return ( _mm256_movemask_pd( ret ) != 0 );
    }
    //! v = round(a)
    inline vec4d round() const
    {
      return _mm256_round_pd( packed, _MM_FROUND_NINT );
    }
    //! v = ceil(a)
    inline vec4d ceil() const
    {
      return _mm256_round_pd( packed, _MM_FROUND_CEIL );
    }
    //! v = floor(a)
    inline vec4d floor() const
    {
      return _mm256_round_pd( packed, _MM_FROUND_FLOOR );
    }
    //! v = trunc(a)
    inline vec4d trunc() const
    {
      return _mm256_round_pd( packed, _MM_FROUND_TRUNC );
    }
    //! v = sqrt(a)
    inline vec4d sqrt() const
    {
      return _mm256_sqrt_pd( packed );
    }
    //! f = x + y + z + w
    inline double sum() const
    {
      auto hadd = _mm256_hadd_pd( packed, packed );
      auto hi = _mm256_extractf128_pd( hadd, 1 );
      auto lo = _mm256_castpd256_pd128( hadd );
      auto ret = _mm_add_sd( lo, hi );
      return _mm_cvtsd_f64( ret );
    }
    //! v = a * b + c (fused multiply & add)
    inline vec4d fma( const vec4d& a, const vec4d& b, const vec4d& c )
    {
      return _mm256_fmadd_pd( a.packed, b.packed, c.packed );
    }
    //! v = a * b - c (fused multiply & subtract)
    inline vec4d fms( const vec4d& a, const vec4d& b, const vec4d& c )
    {
      return _mm256_fmsub_pd( a.packed, b.packed, c.packed );
    }
  };

}

#endif