#pragma once
namespace MiniMapCommon {
  enum {
    ST_NONE  = 0,
    ST_PARCH = 1,
    ST_IRON  = 2,
    ST_GOLD  = 3,
    ST_COUNT = 4,
    ST_ATLAS = 512,
    ST_QUADS = 448
  };
  const int ST_DIM = 96;
  const float ST_TH   = 30.0f;
  const float ST_TPAD = 5.0f;
  const float ST_TGAP = 9.0f;
  struct TStC { float r, g, b, a; };
  inline TStC StRgb( float r, float g, float b, float a = 1.0f ) {
    TStC c;
    c.a = a;
    c.r = r * a / 255.0f;
    c.g = g * a / 255.0f;
    c.b = b * a / 255.0f;
    return c;
  }
  inline TStC StNone() {
    TStC c;
    c.r = c.g = c.b = c.a = 0.0f;
    return c;
  }
  inline float StC01( float x ) { return ( x < 0.0f ) ? 0.0f : ( ( x > 1.0f ) ? 1.0f : x ); }
  inline float StSm( float e0, float e1, float x ) {
    const float t = StC01( ( x - e0 ) / ( e1 - e0 ) );
    return t * t * ( 3.0f - 2.0f * t );
  }
  inline TStC StOver( const TStC& s, const TStC& d ) {
    const float k = 1.0f - s.a;
    TStC c;
    c.r = s.r + d.r * k;
    c.g = s.g + d.g * k;
    c.b = s.b + d.b * k;
    c.a = s.a + d.a * k;
    return c;
  }
  inline TStC StMul( const TStC& c, float f ) {
    TStC o = c;
    o.r *= f; o.g *= f; o.b *= f;
    if( o.r > o.a ) o.r = o.a;
    if( o.g > o.a ) o.g = o.a;
    if( o.b > o.a ) o.b = o.a;
    if( o.r < 0.0f ) o.r = 0.0f;
    if( o.g < 0.0f ) o.g = 0.0f;
    if( o.b < 0.0f ) o.b = 0.0f;
    return o;
  }
  inline TStC StMix( const TStC& a, const TStC& b, float t ) {
    TStC c;
    c.r = a.r + ( b.r - a.r ) * t;
    c.g = a.g + ( b.g - a.g ) * t;
    c.b = a.b + ( b.b - a.b ) * t;
    c.a = a.a + ( b.a - a.a ) * t;
    return c;
  }
  inline TStC StAdd( const TStC& c, float v ) {
    TStC o = c;
    o.r += v * o.a; o.g += v * o.a; o.b += v * o.a;
    if( o.r > o.a ) o.r = o.a;
    if( o.g > o.a ) o.g = o.a;
    if( o.b > o.a ) o.b = o.a;
    return o;
  }
  inline float StHash( int x, int y, int seed ) {
    unsigned int h = (unsigned int)x * 374761393u + (unsigned int)y * 668265263u + (unsigned int)seed * 2246822519u;
    h = ( h ^ ( h >> 13 ) ) * 1274126177u;
    h ^= h >> 16;
    return (float)( h & 0xFFFFu ) * ( 1.0f / 65535.0f );
  }
  inline float StNoise( float x, float y, int seed, int px ) {
    const float fx0 = floorf( x ), fy0 = floorf( y );
    int   x0 = (int)fx0, x1 = x0 + 1;
    const int y0 = (int)fy0;
    float fx = x - fx0, fy = y - fy0;
    fx = fx * fx * ( 3.0f - 2.0f * fx );
    fy = fy * fy * ( 3.0f - 2.0f * fy );
    if( px > 0 ) {
      x0 = ( ( x0 % px ) + px ) % px;
      x1 = ( ( x1 % px ) + px ) % px;
    }
    const float a = StHash( x0, y0, seed ), b = StHash( x1, y0, seed );
    const float c = StHash( x0, y0 + 1, seed ), d = StHash( x1, y0 + 1, seed );
    const float top = a + ( b - a ) * fx, bot = c + ( d - c ) * fx;
    return top + ( bot - top ) * fy;
  }
  inline float StNz( float a, float t, float cx, float cy, float Lt, int seed ) {
    if( Lt > 0.0f ) {
      int nc = (int)( Lt / cx + 0.5f );
      if( nc < 1 ) nc = 1;
      return StNoise( a * (float)nc / Lt, t / cy, seed, nc );
    }
    return StNoise( a / cx, t / cy, seed, 0 );
  }
  struct TStSpec {
    float T;
    float I;
    float S;
    float P;
    int   per;
    float orn;
    float tMin;
    float capW;
  };
  inline TStSpec StSpec( int st, bool full ) {
    TStSpec s;
    s.I    = full ? 3.0f : 2.0f;
    s.S    = full ? 5.0f : 3.0f;
    s.tMin = full ? 8.0f : 4.0f;
    s.orn  = 0.0f;
    if( st == ST_PARCH ) {
      s.T = full ? 16.0f : 7.0f;  s.P = full ? 40.0f : 20.0f; s.per = full ? 3 : 4;
      s.orn = full ? 7.0f : 3.4f; s.capW = 9.0f;
    }
    else if( st == ST_IRON ) {
      s.T = full ? 16.0f : 7.0f;  s.P = full ? 56.0f : 30.0f; s.per = full ? 2 : 3;
      s.capW = 14.0f;
    }
    else {
      s.T = full ? 18.0f : 8.0f;  s.P = full ? 12.0f : 8.0f;  s.per = full ? 8 : 10;
      s.orn = full ? 11.5f : 5.2f; s.capW = 18.0f;
    }
    return s;
  }
  inline float StScaleFor( int st, bool full, float ui ) {
    const TStSpec s = StSpec( st, full );
    float k = ui;
    if( s.T * k < s.tMin ) k = s.tMin / s.T;
    if( k > 3.0f ) k = 3.0f;
    return k;
  }
  inline TStC StShadowOut( float t, const TStSpec& s ) {
    const float u = ( t - s.T ) / s.S;
    if( u >= 1.0f ) return StNone();
    return StRgb( 0, 0, 0, 0.55f * ( 1.0f - u ) * ( 1.0f - u ) );
  }
  inline TStC StShadowIn( float t, const TStSpec& s ) {
    const float u = ( t + s.I ) / s.I;
    if( u <= 0.0f ) return StNone();
    return StRgb( 0, 0, 0, 0.40f * u * u );
  }
  inline TStC StParchCol( float a, float t, float age, float Lt ) {
    const float n1 = StNz( a, t, 6.0f, 6.0f, Lt, 11 );
    const float n2 = StNz( a, t, 7.0f, 0.9f, Lt, 12 );
    const float n3 = StNz( a, t, 19.0f, 13.0f, Lt, 13 );
    const float f  = 0.91f + 0.09f * n1 + 0.05f * ( n2 - 0.5f )
                   - 0.07f * StSm( 0.60f, 0.92f, n3 );
    return StMul( StMix( StRgb( 238, 222, 182 ), StRgb( 182, 146, 94 ), StC01( age ) ), f );
  }
  inline TStC StIronCol( float a, float t, float x, float Lt ) {
    const float n1 = StNz( a, t, 5.0f, 5.0f, Lt, 21 );
    const float n2 = StNz( a, t, 9.0f, 1.0f, Lt, 22 );
    const float f  = ( 0.76f + 0.34f * sinf( 3.1415927f * StC01( x ) ) )
                   * ( 0.88f + 0.20f * n1 ) + 0.06f * ( n2 - 0.5f );
    return StMul( StRgb( 72, 76, 86 ), f );
  }
  inline TStC StGold( float b ) {
    static const float k[6][4] = {
      { 0.00f,  40.0f,  24.0f,   6.0f }, { 0.30f, 122.0f,  78.0f,  22.0f },
      { 0.58f, 198.0f, 150.0f,  60.0f }, { 0.82f, 238.0f, 198.0f, 110.0f },
      { 1.00f, 250.0f, 226.0f, 150.0f }, { 1.25f, 255.0f, 248.0f, 214.0f } };
    if( b <= 0.0f )  return StRgb( k[0][1], k[0][2], k[0][3] );
    if( b >= 1.25f ) return StRgb( k[5][1], k[5][2], k[5][3] );
    int i = 0;
    while( i < 4 && b > k[i + 1][0] ) i++;
    const float t = ( b - k[i][0] ) / ( k[i + 1][0] - k[i][0] );
    return StRgb( k[i][1] + ( k[i + 1][1] - k[i][1] ) * t,
                  k[i][2] + ( k[i + 1][2] - k[i][2] ) * t,
                  k[i][3] + ( k[i + 1][3] - k[i][3] ) * t );
  }
  inline TStC StDome( float dx, float dy, float R, const TStC& base,
                      float lx, float ly, float lz, float shadow ) {
    const float d  = sqrtf( dx * dx + dy * dy );
    const float sw = 0.45f * R + 0.6f;
    if( d >= R ) {
      if( shadow <= 0.0f || d >= R + sw ) return StNone();
      const float u = ( d - R ) / sw;
      return StRgb( 0, 0, 0, shadow * ( 1.0f - u ) * ( 1.0f - u ) );
    }
    const float nx = dx / R, ny = dy / R;
    const float nz = sqrtf( StC01( 1.0f - nx * nx - ny * ny ) );
    float dif = nx * lx + ny * ly + nz * lz;
    if( dif < 0.0f ) dif = 0.0f;
    float hx = lx, hy = ly, hz = lz + 1.0f;
    const float hl = sqrtf( hx * hx + hy * hy + hz * hz );
    hx /= hl; hy /= hl; hz /= hl;
    float sp = nx * hx + ny * hy + nz * hz;
    sp = ( sp > 0.0f ) ? powf( sp, 26.0f ) : 0.0f;
    TStC c = StAdd( StMul( base, 0.32f + 0.85f * dif ), 0.85f * sp );
    float rw = 0.30f * R;
    if( rw > 0.9f ) rw = 0.9f;
    return StMix( c, StRgb( 18, 18, 22 ), 0.60f * StSm( R - rw, R, d ) );
  }
  inline TStC StProfile( int st, bool full, const TStSpec& s, float a, float t,
                         float Lt, bool corner ) {
    if( st == ST_PARCH ) {
      static const float kF[7] = { 1.2f, 2.4f, 3.0f, 7.0f, 7.6f, 14.6f, 16.0f };
      static const float kM[7] = { 0.9f, 1.5f, 1.9f, 4.3f, 4.7f, 6.2f, 7.0f };
      const float* b = full ? kF : kM;
      if( t < b[0] ) return StRgb( 58, 40, 26 );
      if( t < b[1] ) return StParchCol( a, t, 0.0f, Lt );
      if( t < b[2] ) return StRgb( 86, 62, 40 );
      if( t < b[3] ) {
        bool dark = corner;
        if( !corner ) {
          float ph = fmodf( a, s.P ) / s.P;
          if( ph < 0.0f ) ph += 1.0f;
          dark = ( ph >= 0.25f && ph < 0.75f );
        }
        if( dark ) return StMul( StRgb( 104, 74, 46 ), 0.92f + 0.12f * StNz( a, t, 5.0f, 5.0f, Lt, 14 ) );
        return StMul( StParchCol( a, t, 0.0f, Lt ), 1.03f );
      }
      if( t < b[4] ) return StRgb( 86, 62, 40 );
      if( t < b[5] ) return StParchCol( a, t, 0.65f * StSm( b[4], b[6], t ), Lt );
      return StRgb( 64, 45, 29 );
    }
    if( st == ST_IRON ) {
      static const float kF[5] = { 1.0f, 2.0f, 13.5f, 14.5f, 16.0f };
      static const float kM[5] = { 0.8f, 1.4f, 5.6f, 6.2f, 7.0f };
      const float* b = full ? kF : kM;
      TStC c;
      if( t < b[0] )      c = StRgb( 14, 14, 16 );
      else if( t < b[1] ) c = StRgb( 152, 158, 170 );
      else if( t < b[2] ) c = StIronCol( a, t, ( t - b[1] ) / ( b[2] - b[1] ), Lt );
      else if( t < b[3] ) c = StRgb( 118, 124, 136 );
      else                c = StRgb( 22, 22, 26 );
      if( !corner ) {
        float da = fmodf( a, s.P );
        if( da < 0.0f ) da += s.P;
        da -= s.P * 0.5f;
        const float rc = ( b[1] + b[2] ) * 0.5f;
        c = StOver( StDome( da, t - rc, full ? 3.0f : 1.5f, StRgb( 128, 134, 146 ),
                            0.0f, 0.5f, 0.866f, 0.5f ), c );
      }
      return c;
    }
    static const float kF[7] = { 0.9f, 2.6f, 3.3f, 13.7f, 14.4f, 16.8f, 18.0f };
    static const float kM[7] = { 0.7f, 1.9f, 2.4f, 6.0f, 6.4f, 7.4f, 8.0f };
    const float* b = full ? kF : kM;
    const float grain = 0.97f + 0.06f * StNz( a, t, 3.0f, 3.0f, Lt, 31 );
    if( t < b[0] ) return StRgb( 36, 22, 6 );
    if( t < b[1] ) {
      const float x = ( t - b[0] ) / ( b[1] - b[0] );
      return StMul( StGold( 0.35f + 0.80f * sinf( 3.1415927f * x ) ), grain );
    }
    if( t < b[2] ) return StGold( 0.12f );
    if( t < b[3] ) {
      const float x  = ( t - b[2] ) / ( b[3] - b[2] );
      const float b0 = 0.22f + 0.82f * powf( sinf( 3.1415927f * x ), 0.75f );
      const float tw = corner ? 0.55f
                     : 0.5f + 0.5f * sinf( 6.2831853f * ( a / s.P + 1.5f * x ) );
      return StMul( StGold( b0 * ( 0.78f + 0.34f * tw ) ), grain );
    }
    if( t < b[4] ) return StGold( 0.12f );
    if( t < b[5] ) {
      const float x = ( t - b[4] ) / ( b[5] - b[4] );
      return StMul( StGold( 0.35f + 0.80f * sinf( 3.1415927f * x ) ), grain );
    }
    return StRgb( 44, 28, 8 );
  }
  inline TStC StEdge( int st, bool full, const TStSpec& s, float a, float t, float Lt ) {
    if( t < 0.0f )  return StShadowIn( t, s );
    if( t >= s.T )  return StShadowOut( t, s );
    return StProfile( st, full, s, a, t, Lt, false );
  }
  inline TStC StCorner( int st, bool full, const TStSpec& s, float ap, float bp ) {
    if( ap < 0.0f || bp < 0.0f ) {
      const float t = ( ap < 0.0f ) ? bp : ap;
      if( ap < 0.0f && bp < 0.0f ) return StShadowIn( ( ap > bp ) ? ap : bp, s );
      return StEdge( st, full, s, 0.25f, t, 0.0f );
    }
    const float m = ( ap > bp ) ? ap : bp;
    if( m >= s.T ) return StShadowOut( m, s );
    TStC c = StProfile( st, full, s, 0.25f, m, 0.0f, true );
    if( st == ST_IRON ) {
      const float bw = full ? 1.3f : 0.8f;
      const float eo = s.T - m;
      const float ei = ( ap < bp ) ? ap : bp;
      if( eo < bw * 0.55f )      c = StRgb( 20, 20, 24 );
      else if( eo < bw * 1.6f )  c = StRgb( 150, 156, 168 );
      else if( ei < bw * 0.55f ) c = StRgb( 16, 16, 18 );
      else if( ei < bw * 1.3f )  c = StRgb( 44, 46, 52 );
      else                       c = StIronCol( ap, bp, 0.55f, 0.0f );
      c = StOver( StDome( ap - s.T * 0.5f, bp - s.T * 0.5f, full ? 4.0f : 2.0f,
                          StRgb( 132, 138, 150 ), 0.35f, 0.35f, 0.87f, 0.55f ), c );
    }
    return c;
  }
  inline TStC StOrn( int st, const TStSpec& s, float dx, float dy ) {
    const float R = s.orn;
    if( R <= 0.0f ) return StNone();
    const float sx = dx - 0.6f, sy = dy - 1.0f;
    const float ds = sqrtf( sx * sx + sy * sy );
    TStC c = StRgb( 0, 0, 0, 0.45f * ( 1.0f - StSm( R - 0.6f, R + 1.8f, ds ) ) );
    const float d = sqrtf( dx * dx + dy * dy );
    if( d >= R ) return c;
    if( st == ST_PARCH ) {
      const float ring = ( R > 5.0f ) ? 1.1f : 0.7f;
      TStC f;
      if( d > R - ring )
        f = StRgb( 58, 40, 26 );
      else {
        f = StMul( StRgb( 240, 226, 190 ), 0.94f + 0.08f * StNoise( dx * 0.5f, dy * 0.5f, 41, 0 ) );
        const float th = atan2f( dy, dx );
        const float rs = ( R - ring ) * 0.92f * ( 0.22f + 0.78f * powf( fabsf( cosf( 2.0f * th ) ), 5.0f ) );
        if( d < rs ) {
          const bool half = ( fmodf( th + 6.2831853f, 1.5707963f ) < 0.785398f );
          f = half ? StRgb( 58, 40, 26 ) : StRgb( 132, 96, 60 );
        }
        if( d < ( R - ring ) * 0.16f ) f = StRgb( 236, 220, 180 );
      }
      return StOver( f, c );
    }
    TStC f;
    if( d > R * 0.60f ) {
      const float x = ( d - R * 0.60f ) / ( R * 0.40f );
      const float dir = -( dx + dy ) / ( d * 1.4142f + 0.001f );
      f = StGold( 0.28f + 0.80f * sinf( 3.1415927f * x ) + 0.22f * dir );
    }
    else if( d > R * 0.52f )
      f = StRgb( 46, 24, 6 );
    else {
      f = StDome( dx, dy, R * 0.52f, StRgb( 168, 18, 30 ), -0.45f, -0.55f, 0.70f, 0.0f );
      const float gx = dx - R * 0.14f, gy = dy - R * 0.16f;
      const float g  = 1.0f - StSm( 0.0f, R * 0.34f, sqrtf( gx * gx + gy * gy ) );
      f = StMix( f, StRgb( 236, 70, 80 ), 0.35f * g );
    }
    return StOver( f, c );
  }
  inline float StTitleSd( int st, float X, float Y ) {
    const float H = ST_TH;
    const float sy = ( -Y > Y - H ) ? -Y : Y - H;
    if( st == ST_PARCH ) {
      float sp = 5.0f - X;
      if( sy > sp ) sp = sy;
      const float rr = 2.4f;
      const float qx = fabsf( X - 3.5f ) - ( 3.5f - rr );
      const float qy = fabsf( Y - H * 0.5f ) - ( H * 0.5f + 2.0f - rr );
      const float ox = ( qx > 0.0f ) ? qx : 0.0f, oy = ( qy > 0.0f ) ? qy : 0.0f;
      const float in = ( ( qx > qy ) ? qx : qy );
      const float sr = sqrtf( ox * ox + oy * oy ) + ( ( in < 0.0f ) ? in : 0.0f ) - rr;
      return ( sp < sr ) ? sp : sr;
    }
    if( st == ST_IRON ) {
      float sd = -X;
      if( sy > sd ) sd = sy;
      const float ym = ( Y < H - Y ) ? Y : H - Y;
      const float sc = ( 5.0f - ( X + ym ) ) * 0.7071f;
      return ( sc > sd ) ? sc : sd;
    }
    const float st2 = ( 11.0f * fabsf( Y - H * 0.5f ) / ( H * 0.5f ) - X ) * 0.806f;
    return ( st2 > sy ) ? st2 : sy;
  }
  inline TStC StTitleFill( int st, float X, float Y, float sd, bool cap ) {
    const float H = ST_TH;
    const float e = -sd;
    if( st == ST_PARCH ) {
      if( X < 7.0f && cap ) {
        if( e < 0.9f ) return StRgb( 70, 48, 30 );
        if( X > 6.3f && Y > 0.0f && Y < H ) return StRgb( 70, 48, 30 );
        const float f = 0.56f + 0.46f * sinf( 3.1415927f * StC01( X / 7.0f ) );
        TStC c = StMul( StRgb( 226, 206, 160 ), f * ( 0.95f + 0.07f * StNoise( 2.0f, Y / 4.0f, 43, 0 ) ) );
        if( Y < 1.2f || Y > H - 1.2f ) c = StMul( c, 0.80f );
        return c;
      }
      if( e < 0.9f ) return StRgb( 70, 48, 30 );
      if( ( Y > 2.6f && Y < 3.3f ) || ( Y > H - 3.3f && Y < H - 2.6f ) ) return StRgb( 96, 70, 46 );
      return StMul( StParchCol( 0.0f, Y * 1.7f, 0.0f, 0.0f ),
                    0.95f + 0.07f * sinf( 3.1415927f * Y / H ) );
    }
    if( st == ST_IRON ) {
      TStC c;
      if( e < 0.8f )             c = StRgb( 16, 16, 18 );
      else if( Y < 2.4f )        c = StRgb( 158, 164, 176 );
      else if( Y > H - 2.4f )    c = StRgb( 34, 35, 40 );
      else if( cap && X + ( ( Y < H - Y ) ? Y : H - Y ) < 7.4f )
        c = StRgb( 140, 146, 158 );
      else if( cap && X < 2.4f ) c = StRgb( 140, 146, 158 );
      else
        c = StMul( StIronCol( 0.0f, Y, 0.30f + 0.45f * ( 1.0f - Y / H ), 0.0f ), 1.0f );
      if( cap )
        c = StOver( StDome( X - 8.0f, Y - H * 0.5f, 3.4f, StRgb( 134, 140, 152 ),
                            -0.45f, -0.55f, 0.70f, 0.55f ), c );
      return c;
    }
    TStC c;
    if( e < 0.8f )      c = StRgb( 44, 26, 6 );
    else if( e < 3.6f ) {
      const float x = ( e - 0.8f ) / 2.8f;
      c = StGold( 0.30f + 0.82f * sinf( 3.1415927f * x ) + ( ( Y < H * 0.5f ) ? 0.14f : -0.12f ) );
    }
    else if( e < 4.3f ) c = StRgb( 70, 36, 10 );
    else
      c = StMul( StRgb( 96, 24, 20 ), ( 0.62f + 0.42f * sinf( 3.1415927f * Y / H ) )
                                       * ( 0.95f + 0.08f * StNoise( 1.0f, Y / 3.0f, 51, 0 ) ) );
    if( cap ) {
      const float gx = X - 14.5f, gy = Y - H * 0.5f;
      const float gd = sqrtf( gx * gx + gy * gy );
      if( gd < 4.4f )
        c = ( gd > 3.3f ) ? StGold( 0.45f + 0.5f * sinf( 3.1415927f * ( gd - 3.3f ) / 1.1f ) - 0.2f * ( gx + gy ) / ( gd + 0.01f ) )
                          : StDome( gx, gy, 3.3f, StRgb( 168, 18, 30 ), -0.45f, -0.55f, 0.70f, 0.0f );
    }
    return c;
  }
  inline TStC StTitle( int st, float X, float Y, bool cap ) {
    TStC c = StNone();
    const float ss = StTitleSd( st, X, Y - 2.2f );
    const float sa = 0.50f * ( 1.0f - StSm( -1.0f, 3.5f, ss ) );
    if( sa > 0.0f ) c = StRgb( 0, 0, 0, sa );
    const float sd = StTitleSd( st, X, Y );
    if( sd < 0.0f ) c = StOver( StTitleFill( st, X, Y, sd, cap ), c );
    return c;
  }
  struct TStAtlas {
    int     style;
    bool    full;
    float   k;
    TStSpec sp;
    int     ex, ey, ew, eh, eo;
    float   tpx;
    int     cx, cy;
    int     ox, oy, os;
    int     tx, ty, tw, th, mx, my, mw, tpad;
    float   tTextH;
  };
  inline unsigned long StArgb( const TStC& c ) {
    if( c.a <= 0.002f ) return 0;
    const float ia = 1.0f / c.a;
    float r = c.r * ia, g = c.g * ia, b = c.b * ia, a = c.a;
    if( r > 1.0f ) r = 1.0f;
    if( g > 1.0f ) g = 1.0f;
    if( b > 1.0f ) b = 1.0f;
    if( a > 1.0f ) a = 1.0f;
    return ( (unsigned long)( a * 255.0f + 0.5f ) << 24 )
         | ( (unsigned long)( r * 255.0f + 0.5f ) << 16 )
         | ( (unsigned long)( g * 255.0f + 0.5f ) << 8 )
         |   (unsigned long)( b * 255.0f + 0.5f );
  }
  inline bool StBuildAtlas( int st, bool full, float k, unsigned long* px, TStAtlas& at ) {
    const int n = ST_ATLAS;
    memset( px, 0, (size_t)n * (size_t)n * 4 );
    memset( &at, 0, sizeof( at ) );
    if( st < 1 || st > 3 || !px ) return false;
    const TStSpec s = StSpec( st, full );
    at.style = st;
    at.full  = full;
    at.k     = k;
    at.sp    = s;
    at.tpx   = s.T * k;
    const float Lt = s.P * (float)s.per;
    at.ew = (int)( Lt * k + 0.5f );
    if( at.ew < 8 ) at.ew = 8;
    at.eo = (int)( ( s.T + s.S ) * k + 0.5f );
    int ip = (int)( s.I * k + 0.5f );
    if( ip < 1 ) ip = 1;
    at.eh = at.eo + ip;
    at.ex = 2;
    at.ey = 2;
    const float inv16 = 1.0f / 16.0f;
    for( int y = 0; y < at.eh; y++ ) {
      for( int x = -1; x <= at.ew; x++ ) {
        const int xw = ( x < 0 ) ? at.ew - 1 : ( ( x >= at.ew ) ? 0 : x );
        TStC acc = StNone();
        for( int j = 0; j < 4; j++ ) {
          const float t = ( (float)at.eo - ( (float)y + ( (float)j + 0.5f ) * 0.25f ) ) / k;
          for( int i = 0; i < 4; i++ ) {
            const float a = ( (float)xw + ( (float)i + 0.5f ) * 0.25f ) / (float)at.ew * Lt;
            const TStC c = StEdge( st, full, s, a, t, Lt );
            acc.r += c.r; acc.g += c.g; acc.b += c.b; acc.a += c.a;
          }
        }
        acc.r *= inv16; acc.g *= inv16; acc.b *= inv16; acc.a *= inv16;
        px[( at.ey + y ) * n + at.ex + x] = StArgb( acc );
      }
    }
    at.cx = 2;
    at.cy = at.ey + at.eh + 3;
    for( int y = 0; y <= at.eo; y++ ) {
      for( int x = 0; x <= at.eo; x++ ) {
        TStC acc = StNone();
        for( int j = 0; j < 4; j++ ) {
          const float bp = ( (float)at.eo - ( (float)y + ( (float)j + 0.5f ) * 0.25f ) ) / k;
          for( int i = 0; i < 4; i++ ) {
            const float ap = ( (float)at.eo - ( (float)x + ( (float)i + 0.5f ) * 0.25f ) ) / k;
            const TStC c = StCorner( st, full, s, ap, bp );
            acc.r += c.r; acc.g += c.g; acc.b += c.b; acc.a += c.a;
          }
        }
        acc.r *= inv16; acc.g *= inv16; acc.b *= inv16; acc.a *= inv16;
        px[( at.cy + y ) * n + at.cx + x] = StArgb( acc );
      }
    }
    at.ox = at.cx + at.eo + 4;
    at.oy = at.cy;
    at.os = 0;
    if( s.orn > 0.0f ) {
      at.os = (int)ceilf( 2.0f * ( s.orn + 2.4f ) * k );
      const float c0 = (float)at.os * 0.5f;
      for( int y = 0; y < at.os; y++ ) {
        for( int x = 0; x < at.os; x++ ) {
          TStC acc = StNone();
          for( int j = 0; j < 4; j++ ) {
            const float dy = ( (float)y + ( (float)j + 0.5f ) * 0.25f - c0 ) / k;
            for( int i = 0; i < 4; i++ ) {
              const float dx = ( (float)x + ( (float)i + 0.5f ) * 0.25f - c0 ) / k;
              const TStC c = StOrn( st, s, dx, dy );
              acc.r += c.r; acc.g += c.g; acc.b += c.b; acc.a += c.a;
            }
          }
          acc.r *= inv16; acc.g *= inv16; acc.b *= inv16; acc.a *= inv16;
          px[( at.oy + y ) * n + at.ox + x] = StArgb( acc );
        }
      }
    }
    if( full ) {
      at.tpad = (int)( ST_TPAD * k + 0.5f );
      const int plate = (int)( ST_TH * k + 0.5f );
      at.th = plate + 2 * at.tpad;
      at.tw = at.tpad + (int)( s.capW * k + 0.5f );
      at.tx = 2;
      int row = at.cy + at.eo + 4;
      if( at.os > 0 && at.oy + at.os + 3 > row ) row = at.oy + at.os + 3;
      at.ty = row;
      at.mw = 4;
      at.mx = at.tx + at.tw + 4;
      at.my = at.ty;
      at.tTextH = ST_TH * 0.74f * k;
      for( int y = 0; y < at.th; y++ ) {
        for( int x = 0; x <= at.tw + 1 + at.mw; x++ ) {
          const bool cap = ( x <= at.tw );
          if( !cap && x < at.tw + 2 ) continue;
          TStC acc = StNone();
          for( int j = 0; j < 4; j++ ) {
            const float Y = ( (float)y + ( (float)j + 0.5f ) * 0.25f - (float)at.tpad ) / k;
            for( int i = 0; i < 4; i++ ) {
              float X;
              if( cap )
                X = ( (float)x + ( (float)i + 0.5f ) * 0.25f - (float)at.tpad ) / k;
              else
                X = s.capW + 4.0f;
              const TStC c = StTitle( st, X, Y, cap && X < s.capW );
              acc.r += c.r; acc.g += c.g; acc.b += c.b; acc.a += c.a;
            }
          }
          acc.r *= inv16; acc.g *= inv16; acc.b *= inv16; acc.a *= inv16;
          const unsigned long v = StArgb( acc );
          if( cap )
            px[( at.ty + y ) * n + at.tx + x] = v;
          else
            for( int m = -1; m <= at.mw; m++ )
              px[( at.my + y ) * n + at.mx + m] = v;
        }
      }
      if( at.ty + at.th + 2 > n || at.mx + at.mw + 2 > n ) return false;
    }
    return ( at.ex + at.ew + 2 < n ) && ( at.cy + at.eo + 2 < n )
        && ( at.ox + at.os + 2 < n );
  }
  struct TStQuad {
    float x[4], y[4], u[4], v[4];
    unsigned char r, g, b, a;
    unsigned char white;
  };
  inline void StSet( TStQuad& q, int i, float x, float y, float u, float v ) {
    q.x[i] = x; q.y[i] = y;
    q.u[i] = u * ( 1.0f / (float)ST_ATLAS );
    q.v[i] = v * ( 1.0f / (float)ST_ATLAS );
  }
  inline void StWhite( TStQuad& q ) { q.r = q.g = q.b = q.a = 255; q.white = 0; }
  inline int StDimRect( TStQuad* q, int n, int cap, float x0, float y0, float x1, float y1 ) {
    if( n >= cap || x1 <= x0 || y1 <= y0 ) return n;
    TStQuad& d = q[n];
    StSet( d, 0, x0, y0, 0.0f, 0.0f );
    StSet( d, 1, x1, y0, 0.0f, 0.0f );
    StSet( d, 2, x1, y1, 0.0f, 0.0f );
    StSet( d, 3, x0, y1, 0.0f, 0.0f );
    d.r = d.g = d.b = 0;
    d.a = (unsigned char)ST_DIM;
    d.white = 1;
    return n + 1;
  }
  inline void StSidePt( int side, float x, float y, float w, float h,
                        float s, float d, float& px, float& py ) {
    switch( side ) {
      case 0:  px = x + s;     py = y - d;     break;
      case 1:  px = x + w + d; py = y + s;     break;
      case 2:  px = x + w - s; py = y + h + d; break;
      default: px = x - d;     py = y + h - s; break;
    }
  }
  inline int StOrnQuad( const TStAtlas& at, TStQuad* q, int n, int cap, float cx, float cy ) {
    if( at.os <= 0 || n >= cap ) return n;
    const float h = (float)at.os * 0.5f;
    TStQuad& o = q[n];
    StWhite( o );
    StSet( o, 0, cx - h, cy - h, (float)at.ox,             (float)at.oy );
    StSet( o, 1, cx + h, cy - h, (float)( at.ox + at.os ), (float)at.oy );
    StSet( o, 2, cx + h, cy + h, (float)( at.ox + at.os ), (float)( at.oy + at.os ) );
    StSet( o, 3, cx - h, cy + h, (float)at.ox,             (float)( at.oy + at.os ) );
    return n + 1;
  }
  inline int StLayoutFrame( const TStAtlas& at, int x, int y, int w, int h,
                            bool circle, float ccx, float ccy, float R,
                            int sw, int sh, TStQuad* q, int cap ) {
    int n = 0;
    if( at.style < 1 || w <= 0 || h <= 0 ) return 0;
    const float fx = (float)x, fy = (float)y, fw = (float)w, fh = (float)h;
    const float dOut = (float)at.eo;
    const float dIn  = (float)( at.eo - at.eh );
    const float vOut = (float)at.ey, vIn = (float)( at.ey + at.eh );
    const float pw   = (float)at.ew / (float)at.sp.per;
    const float Pp   = at.sp.P * at.k;
    if( at.full && !circle ) {
      const float o = floorf( at.tpx );
      n = StDimRect( q, n, cap, 0.0f, 0.0f, (float)sw, fy - o );
      n = StDimRect( q, n, cap, 0.0f, fy + fh + o, (float)sw, (float)sh );
      n = StDimRect( q, n, cap, 0.0f, fy - o, fx - o, fy + fh + o );
      n = StDimRect( q, n, cap, fx + fw + o, fy - o, (float)sw, fy + fh + o );
    }
    if( circle ) {
      int step = 4;
      while( step % at.sp.per )
        step += 4;
      const float Rm = R + at.tpx * 0.5f;
      int np = (int)( 6.2831853f * Rm / Pp / (float)step + 0.5f ) * step;
      if( np < step ) np = step;
      int sub = (int)ceilf( 6.2831853f * ( R + dOut ) / 10.0f / (float)np );
      if( sub < 1 ) sub = 1; else if( sub > 16 ) sub = 16;
      while( np * sub > cap - n - 8 && sub > 1 )
        sub--;
      const int   tot = np * sub;
      const float r0  = R + dIn, r1 = R + dOut;
      float pc = 0.0f, ps = -1.0f;
      for( int i = 0; i < tot && n < cap; i++ ) {
        const float an = 6.2831853f * (float)( i + 1 ) / (float)tot - 1.5707963f;
        const float nc = cosf( an ), ns = sinf( an );
        const int   p  = ( i / sub ) % at.sp.per, j = i % sub;
        const float ua = (float)at.ex + pw * ( (float)p + (float)j / (float)sub );
        const float ub = (float)at.ex + pw * ( (float)p + (float)( j + 1 ) / (float)sub );
        TStQuad& d = q[n++];
        StWhite( d );
        StSet( d, 0, ccx + r1 * pc, ccy + r1 * ps, ua, vOut );
        StSet( d, 1, ccx + r1 * nc, ccy + r1 * ns, ub, vOut );
        StSet( d, 2, ccx + r0 * nc, ccy + r0 * ns, ub, vIn );
        StSet( d, 3, ccx + r0 * pc, ccy + r0 * ps, ua, vIn );
        pc = nc;
        ps = ns;
      }
      const float ro = R + at.tpx * 0.5f;
      for( int i = 0; i < 4; i++ ) {
        const float an = 0.7853982f + 1.5707963f * (float)i;
        n = StOrnQuad( at, q, n, cap, ccx + ro * cosf( an ), ccy + ro * sinf( an ) );
      }
      return n;
    }
    for( int side = 0; side < 4; side++ ) {
      const float len = ( side & 1 ) ? fh : fw;
      int np = (int)( len / Pp + 0.5f );
      if( np < 1 ) np = 1;
      const float pl = len / (float)np;
      for( int p0 = 0; p0 < np && n < cap; p0 += at.sp.per ) {
        const int m = ( np - p0 < at.sp.per ) ? np - p0 : at.sp.per;
        const float s0 = pl * (float)p0, s1 = pl * (float)( p0 + m );
        const float ua = (float)at.ex, ub = (float)at.ex + pw * (float)m;
        float ax, ay;
        TStQuad& d = q[n++];
        StWhite( d );
        StSidePt( side, fx, fy, fw, fh, s0, dOut, ax, ay ); StSet( d, 0, ax, ay, ua, vOut );
        StSidePt( side, fx, fy, fw, fh, s1, dOut, ax, ay ); StSet( d, 1, ax, ay, ub, vOut );
        StSidePt( side, fx, fy, fw, fh, s1, dIn,  ax, ay ); StSet( d, 2, ax, ay, ub, vIn );
        StSidePt( side, fx, fy, fw, fh, s0, dIn,  ax, ay ); StSet( d, 3, ax, ay, ua, vIn );
      }
    }
    const float c0x = (float)at.cx, c0y = (float)at.cy;
    const float c1x = (float)( at.cx + at.eo ), c1y = (float)( at.cy + at.eo );
    const float tu[4] = { c0x, c1x, c1x, c0x };
    const float tv[4] = { c0y, c0y, c1y, c1y };
    for( int c = 0; c < 4 && n < cap; c++ ) {
      const float ox = ( c == 1 || c == 2 ) ? fx + fw : fx - dOut;
      const float oy = ( c >= 2 ) ? fy + fh : fy - dOut;
      TStQuad& d = q[n++];
      StWhite( d );
      const float sx[4] = { ox, ox + dOut, ox + dOut, ox };
      const float sy[4] = { oy, oy, oy + dOut, oy + dOut };
      for( int i = 0; i < 4; i++ ) {
        const int k = ( i - c + 4 ) & 3;
        StSet( d, i, sx[i], sy[i], tu[k], tv[k] );
      }
    }
    const float hm = at.tpx * 0.5f;
    n = StOrnQuad( at, q, n, cap, fx - hm,      fy - hm );
    n = StOrnQuad( at, q, n, cap, fx + fw + hm, fy - hm );
    n = StOrnQuad( at, q, n, cap, fx + fw + hm, fy + fh + hm );
    n = StOrnQuad( at, q, n, cap, fx - hm,      fy + fh + hm );
    return n;
  }
  inline float StTitleCY( const TStAtlas& at, int winY ) {
    const float hp = ST_TH * at.k * 0.5f;
    float cy = (float)winY - ( at.sp.T * 0.5f + 3.0f ) * at.k;
    if( cy - hp < 2.0f ) cy = hp + 2.0f;
    return cy;
  }
  inline float StPlateBelow( const TStAtlas& at ) {
    const float d = ( ST_TH * 0.5f - at.sp.T * 0.5f - 3.0f ) * at.k;
    return ( d < 0.0f ) ? 0.0f : d;
  }
  inline int StFullMinMargin( int st, float ui ) {
    if( st < ST_PARCH || st > ST_GOLD ) return 0;
    const TStSpec s = StSpec( st, true );
    const float k = StScaleFor( st, true, ui );
    const float top  = ST_TH * 0.5f + s.T * 0.5f + 3.0f + ST_TPAD + 2.0f;
    const float side = s.T + s.S + 2.0f;
    return (int)ceilf( ( ( top > side ) ? top : side ) * k );
  }
  inline int StLayoutTitle( const TStAtlas& at, float midX, float cy, float textW,
                            TStQuad* q, int cap ) {
    if( !at.full || at.th <= 0 || cap < 3 ) return 0;
    const float mh  = textW * 0.5f + ST_TGAP * at.k;
    const float top = cy - ST_TH * at.k * 0.5f - (float)at.tpad;
    const float bot = top + (float)at.th;
    const float l   = floorf( midX - mh + 0.5f ), r = l + floorf( 2.0f * mh + 0.5f );
    const float tw  = (float)at.tw;
    const float t0  = (float)at.ty, t1 = (float)( at.ty + at.th );
    StWhite( q[0] );
    StSet( q[0], 0, l - tw, top, (float)at.tx,        t0 );
    StSet( q[0], 1, l,      top, (float)( at.tx + at.tw ), t0 );
    StSet( q[0], 2, l,      bot, (float)( at.tx + at.tw ), t1 );
    StSet( q[0], 3, l - tw, bot, (float)at.tx,        t1 );
    const float mu = (float)at.mx + (float)at.mw * 0.5f;
    StWhite( q[1] );
    StSet( q[1], 0, l, top, mu, t0 );
    StSet( q[1], 1, r, top, mu, t0 );
    StSet( q[1], 2, r, bot, mu, t1 );
    StSet( q[1], 3, l, bot, mu, t1 );
    StWhite( q[2] );
    StSet( q[2], 0, r,      top, (float)( at.tx + at.tw ), t0 );
    StSet( q[2], 1, r + tw, top, (float)at.tx,        t0 );
    StSet( q[2], 2, r + tw, bot, (float)at.tx,        t1 );
    StSet( q[2], 3, r,      bot, (float)( at.tx + at.tw ), t1 );
    return 3;
  }
}
