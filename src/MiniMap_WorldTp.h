#pragma once
namespace WorldTp {
  using namespace Gothic_II_Addon;
  enum { WT_PUSHINT = zPAR_TOK_PUSHINT, WT_PUSHVAR = zPAR_TOK_PUSHVAR, WT_PUSHINST = zPAR_TOK_PUSHINST,
         WT_CALLEXT = zPAR_TOK_CALLEXTERN, WT_CALL = zPAR_TOK_CALL, WT_RET = zPAR_TOK_RET,
         WT_JUMP = zPAR_TOK_JUMP, WT_JUMPF = zPAR_TOK_JUMPF, WT_SETINST = zPAR_TOK_SETINSTANCE,
         WT_ARRAY = zPAR_TOK_PUSHVAR + zPAR_TOK_FLAGARRAY };
  enum { WT_MAX_DEST = 16, WT_GUARD = 20000 };
  static const float WT_MIN_D = 500.0f;
  static const float WT_SRC_D = 3000.0f;
  inline int WtTokLen( unsigned char t ) { return PlaceLearn::PlTokLen( t ); }
  typedef bool ( *TWtIsStr )( void* ctx, int index );
  struct TWtDest {
    int dest;
    int src;
    int at;
  };
  struct TWtSyms {
    int tele;
    int dist;
    int nearest;
    int strcmp_;
    int hero, self;
  };
  inline int WtDests( const unsigned char* code, int size, int adr, void* ctx, TWtIsStr isStr,
                      const TWtSyms& y, TWtDest* out ) {
    if( !code || !isStr || adr < 0 || adr >= size || y.tele < 0 ) return 0;
    int n = 0, jumpMax = 0, i = adr;
    unsigned char t1 = 0, t2 = 0;
    int a1 = 0, a2 = 0;
    int src = -1;
    for( int guard = 0; i < size && guard < WT_GUARD; guard++ ) {
      const unsigned char t = code[i];
      if( t == WT_RET && jumpMax <= i ) break;
      const int len = WtTokLen( t );
      if( i + len > size ) break;
      int arg = 0;
      if( len >= 5 )
        memcpy( &arg, code + i + 1, 4 );
      if( ( t == WT_JUMP || t == WT_JUMPF ) && arg > jumpMax && arg < size ) jumpMax = arg;
      if( t == WT_CALLEXT ) {
        if( y.dist >= 0 && arg == y.dist ) {
          if( t1 == WT_PUSHVAR && isStr( ctx, a1 ) ) src = a1;
        }
        else if( y.strcmp_ >= 0 && arg == y.strcmp_ ) {
          if( t1 == WT_PUSHVAR && isStr( ctx, a1 ) && t2 == WT_CALLEXT && y.nearest >= 0 && a2 == y.nearest ) src = a1;
        }
        else if( arg == y.tele ) {
          const bool pl = t2 == WT_PUSHINST && ( ( y.hero >= 0 && a2 == y.hero ) || ( y.self >= 0 && a2 == y.self ) );
          if( pl && t1 == WT_PUSHVAR && isStr( ctx, a1 ) && n < WT_MAX_DEST ) {
            bool dup = false;
            for( int k = 0; k < n && !dup; k++ ) dup = out[k].dest == a1 && out[k].src == src;
            if( !dup ) { out[n].dest = a1; out[n].src = src; out[n].at = i; n++; }
          }
          src = -1;
        }
      }
      t2 = t1; a2 = a1;
      t1 = t;  a1 = arg;
      i += len;
    }
    return n;
  }
  typedef bool ( *TWtWpPos )( void* ctx, int strIndex, float& x, float& z );
  inline int WtChoose( const TWtDest* d, int n, float cx, float cz, void* ctx, TWtWpPos pos ) {
    int best = -1, fb = -1, nfb = 0;
    float bestD = 0.0f;
    for( int k = 0; k < n; k++ ) {
      float tx, tz;
      if( !pos( ctx, d[k].dest, tx, tz ) ) continue;
      const float dx = tx - cx, dz = tz - cz;
      if( dx * dx + dz * dz <= WT_MIN_D * WT_MIN_D ) continue;
      if( d[k].src < 0 ) {
        bool same = false;
        for( int j = 0; j < k && !same; j++ )
          same = d[j].src < 0 && d[j].dest == d[k].dest;
        if( !same ) { if( !nfb ) fb = k; nfb++; }
        continue;
      }
      float sx, sz;
      if( !pos( ctx, d[k].src, sx, sz ) ) continue;
      const float ex = sx - cx, ez = sz - cz;
      const float sd = ex * ex + ez * ez;
      if( sd <= WT_SRC_D * WT_SRC_D && ( best < 0 || sd < bestD ) ) { best = k; bestD = sd; }
    }
    if( best >= 0 ) return best;
    return ( nfb == 1 ) ? fb : -1;
  }
}
