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
    nmath_inline vec4d()
    {
      packed = _mm256_setzero_pd();
    }
    nmath_inline vec4d( const double value )
    {
      packed = _mm256_broadcast_sd( &value );
    }
    nmath_inline vec4d( const double* __restrict values )
    {
      packed = _mm256_load_pd( values );
    }
    nmath_inline vec4d( const __m256d& rhs ): packed( rhs )
    {
    }
    nmath_inline vec4d( double _x, double _y, double _z, double _w )
    {
      packed = _mm256_setr_pd( _x, _y, _z, _w );
    }
    //! Load four 32-byte boundary aligned sample values into vector
    nmath_inline void load( const double* __restrict values )
    {
      packed = _mm256_load_pd( values );
    }
    //! Load four unaligned sample values into vector
    nmath_inline void loadUnaligned( const double* __restrict values )
    {
      packed = _mm256_loadu_pd( values );
    }
    //! Store four sample values from vector to 32-byte boundary aligned memory
    //! using a non-temporal usage hint (data not reused, don't cache in SSB)
    //! treating nontemporal data as temporal would be cache pollution = bad
    nmath_inline void storeNontemporal( double* __restrict values )
    {
      _mm256_stream_pd( values, packed );
    }
    //! Store four sample values from vector to 32-byte boundary aligned memory
    //! using a temporal usage hint (data is reused, maybe cache in SSB)
    //! subsequent stores of values cached in SSB can bypass L1, L2 & memctrl = nice
    nmath_inline void storeTemporal( double* __restrict values )
    {
      _mm256_store_pd( values, packed );
    }
    //! Store four sample values from vector to unaligned memory
    nmath_inline void storeUnaligned( double* __restrict values )
    {
      _mm256_storeu_pd( values, packed );
    }
    //! Set all vector members to single value
    nmath_inline void set( const double* value )
    {
      packed = _mm256_broadcast_sd( value );
    }
    //! Set vector members individually
    nmath_inline void set( double _x, double _y, double _z, double _w )
    {
      packed = _mm256_setr_pd( _x, _y, _z, _w );
    }
    //! v = a + b
    nmath_inline vec4d operator + ( const vec4d& rhs ) const
    {
      return _mm256_add_pd( packed, rhs.packed );
    }
    //! v = a - b
    nmath_inline vec4d operator - ( const vec4d& rhs ) const
    {
      return _mm256_sub_pd( packed, rhs.packed );
    }
    //! v = a & b (bitwise and)
    nmath_inline vec4d operator & ( const vec4d& rhs ) const
    {
      return _mm256_and_pd( packed, rhs.packed );
    }
    //! v = a | b (bitwise or)
    nmath_inline vec4d operator | ( const vec4d& rhs ) const
    {
      return _mm256_or_pd( packed, rhs.packed );
    }
    //! v = a ^ b (bitwise xor)
    nmath_inline vec4d operator ^ ( const vec4d& rhs ) const
    {
      return _mm256_xor_pd( packed, rhs.packed );
    }
    //! v = a * b
    nmath_inline vec4d operator * ( const double scalar ) const
    {
      const auto im = _mm256_set1_pd( scalar );
      return _mm256_mul_pd( packed, im );
    }
    //! v = a * b
    nmath_inline vec4d operator * ( const vec4d& rhs ) const
    {
      return _mm256_mul_pd( packed, rhs.packed );
    }
    //! v = a / b
    nmath_inline vec4d operator / ( const double scalar ) const
    {
      const auto im = _mm256_set1_pd( scalar );
      return _mm256_div_pd( packed, im );
    }
    //! v = a / b
    nmath_inline vec4d operator / ( const vec4d& rhs ) const
    {
      return _mm256_div_pd( packed, rhs.packed );
    }
    //! a == b
    nmath_inline bool operator == ( const vec4d& rhs ) const
    {
      const auto ret = _mm256_cmp_pd( packed, rhs.packed, _CMP_NEQ_OS );
      return ( _mm256_movemask_pd( ret ) == 0 );
    }
    //! a != b
    nmath_inline bool operator != ( const vec4d& rhs ) const
    {
      const auto ret = _mm256_cmp_pd( packed, rhs.packed, _CMP_NEQ_OS );
      return ( _mm256_movemask_pd( ret ) != 0 );
    }
    //! v = round(a)
    nmath_inline vec4d round() const
    {
      return _mm256_round_pd( packed, _MM_FROUND_NINT );
    }
    //! v = ceil(a)
    nmath_inline vec4d ceil() const
    {
      return _mm256_round_pd( packed, _MM_FROUND_CEIL );
    }
    //! v = floor(a)
    nmath_inline vec4d floor() const
    {
      return _mm256_round_pd( packed, _MM_FROUND_FLOOR );
    }
    //! v = trunc(a)
    nmath_inline vec4d trunc() const
    {
      return _mm256_round_pd( packed, _MM_FROUND_TRUNC );
    }
    //! v = sqrt(a)
    nmath_inline vec4d sqrt() const
    {
      return _mm256_sqrt_pd( packed );
    }
    //! f = x + y + z + w
    nmath_inline double sum() const
    {
      const auto hadd = _mm256_hadd_pd( packed, packed );
      const auto hi = _mm256_extractf128_pd( hadd, 1 );
      const auto lo = _mm256_castpd256_pd128( hadd );
      const auto ret = _mm_add_sd( lo, hi );
      return _mm_cvtsd_f64( ret );
    }
    //! v = a * b + c (fused multiply & add)
    nmath_inline vec4d fma( const vec4d& a, const vec4d& b, const vec4d& c )
    {
      return _mm256_fmadd_pd( a.packed, b.packed, c.packed );
    }
    //! v = a * b - c (fused multiply & subtract)
    nmath_inline vec4d fms( const vec4d& a, const vec4d& b, const vec4d& c )
    {
      return _mm256_fmsub_pd( a.packed, b.packed, c.packed );
    }
  };

}

#endif