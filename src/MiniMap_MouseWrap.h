#pragma once
namespace MouseUnionWrap {
  struct TPattern {
    unsigned int player;
    unsigned int counterX;
  };
  inline int FindSkip( const unsigned char* img, int size, const TPattern& p,
                       bool* wasOurs ) {
    if( wasOurs ) *wasOurs = false;
    if( !img || size < 18 || !p.player || !p.counterX ) return -1;
    const unsigned char pl[4] = {
      (unsigned char)( p.player & 0xFF ), (unsigned char)( ( p.player >> 8 ) & 0xFF ),
      (unsigned char)( ( p.player >> 16 ) & 0xFF ), (unsigned char)( ( p.player >> 24 ) & 0xFF ) };
    const unsigned char cx[4] = {
      (unsigned char)( p.counterX & 0xFF ), (unsigned char)( ( p.counterX >> 8 ) & 0xFF ),
      (unsigned char)( ( p.counterX >> 16 ) & 0xFF ), (unsigned char)( ( p.counterX >> 24 ) & 0xFF ) };
    int found = -1;
    for( int i = 0; i + 18 <= size; i++ ) {
      const unsigned char* c = img + i;
      if( c[0] != 0x39 || c[1] != 0x1D ) continue;
      if( c[2] != pl[0] || c[3] != pl[1] || c[4] != pl[2] || c[5] != pl[3] ) continue;
      if( ( c[6] != 0x74 && c[6] != 0xEB ) || c[7] != 0x0A || c[8] != 0xE8 || c[13] != 0xA3 ) continue;
      if( c[14] != cx[0] || c[15] != cx[1] || c[16] != cx[2] || c[17] != cx[3] ) continue;
      if( found >= 0 ) return -1;
      found = i + 6;
      if( wasOurs ) *wasOurs = ( c[6] == 0xEB );
    }
    return found;
  }
  inline unsigned char SkipByte( bool mapOpen ) {
    return mapOpen ? (unsigned char)0xEB : (unsigned char)0x74;
  }
  enum { kMaxCopies = 8 };
  struct TEventStores {
    int n;
    int x[kMaxCopies];
    int y[kMaxCopies];
    int ptrOff[kMaxCopies];
  };
  inline unsigned int Rd32( const unsigned char* p ) {
    return (unsigned int)p[0] | ( (unsigned int)p[1] << 8 )
         | ( (unsigned int)p[2] << 16 ) | ( (unsigned int)p[3] << 24 );
  }
  inline bool FindEventStores( const unsigned char* img, int size, unsigned int imgVA,
                               unsigned int counterX, TEventStores* out, bool* wasOurs ) {
    if( wasOurs ) *wasOurs = false;
    if( out ) out->n = 0;
    if( !img || size < 32 || !counterX || !out ) return false;
    int ptrs[kMaxCopies], nx[kMaxCopies], ny[kMaxCopies], xs[kMaxCopies], ys[kMaxCopies];
    int np = 0, nOurs = 0, nAll = 0;
    for( int i = 0; i + 24 <= size; i++ ) {
      const unsigned char* c = img + i;
      if( c[0] != 0xE8 || c[5] != 0x8B ) continue;
      const unsigned char m = c[6];
      if( ( m & 0xC7 ) != 0x05 ) continue;
      const int reg = ( m >> 3 ) & 7;
      if( reg == 0 || reg == 4 || reg == 5 ) continue;
      const unsigned int ptr = Rd32( c + 7 );
      if( ptr < imgVA || ptr - imgVA > (unsigned int)( size - 4 ) ) continue;
      const int po = (int)( ptr - imgVA );
      if( Rd32( img + po ) != counterX ) continue;
      const unsigned char op = c[11];
      if( op != 0x89 && op != 0x01 ) continue;
      int len = 0;
      bool isX = false;
      if( c[12] == (unsigned char)reg ) { len = 2; isX = true; }
      else if( c[12] == (unsigned char)( 0x40 | reg ) && c[13] == 0x04 ) len = 3;
      else
        continue;
      if( c[11 + len] != 0xA3 || c[16 + len] != 0xE9 ) continue;
      int k = 0;
      while( k < np && ptrs[k] != po )
        k++;
      if( k == np ) {
        if( np == kMaxCopies ) return false;
        ptrs[np] = po; nx[np] = ny[np] = 0; xs[np] = ys[np] = -1;
        np++;
      }
      if( isX ) { nx[k]++; xs[k] = i + 11; }
      else      { ny[k]++; ys[k] = i + 11; }
      nAll++;
      if( op == 0x01 ) nOurs++;
    }
    if( np == 0 || ( nOurs != 0 && nOurs != nAll ) ) return false;
    for( int k = 0; k < np; k++ )
      if( nx[k] != 1 || ny[k] != 1 ) return false;
    for( int k = 0; k < np; k++ ) {
      out->x[k] = xs[k];
      out->y[k] = ys[k];
      out->ptrOff[k] = ptrs[k];
    }
    out->n = np;
    if( wasOurs ) *wasOurs = ( nOurs != 0 );
    return true;
  }
  inline unsigned char StoreByte( bool mapOpen ) {
    return mapOpen ? (unsigned char)0x01 : (unsigned char)0x89;
  }
  inline bool EventStoresReady( const unsigned char* img, const TEventStores& e, bool mapOpen ) {
    if( !img || e.n <= 0 || e.n > kMaxCopies ) return false;
    const unsigned char now = StoreByte( !mapOpen );
    for( int k = 0; k < e.n; k++ )
      if( img[e.x[k]] != now || img[e.y[k]] != now ) return false;
    return true;
  }
}
