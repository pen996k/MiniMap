#pragma once
#define STBI_NO_STDIO
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_NO_THREAD_LOCALS
#define STBI_ONLY_PNG
#define STBI_ONLY_TGA
#define STBI_MAX_DIMENSIONS 1024
#include "third_party/stb_image.h"
namespace MiniMapImage {
  enum {
    IMG_MAX_SIDE  = 1024,
    IMG_MAX_BYTES = 8 * 1024 * 1024,
    CELL_MAX      = 64
  };
  enum { KEEPCOLORS = 1, NOOUTLINE = 2 };
  inline unsigned char* DecodeCore( const unsigned char* data, int len, int* w, int* h ) {
    int comp = 0;
    return stbi_load_from_memory( data, len, w, h, &comp, 4 );
  }
  inline bool IsPng( const unsigned char* d, int len ) {
    return len >= 8 && d[0] == 0x89 && d[1] == 'P' && d[2] == 'N' && d[3] == 'G'
        && d[4] == 0x0D && d[5] == 0x0A && d[6] == 0x1A && d[7] == 0x0A;
  }
  inline const char* Decode( const unsigned char* data, int len,
                             unsigned char*& rgba, int& w, int& h ) {
    rgba = 0;
    w = h = 0;
    if( !data || len <= 0 ) return "file is empty";
    if( len > IMG_MAX_BYTES ) return "file is larger than 8 MB";
    const bool png = IsPng( data, len );
    unsigned char* px = 0;
    int iw = 0, ih = 0;
    __try {
      px = DecodeCore( data, len, &iw, &ih );
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return "broken image file (the decoder stopped on it)";
    }
    if( !px ) {
      const char* r = stbi_failure_reason();
      if( r && ( !strcmp( r, "too large" ) ) ) return "image is larger than 1024 x 1024 pixels";
      if( r && !strcmp( r, "outofmem" ) ) return "not enough memory for this image";
      if( !png ) return "not a PNG or TGA image (or an unsupported TGA variant)";
      return "broken or unsupported PNG file";
    }
    if( iw <= 0 || ih <= 0 || iw > IMG_MAX_SIDE || ih > IMG_MAX_SIDE ) {
      stbi_image_free( px );
      return "image is larger than 1024 x 1024 pixels";
    }
    rgba = px;
    w = iw;
    h = ih;
    return 0;
  }
  inline void Free( unsigned char* rgba ) {
    if( rgba )
      stbi_image_free( rgba );
  }
  struct TSrc {
    const unsigned char* px;
    int  w, h;
    int  x0, y0, cw, ch;
    bool keyed;
  };
  inline int ColDist( const unsigned char* a, const unsigned char* b ) {
    const int dr = (int)a[0] - (int)b[0], dg = (int)a[1] - (int)b[1], db = (int)a[2] - (int)b[2];
    return ( dr < 0 ? -dr : dr ) + ( dg < 0 ? -dg : dg ) + ( db < 0 ? -db : db );
  }
  inline bool KeyBackground( unsigned char* px, int w, int h ) {
    if( !px || w < 2 || h < 2 ) return false;
    const int n = w * h;
    for( int i = 0; i < n; i++ )
      if( px[i * 4 + 3] < 250 ) return false;
    unsigned char key[4];
    memcpy( key, px, 4 );
    const unsigned char* c[3] = { px + ( w - 1 ) * 4, px + ( h - 1 ) * w * 4, px + ( n - 1 ) * 4 };
    for( int k = 0; k < 3; k++ )
      if( ColDist( c[k], key ) > 24 ) return false;
    int same = 0;
    for( int i = 0; i < n; i++ )
      if( ColDist( px + i * 4, key ) <= 24 )
        same++;
    if( same * 10 < n ) return false;
    for( int i = 0; i < n; i++ ) {
      const int d = ColDist( px + i * 4, key );
      if( d <= 12 )
        px[i * 4 + 3] = 0;
      else if( d < 36 )
        px[i * 4 + 3] = (unsigned char)( ( d - 12 ) * 255 / 24 );
    }
    return true;
  }
  inline bool IsColored( const unsigned char* px, int w, int h ) {
    if( !px || w <= 0 || h <= 0 ) return false;
    int vis = 0, col = 0;
    for( int i = 0; i < w * h; i++ ) {
      const unsigned char* p = px + i * 4;
      if( p[3] <= 32 ) continue;
      vis++;
      int mx = p[0], mn = p[0];
      for( int c = 1; c < 3; c++ ) { if( p[c] > mx ) mx = p[c]; if( p[c] < mn ) mn = p[c]; }
      if( mx - mn > 24 )
        col++;
    }
    return vis > 0 && col * 50 >= vis;
  }
  inline bool Prep( unsigned char* px, int w, int h, TSrc& s ) {
    memset( &s, 0, sizeof( s ) );
    if( !px || w <= 0 || h <= 0 ) return false;
    s.keyed = KeyBackground( px, w, h );
    int x0 = w, y0 = h, x1 = -1, y1 = -1;
    for( int y = 0; y < h; y++ ) {
      const unsigned char* row = px + y * w * 4;
      for( int x = 0; x < w; x++ ) {
        if( row[x * 4 + 3] > 8 ) {
          if( x < x0 ) x0 = x;
          if( x > x1 ) x1 = x;
          if( y < y0 ) y0 = y;
          if( y > y1 ) y1 = y;
        }
      }
    }
    if( x1 < x0 || y1 < y0 ) return false;
    s.px = px;
    s.w  = w;
    s.h  = h;
    s.x0 = x0;
    s.y0 = y0;
    s.cw = x1 - x0 + 1;
    s.ch = y1 - y0 + 1;
    return true;
  }
  inline constexpr float kDir16[16][2] = {
    { 1.0f, 0.0f }, { -1.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, -1.0f },
    { 0.707f, 0.707f }, { -0.707f, 0.707f }, { 0.707f, -0.707f }, { -0.707f, -0.707f },
    { 0.924f, 0.383f }, { -0.924f, 0.383f }, { 0.924f, -0.383f }, { -0.924f, -0.383f },
    { 0.383f, 0.924f }, { -0.383f, 0.924f }, { 0.383f, -0.924f }, { -0.383f, -0.924f }
  };
  inline void SamplePx( const unsigned char* px, int w, int h, float fx, float fy, float wgt, bool clamp, float o[4] ) {
    fx -= 0.5f; fy -= 0.5f;
    if( clamp ) {
      if( fx < 0.0f ) fx = 0.0f; else if( fx > (float)( w - 1 ) ) fx = (float)( w - 1 );
      if( fy < 0.0f ) fy = 0.0f; else if( fy > (float)( h - 1 ) ) fy = (float)( h - 1 );
    }
    const float flx = floorf( fx ), fly = floorf( fy );
    const int   ix  = (int)flx, iy = (int)fly;
    const float tx  = fx - flx, ty = fy - fly;
    for( int dy = 0; dy < 2; dy++ ) {
      int y = iy + dy;
      if( clamp ) { if( y >= h ) y = h - 1; }
      else if( y < 0 || y >= h ) continue;
      const float wy = dy ? ty : 1.0f - ty;
      for( int dx = 0; dx < 2; dx++ ) {
        int x = ix + dx;
        if( clamp ) { if( x >= w ) x = w - 1; }
        else if( x < 0 || x >= w ) continue;
        const float k = wy * ( dx ? tx : 1.0f - tx ) * wgt;
        if( k <= 0.0f ) continue;
        const unsigned char* p = px + ( y * w + x ) * 4;
        const float a = (float)p[3] * ( 1.0f / 255.0f ) * k;
        o[0] += (float)p[0] * a; o[1] += (float)p[1] * a; o[2] += (float)p[2] * a; o[3] += a;
      }
    }
  }
  inline float GridA( const float* g, int S, float x, float y ) {
    x -= 0.5f;
    y -= 0.5f;
    const float flx = floorf( x ), fly = floorf( y );
    const int   ix  = (int)flx, iy = (int)fly;
    const float tx  = x - flx, ty = y - fly;
    float r = 0.0f;
    for( int dy = 0; dy < 2; dy++ ) {
      const int yy = iy + dy;
      if( yy < 0 || yy >= S ) continue;
      for( int dx = 0; dx < 2; dx++ ) {
        const int xx = ix + dx;
        if( xx < 0 || xx >= S ) continue;
        r += g[yy * S + xx] * ( dy ? ty : 1.0f - ty ) * ( dx ? tx : 1.0f - tx );
      }
    }
    return r;
  }
  inline void Bake( const TSrc& s, int S, unsigned flags, unsigned long* dst, int pitch ) {
    if( !s.px || !dst || S < 4 || S > CELL_MAX || s.cw <= 0 || s.ch <= 0 ) return;
    static float A[CELL_MAX * CELL_MAX], C[CELL_MAX * CELL_MAX][3];
    const int   side = ( s.cw > s.ch ) ? s.cw : s.ch;
    const float D    = (float)S - 3.2f;
    const float f    = (float)side / D;
    int ss = (int)ceilf( f );
    if( ss < 2 ) ss = 2; else if( ss > 8 ) ss = 8;
    const float cx = (float)s.x0 + (float)s.cw * 0.5f;
    const float cy = (float)s.y0 + (float)s.ch * 0.5f;
    const float inv = 1.0f / (float)( ss * ss );
    for( int ty = 0; ty < S; ty++ ) {
      for( int tx = 0; tx < S; tx++ ) {
        float acc[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        for( int sy = 0; sy < ss; sy++ ) {
          const float v = (float)ty + ( (float)sy + 0.5f ) / (float)ss - (float)S * 0.5f;
          for( int sx = 0; sx < ss; sx++ ) {
            const float u = (float)tx + ( (float)sx + 0.5f ) / (float)ss - (float)S * 0.5f;
            SamplePx( s.px, s.w, s.h, cx + u * f, cy + v * f, 1.0f, false, acc );
          }
        }
        const int q = ty * S + tx;
        A[q]    = acc[3] * inv;
        C[q][0] = acc[0] * inv;
        C[q][1] = acc[1] * inv;
        C[q][2] = acc[2] * inv;
      }
    }
    const float dk[3] = { 18.0f, 16.0f, 15.0f };
    const float tpu   = ( (float)S * 0.5f - 1.6f ) / 0.9f;
    for( int ty = 0; ty < S; ty++ ) {
      const float vv    = ( (float)ty + 0.5f - (float)S * 0.5f ) / tpu;
      const float shade = 1.08f - 0.16f * vv;
      for( int tx = 0; tx < S; tx++ ) {
        const int   q = ty * S + tx;
        const float a = ( A[q] > 1.0f ) ? 1.0f : A[q];
        float o = 0.0f;
        if( !( flags & NOOUTLINE ) ) {
          const float px = (float)tx + 0.5f, py = (float)ty + 0.5f;
          for( int d = 0; d < 16 && o < 0.999f; d++ ) {
            const float g1 = GridA( A, S, px + kDir16[d][0] * 1.1f,  py + kDir16[d][1] * 1.1f );
            const float g2 = GridA( A, S, px + kDir16[d][0] * 0.55f, py + kDir16[d][1] * 0.55f );
            if( g1 > o ) o = g1;
            if( g2 > o ) o = g2;
          }
          if( o > 1.0f ) o = 1.0f;
        }
        const float oa  = o * ( 1.0f - a );
        const float tot = a + oa;
        unsigned long argb = 0;
        if( tot >= 1.0f / 255.0f ) {
          float rgb[3];
          if( a > 0.0f ) {
            const float r = C[q][0] / A[q], g = C[q][1] / A[q], b = C[q][2] / A[q];
            if( flags & KEEPCOLORS ) {
              rgb[0] = r; rgb[1] = g; rgb[2] = b;
            }
            else {
              float l = ( 0.299f * r + 0.587f * g + 0.114f * b ) * shade * 0.925f;
              if( l > 255.0f ) l = 255.0f;
              rgb[0] = rgb[1] = rgb[2] = l;
            }
          }
          else
            rgb[0] = rgb[1] = rgb[2] = 0.0f;
          unsigned long cc[3];
          for( int ch = 0; ch < 3; ch++ ) {
            float v = ( rgb[ch] * a + dk[ch] * oa ) / tot;
            if( v < 0.0f ) v = 0.0f; else if( v > 255.0f ) v = 255.0f;
            cc[ch] = (unsigned long)( v + 0.5f );
          }
          float al = tot * 255.0f + 0.5f;
          if( al > 255.0f ) al = 255.0f;
          argb = ( (unsigned long)al << 24 ) | ( cc[0] << 16 ) | ( cc[1] << 8 ) | cc[2];
        }
        dst[ty * pitch + tx] = argb;
      }
    }
  }
  inline void Tint( unsigned long* px, int n, int r, int g, int b ) {
    if( !px || n <= 0 ) return;
    const int c[3] = { r, g, b };
    for( int i = 0; i < n; i++ ) {
      const unsigned long p = px[i];
      if( !( p >> 24 ) ) continue;
      unsigned long out = p & 0xFF000000UL;
      for( int ch = 0; ch < 3; ch++ ) {
        const int sh = 16 - ch * 8;
        int v = (int)( ( p >> sh ) & 255 ) * c[ch] / 236;
        if( v > 255 ) v = 255;
        if( v < 0 ) v = 0;
        out |= (unsigned long)v << sh;
      }
      px[i] = out;
    }
  }
  inline void Resize( const unsigned char* px, int w, int h, unsigned long* dst, int pitch, int dw, int dh ) {
    if( !px || !dst || w <= 0 || h <= 0 || dw <= 0 || dh <= 0 || pitch < dw ) return;
    int sx = (int)ceilf( (float)w / (float)dw ), sy = (int)ceilf( (float)h / (float)dh );
    if( sx < 1 ) sx = 1; else if( sx > 8 ) sx = 8;
    if( sy < 1 ) sy = 1; else if( sy > 8 ) sy = 8;
    const float kx = (float)w / (float)dw, ky = (float)h / (float)dh;
    const float wgt = 1.0f / (float)( sx * sy );
    for( int y = 0; y < dh; y++ )
      for( int x = 0; x < dw; x++ ) {
        float o[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        for( int j = 0; j < sy; j++ )
          for( int i = 0; i < sx; i++ )
            SamplePx( px, w, h, ( (float)x + ( (float)i + 0.5f ) / (float)sx ) * kx,
                         ( (float)y + ( (float)j + 0.5f ) / (float)sy ) * ky, wgt, true, o );
        unsigned long argb = 0;
        if( o[3] > 0.002f ) {
          unsigned long c[3];
          for( int ch = 0; ch < 3; ch++ ) {
            float v = o[ch] / o[3];
            if( v > 255.0f ) v = 255.0f;
            if( v < 0.0f )   v = 0.0f;
            c[ch] = (unsigned long)( v + 0.5f );
          }
          float a = o[3] * 255.0f + 0.5f;
          if( a > 255.0f ) a = 255.0f;
          argb = ( (unsigned long)a << 24 ) | ( c[0] << 16 ) | ( c[1] << 8 ) | c[2];
        }
        dst[y * pitch + x] = argb;
      }
  }
  inline unsigned long Crc32( const unsigned char* p, int n, unsigned long crc = 0 ) {
    return ( p && n > 0 ) ? (unsigned long)::crc32( (uLong)crc, (const Bytef*)p, (uInt)n ) : crc;
  }
}
