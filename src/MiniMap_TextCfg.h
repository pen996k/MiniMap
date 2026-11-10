#pragma once
namespace MiniMapCommon {
  template<class T>
  inline T* MmAlloc( size_t count = 1 ) {
    static_assert( std::is_trivially_default_constructible_v<T> && std::is_trivially_destructible_v<T>, "MmAlloc requires trivial POD" );
    if( count == 0 ) count = 1;
    if( count > ( (size_t)-1 ) / sizeof( T ) ) return 0;
    const size_t bytes = count * sizeof( T );
    if( bytes >= ( 4u << 20 ) ) {
      MEMORYSTATUSEX ms = { sizeof( ms ) };
      if( GlobalMemoryStatusEx( &ms ) && ms.ullAvailVirtual < (unsigned __int64)bytes + ( 256ull << 20 ) ) return 0;
    }
    return static_cast<T*>( ::HeapAlloc( ::GetProcessHeap(), 0, bytes ) );
  }
  inline void MmFree( const void* p ) {
    if( p ) ::HeapFree( ::GetProcessHeap(), 0, const_cast<void*>( p ) );
  }
}
namespace MiniMapCfg {
  enum {
    KEY_LEN      = 64,
    VAL_LEN      = 160,
    SEC_LEN      = 24,
    MAX_SECTIONS = 48,
    MAX_ENTRIES  = 16384,
    LINE_MAX     = 511
  };
  struct TItem {
    char  key[KEY_LEN];
    char  val[VAL_LEN];
    short sec;
    short src;
    int   line;
    int   dead;
  };
  typedef void ( *TMsgFn )( void* ctx, int src, int line, const char* msg );
  struct TCfg {
    TItem* e;
    int    n, cap;
    char   sec[MAX_SECTIONS][SEC_LEN];
    int    nsec;
    int    errors, notes;
    bool   full;
    int*   hash;
    int    hcap;
    const char* known;
  };
  inline void Init( TCfg& C, const char* known = 0 ) {
    memset( &C, 0, sizeof( C ) );
    C.known = known;
  }
  inline void Free( TCfg& C ) {
    const char* k = C.known;
    MiniMapCommon::MmFree( C.e );
    MiniMapCommon::MmFree( C.hash );
    memset( &C, 0, sizeof( C ) );
    C.known = k;
  }
  inline void Copy( char* dst, int cap, const char* src ) {
    if( dst && cap > 0 ) strncpy_s( dst, (size_t)cap, src ? src : "", _TRUNCATE );
  }
  inline void CopyN( char* dst, int cap, const char* src, int n ) {
    if( !dst || cap <= 0 ) return;
    if( !src || n <= 0 ) { dst[0] = 0; return; }
    strncpy_s( dst, (size_t)cap, src, (size_t)( ( n < cap - 1 ) ? n : ( cap - 1 ) ) );
  }
  inline void Cat( char* dst, int cap, const char* src ) {
    if( dst && cap > 0 && src ) strncat_s( dst, (size_t)cap, src, _TRUNCATE );
  }
  inline char Up( char c ) { return (char)toupper( (unsigned char)c ); }
  inline char Low( char c ) { return (char)tolower( (unsigned char)c ); }
  inline bool IsSpace( char c ) { return c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f'; }
  inline void TrimSpan( const char*& s, int& n ) {
    while( n > 0 && IsSpace( *s ) ) { s++; n--; }
    while( n > 0 && IsSpace( s[n - 1] ) ) n--;
  }
  inline int TrimStr( char*& s ) {
    while( IsSpace( *s ) ) s++;
    int n = (int)strlen( s );
    while( n > 0 && IsSpace( s[n - 1] ) ) s[--n] = 0;
    return n;
  }
  inline bool EqNoCase( const char* a, const char* b ) { return a && b && _stricmp( a, b ) == 0; }
  inline bool StartsNoCase( const char* s, const char* p ) { return s && p && _strnicmp( s, p, strlen( p ) ) == 0; }
  inline bool EndsNoCase( const char* s, int n, const char* suf ) {
    const int m = (int)strlen( suf );
    return n >= m && StartsNoCase( s + n - m, suf );
  }
  inline bool HasNoCase( const char* s, const char* sub ) {
    if( !s || !sub || !*sub ) return false;
    const size_t n = strlen( sub );
    for( ; *s; s++ ) if( _strnicmp( s, sub, n ) == 0 ) return true;
    return false;
  }
  inline const char* BaseName( const char* path ) {
    if( !path ) return "";
    const char* b1 = strrchr( path, '\\' );
    const char* b2 = strrchr( path, '/' );
    const char* b  = ( b1 && b2 ) ? ( b1 > b2 ? b1 : b2 ) : ( b1 ? b1 : b2 );
    return b ? ( b + 1 ) : path;
  }
  inline int HexNib( char c ) {
    return ( c >= '0' && c <= '9' ) ? ( c - '0' )
         : ( c >= 'a' && c <= 'f' ) ? ( c - 'a' + 10 )
         : ( c >= 'A' && c <= 'F' ) ? ( c - 'A' + 10 ) : -1;
  }
  inline int DecodeKeyHex4( const char* p ) {
    if( !p ) return -1;
    const int d0 = HexNib( p[0] ), d1 = HexNib( p[1] ), d2 = HexNib( p[2] ), d3 = HexNib( p[3] );
    return ( ( d0 | d1 | d2 | d3 ) < 0 ) ? -1 : ( ( d0 << 4 ) | d1 | ( d2 << 12 ) | ( d3 << 8 ) );
  }
  inline unsigned char* ReadFile( const char* path, int& len, int maxLen ) {
    len = 0;
    HANDLE h = CreateFileA( path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) return 0;
    const DWORD sz = GetFileSize( h, 0 );
    if( sz == INVALID_FILE_SIZE || sz > (DWORD)maxLen ) {
      CloseHandle( h );
      len = ( sz == INVALID_FILE_SIZE ) ? -1 : -2;
      return 0;
    }
    unsigned char* buf = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( sz + 1 ) );
    DWORD rd = 0;
    const BOOL ok = buf && ::ReadFile( h, buf, sz, &rd, 0 ) && rd == sz;
    CloseHandle( h );
    if( !ok ) { MiniMapCommon::MmFree( buf ); return 0; }
    buf[sz] = 0;
    len = (int)sz;
    return buf;
  }
  inline unsigned int Fnv( const char* s, unsigned int h = 2166136261u ) {
    for( ; s && *s; s++ ) { h ^= (unsigned char)Up( *s ); h *= 16777619u; }
    return h;
  }
  inline bool KeyChar( char c ) {
    return ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) || ( c >= '0' && c <= '9' )
        || c == '_' || c == '.' || c == '-' || c == '$' || c == '@' || c == ':' || c == '*';
  }
  inline bool SecChar( char c ) {
    return ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || ( c >= '0' && c <= '9' ) || c == '_' || c == '-';
  }
  inline void Note( TCfg& C, TMsgFn msg, void* ctx, int src, int line, const char* m, bool err ) {
    if( err ) C.errors++; else C.notes++;
    if( msg ) msg( ctx, src, line, m );
  }
  inline bool SecKnown( const char* known, const char* name ) {
    if( !known ) return true;
    const int n = (int)strlen( name );
    for( const char* p = known; *p; ) {
      while( *p == ' ' ) p++;
      const char* q = p;
      while( *q && *q != ' ' ) q++;
      if( q - p == n && n > 0 && _strnicmp( p, name, (size_t)n ) == 0 ) return true;
      p = q;
    }
    return false;
  }
  inline int SecFind( const TCfg& C, const char* name ) {
    for( int i = 0; i < C.nsec; i++ )
      if( EqNoCase( C.sec[i], name ) ) return i;
    return -1;
  }
  inline unsigned int HashOf( int sec, const char* key ) {
    return Fnv( key, 2166136261u ^ (unsigned int)( sec * 0x9E3779B1u ) );
  }
  inline bool HashGrow( TCfg& C, int want ) {
    int cap = 64;
    while( cap < want * 2 ) cap *= 2;
    if( cap <= C.hcap ) return true;
    int* h = MiniMapCommon::MmAlloc<int>( (size_t)( cap ) );
    if( !h ) return false;
    memset( h, 0, sizeof( int ) * cap );
    for( int i = 0; i < C.n; i++ ) {
      if( C.e[i].dead ) continue;
      unsigned int k = HashOf( C.e[i].sec, C.e[i].key ) & ( cap - 1 );
      while( h[k] ) k = ( k + 1 ) & ( cap - 1 );
      h[k] = i + 1;
    }
    MiniMapCommon::MmFree( C.hash );
    C.hash = h;
    C.hcap = cap;
    return true;
  }
  inline int HashFind( const TCfg& C, int sec, const char* key ) {
    if( !C.hash || C.hcap <= 0 || sec < 0 ) return -1;
    unsigned int k = HashOf( sec, key ) & ( C.hcap - 1 );
    for( int probe = 0; probe < C.hcap && C.hash[k]; probe++ ) {
      const int i = C.hash[k] - 1;
      if( C.e[i].sec == sec && EqNoCase( C.e[i].key, key ) ) return i;
      k = ( k + 1 ) & ( C.hcap - 1 );
    }
    return -1;
  }
  inline void HashPut( TCfg& C, int i ) {
    unsigned int k = HashOf( C.e[i].sec, C.e[i].key ) & ( C.hcap - 1 );
    while( C.hash[k] ) {
      const int j = C.hash[k] - 1;
      if( C.e[j].sec == C.e[i].sec && EqNoCase( C.e[j].key, C.e[i].key ) ) {
        C.hash[k] = i + 1;
        return;
      }
      k = ( k + 1 ) & ( C.hcap - 1 );
    }
    C.hash[k] = i + 1;
  }
  inline int Add( TCfg& C, int sec, const char* key, const char* val, int src, int line,
                  TMsgFn msg, void* ctx ) {
    if( C.n >= MAX_ENTRIES ) {
      if( !C.full )
        Note( C, msg, ctx, src, line, "too many entries (limit 16384) - the rest is ignored", true );
      else
        C.errors++;
      C.full = true;
      return -1;
    }
    if( C.n >= C.cap ) {
      int nc = C.cap ? C.cap * 2 : 256;
      if( nc > MAX_ENTRIES ) nc = MAX_ENTRIES;
      TItem* ne = MiniMapCommon::MmAlloc<TItem>( (size_t)( nc ) );
      if( !ne ) {
        Note( C, msg, ctx, src, line, "out of memory - the rest is ignored", true );
        C.full = true;
        return -1;
      }
      if( C.n ) memcpy( ne, C.e, sizeof( TItem ) * C.n );
      MiniMapCommon::MmFree( C.e );
      C.e   = ne;
      C.cap = nc;
    }
    if( !HashGrow( C, C.n + 1 ) ) {
      Note( C, msg, ctx, src, line, "out of memory - the rest is ignored", true );
      C.full = true;
      return -1;
    }
    TItem& t = C.e[C.n];
    memset( &t, 0, sizeof( t ) );
    Copy( t.key, KEY_LEN, key );
    _strupr_s( t.key, KEY_LEN );
    Copy( t.val, VAL_LEN, val );
    t.sec  = (short)sec;
    t.src  = (short)src;
    t.line = line;
    const int old = HashFind( C, sec, t.key );
    if( old >= 0 ) C.e[old].dead = 1;
    HashPut( C, C.n );
    return C.n++;
  }
  inline int Parse( TCfg& C, const char* text, int len, int src, TMsgFn msg, void* ctx ) {
    if( !text || len <= 0 ) return 0;
    if( len >= 3 && (unsigned char)text[0] == 0xEF && (unsigned char)text[1] == 0xBB && (unsigned char)text[2] == 0xBF ) {
      text += 3;
      len  -= 3;
    }
    int  section = -1;
    bool skip    = false;
    bool warnedNoSec = false;
    int  added = 0, lineNo = 0, p = 0;
    char buf[LINE_MAX + 1];
    while( p < len ) {
      int e = p;
      while( e < len && text[e] != '\n' && text[e] != '\r' ) e++;
      lineNo++;
      int n = e - p;
      const bool tooLong = n > LINE_MAX;
      if( tooLong ) n = LINE_MAX;
      memcpy( buf, text + p, n );
      buf[n] = 0;
      p = e + 1;
      if( e < len && text[e] == '\r' && e + 1 < len && text[e + 1] == '\n' )
        p++;
      if( tooLong ) {
        Note( C, msg, ctx, src, lineNo, "line is longer than 511 characters - skipped", true );
        continue;
      }
      for( int i = 0; buf[i]; i++ ) {
        if( buf[i] == 0 ) break;
        if( buf[i] == '#' || buf[i] == ';' || ( buf[i] == '/' && buf[i + 1] == '/' ) ) {
          buf[i] = 0;
          break;
        }
      }
      char* s = buf;
      const int sl = TrimStr( s );
      if( !sl ) continue;
      if( s[0] == '[' ) {
        skip = true;
        if( s[sl - 1] != ']' ) {
          Note( C, msg, ctx, src, lineNo, "section name must end with ']'", true );
          continue;
        }
        s[sl - 1] = 0;
        char* nm = s + 1;
        const int nl = TrimStr( nm );
        bool ok = nl > 0 && nl < SEC_LEN;
        for( int i = 0; ok && nm[i]; i++ ) ok = SecChar( nm[i] );
        if( !ok ) {
          Note( C, msg, ctx, src, lineNo, "bad section name (letters, digits, _ and -, max 23)", true );
          continue;
        }
        if( !SecKnown( C.known, nm ) ) {
          Note( C, msg, ctx, src, lineNo, "unknown section - lines below it are ignored (see examples)", true );
          continue;
        }
        int si = SecFind( C, nm );
        if( si < 0 ) {
          if( C.nsec >= MAX_SECTIONS ) {
            Note( C, msg, ctx, src, lineNo, "too many sections (limit 48)", true );
            continue;
          }
          si = C.nsec++;
          for( int i = 0; i <= nl; i++ ) C.sec[si][i] = Low( nm[i] );
        }
        section = si;
        skip    = false;
        continue;
      }
      if( skip ) continue;
      if( section < 0 ) {
        if( !warnedNoSec )
          Note( C, msg, ctx, src, lineNo, "line before the first [section] - ignored", true );
        else
          C.errors++;
        warnedNoSec = true;
        continue;
      }
      char* eq = strchr( s, '=' );
      char* kEnd = eq ? eq : s + sl;
      while( kEnd > s && IsSpace( kEnd[-1] ) ) kEnd--;
      const int kl = (int)( kEnd - s );
      if( kl <= 0 ) {
        Note( C, msg, ctx, src, lineNo, "missing name before '='", true );
        continue;
      }
      if( kl >= KEY_LEN ) {
        Note( C, msg, ctx, src, lineNo, "name is longer than 63 characters", true );
        continue;
      }
      bool kok = true;
      for( int i = 0; i < kl && kok; i++ ) kok = KeyChar( s[i] );
      if( !kok ) {
        Note( C, msg, ctx, src, lineNo, "name has a character that is not allowed (use letters, digits, _ . - $ @ : *)", true );
        continue;
      }
      char key[KEY_LEN];
      CopyN( key, KEY_LEN, s, kl );
      const char* v = "yes";
      if( eq ) {
        v = eq + 1;
        while( IsSpace( *v ) ) v++;
        if( strlen( v ) >= VAL_LEN ) {
          Note( C, msg, ctx, src, lineNo, "value is longer than 159 characters", true );
          continue;
        }
      }
      if( Add( C, section, key, v, src, lineNo, msg, ctx ) >= 0 )
        added++;
    }
    return added;
  }
  inline const TItem* Find( const TCfg& C, const char* section, const char* key ) {
    if( !key || !*key ) return 0;
    const int i = HashFind( C, SecFind( C, section ), key );
    return i >= 0 ? &C.e[i] : 0;
  }
  inline int Next( const TCfg& C, int sec, int from ) {
    if( sec < 0 ) return -1;
    for( int i = from + 1; i < C.n; i++ )
      if( !C.e[i].dead && C.e[i].sec == sec ) return i;
    return -1;
  }
  inline int Count( const TCfg& C, const char* section ) {
    const int s = SecFind( C, section );
    int n = 0;
    for( int i = Next( C, s, -1 ); i >= 0; i = Next( C, s, i ) ) n++;
    return n;
  }
  inline int Bool( const char* v ) {
    if( !v ) return -1;
    static const char* const y[] = { "yes", "on", "true", "1", "show", "y" };
    static const char* const n[] = { "no", "off", "false", "0", "hide", "n" };
    for( int i = 0; i < 6; i++ ) {
      if( EqNoCase( v, y[i] ) ) return 1;
      if( EqNoCase( v, n[i] ) ) return 0;
    }
    return -1;
  }
  inline bool Int( const char* v, int lo, int hi, int& out ) {
    if( !v ) return false;
    while( IsSpace( *v ) ) v++;
    if( !*v ) return false;
    char* end = 0;
    const long long r = strtoll( v, &end, 10 );
    if( !end || end == v ) return false;
    while( IsSpace( *end ) ) end++;
    if( *end || r < lo || r > hi ) return false;
    out = (int)r;
    return true;
  }
  inline bool Color( const char* v, unsigned char out[4] ) {
    if( !v ) return false;
    int c[4] = { 0, 0, 0, 255 }, k = 0;
    const char* p = v;
    while( *p ) {
      while( IsSpace( *p ) || *p == ',' ) p++;
      if( !*p ) break;
      if( k >= 4 || *p < '0' || *p > '9' ) return false;
      int r = 0, d = 0;
      while( *p >= '0' && *p <= '9' ) { r = r * 10 + ( *p - '0' ); if( ++d > 3 ) return false; p++; }
      if( r > 255 ) return false;
      if( *p && !IsSpace( *p ) && *p != ',' ) return false;
      c[k++] = r;
    }
    if( k != 3 && k != 4 ) return false;
    for( int i = 0; i < 4; i++ ) out[i] = (unsigned char)c[i];
    return true;
  }
  inline int Word( const char* v, int* pos, char* out, int cap ) {
    if( !v || !pos || !out || cap <= 0 ) return 0;
    int i = *pos;
    while( v[i] && ( IsSpace( v[i] ) || v[i] == ',' ) ) i++;
    int n = 0;
    while( v[i] && !IsSpace( v[i] ) && v[i] != ',' ) {
      if( n < cap - 1 ) out[n] = v[i];
      n++;
      i++;
    }
    out[n < cap ? n : cap - 1] = 0;
    *pos = i;
    return n;
  }
  inline void ModFolderName( const char* raw, char* out, int cap ) {
    if( !out || cap <= 0 ) return;
    out[0] = 0;
    if( !raw ) return;
    const char* b = raw;
    for( const char* p = raw; *p; p++ )
      if( *p == '\\' || *p == '/' || *p == ':' ) b = p + 1;
    int n = (int)strlen( b );
    if( EndsNoCase( b, n, ".ini" ) )
      n -= 4;
    while( n > 0 && ( IsSpace( *b ) || *b == '"' ) ) { b++; n--; }
    while( n > 0 && ( IsSpace( b[n - 1] ) || b[n - 1] == '"' ) ) n--;
    int k = 0;
    for( int i = 0; i < n && k < cap - 1; i++ ) {
      const char c = b[i];
      const bool ok = ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) || ( c >= '0' && c <= '9' )
                   || c == '_' || c == '-' || c == ' ' || ( c == '.' && k > 0 );
      out[k++] = ok ? c : '_';
    }
    while( k > 0 && ( out[k - 1] == '.' || out[k - 1] == ' ' ) ) k--;
    out[k] = 0;
    bool any = false;
    for( int i = 0; i < k; i++ ) if( out[i] != '_' ) any = true;
    if( !any ) out[0] = 0;
  }
  enum { PT_WORLD_LEN = 64, PT_NAME_LEN = 96 };
  struct TPoint {
    char  world[PT_WORLD_LEN];
    float x, z;
    int   icon;
    char  name[PT_NAME_LEN];
  };
  inline bool ParseFloat( const char* s, int n, float& out ) {
    char buf[32];
    TrimSpan( s, n );
    if( n <= 0 || n > 24 ) return false;
    CopyN( buf, sizeof( buf ), s, n );
    char* end = 0;
    const double v = strtod( buf, &end );
    if( !end || end != buf + n || !( v >= -1.0e7 && v <= 1.0e7 ) ) return false;
    out = (float)v;
    return true;
  }
  static const char* const kPointIcon[10] = {
    "target", "treasure", "warning", "star", "point", "fight", "house", "cave", "herb", "quest"
  };
  inline bool ParsePoint( const char* v, TPoint& p, const char** err ) {
    static const char* const kFmt = "write: WORLD, x, z, icon, Name  (e.g. NEWWORLD, 1200, -3400, quest, Old tower)";
    if( err ) *err = kFmt;
    if( !v ) return false;
    const char* f[4];
    int fl[4];
    const char* q = v;
    const char* name = 0;
    for( int k = 0; k < 4; k++ ) {
      f[k] = q;
      while( *q && *q != ',' ) q++;
      fl[k] = (int)( q - f[k] );
      if( *q != ',' ) {
        if( k < 3 ) return false;
        break;
      }
      q++;
      if( k == 3 ) name = q;
    }
    const char* w = f[0];
    int wn = fl[0];
    TrimSpan( w, wn );
    if( EndsNoCase( w, wn, ".zen" ) )
      wn -= 4;
    if( wn <= 0 || wn >= PT_WORLD_LEN ) { if( err ) *err = "world name is missing or too long"; return false; }
    for( int i = 0; i < wn; i++ ) {
      const char c = w[i];
      if( !( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) || ( c >= '0' && c <= '9' ) || c == '_' || c == '-' ) ) {
        if( err ) *err = "world name: letters, digits, _ and - only (file name of the .zen without .zen)";
        return false;
      }
      p.world[i] = Up( c );
    }
    p.world[wn] = 0;
    if( !ParseFloat( f[1], fl[1], p.x ) || !ParseFloat( f[2], fl[2], p.z ) ) {
      if( err ) *err = "x and z must be numbers (world centimeters, e.g. -3400 or 1200.5)";
      return false;
    }
    const char* ic = f[3];
    int icn = fl[3];
    TrimSpan( ic, icn );
    p.icon = -1;
    for( int k = 0; k < 10 && p.icon < 0; k++ )
      if( (int)strlen( kPointIcon[k] ) == icn && _strnicmp( ic, kPointIcon[k], (size_t)icn ) == 0 ) p.icon = k;
    if( p.icon < 0 ) {
      if( err ) *err = "icon: target treasure warning star point fight house cave herb quest";
      return false;
    }
    p.name[0] = 0;
    if( name ) {
      int nn = (int)strlen( name );
      TrimSpan( name, nn );
      if( nn >= PT_NAME_LEN ) nn = PT_NAME_LEN - 1;
      CopyN( p.name, PT_NAME_LEN, name, nn );
    }
    if( err ) *err = 0;
    return true;
  }
  inline int PatPrefix( const char* pat ) {
    if( !pat ) return -1;
    const int n = (int)strlen( pat );
    for( int i = 0; i < n - 1; i++ )
      if( pat[i] == '*' ) return -1;
    if( n == 0 ) return -1;
    if( pat[n - 1] != '*' ) return 0;
    return ( n - 1 >= 3 ) ? n - 1 : -1;
  }
  inline bool PatMatch( const char* pat, const char* name ) {
    if( !pat || !name ) return false;
    const int pre = PatPrefix( pat );
    if( pre < 0 ) return false;
    return pre == 0 ? EqNoCase( pat, name ) : ( _strnicmp( name, pat, (size_t)pre ) == 0 );
  }
  inline bool GameParam( const char* cmd, char* out, int cap ) {
    if( !out || cap <= 0 ) return false;
    out[0] = 0;
    if( !cmd ) return false;
    for( const char* p = cmd; *p; p++ ) {
      if( ( *p != '-' && *p != '/' ) || ( p > cmd && !IsSpace( p[-1] ) && p[-1] != '"' ) ) continue;
      if( !StartsNoCase( p + 1, "game:" ) ) continue;
      const char* v = p + 6;
      int n = 0;
      if( *v == '"' ) {
        v++;
        while( v[n] && v[n] != '"' ) n++;
      }
      else
        while( v[n] && !IsSpace( v[n] ) && v[n] != '"' ) n++;
      CopyN( out, cap, v, n );
      return out[0] != 0;
    }
    return false;
  }
  enum { ICOV_TINT = 1, ICOV_NOOUTLINE = 2, ICOV_COLOR = 4 };
  inline bool ParseIconVal( const char* v, char* file, int cap, unsigned& flags, const char** err,
                            unsigned char* rgb = 0 ) {
    flags = 0;
    if( err ) *err = 0;
    if( file && cap > 0 ) file[0] = 0;
    if( !v || !file || cap <= 1 ) { if( err ) *err = "write: file.png [, tint] [, nooutline]"; return false; }
    int pos = 0;
    char w[VAL_LEN];
    if( !Word( v, &pos, w, (int)sizeof( w ) ) || !w[0] ) { if( err ) *err = "no file name (write: folder\\icon.png [, tint])"; return false; }
    const int n = (int)strlen( w );
    if( n >= cap ) { if( err ) *err = "file name is too long"; return false; }
    if( strstr( w, ".." ) || strchr( w, ':' ) || w[0] == '\\' || w[0] == '/' ) {
      if( err ) *err = "file must be inside MiniMap\\Icons (no .., no drive, no leading \\)";
      return false;
    }
    if( n < 5 || !( EqNoCase( w + n - 4, ".png" ) || EqNoCase( w + n - 4, ".tga" ) ) ) {
      if( err ) *err = "only .png and .tga files";
      return false;
    }
    Copy( file, cap, w );
    for( char* p = file; *p; p++ ) if( *p == '/' ) *p = '\\';
    while( Word( v, &pos, w, (int)sizeof( w ) ) ) {
      if( EqNoCase( w, "tint" ) )           flags |= ICOV_TINT;
      else if( EqNoCase( w, "nooutline" ) ) flags |= ICOV_NOOUTLINE;
      else if( EqNoCase( w, "color" ) || EqNoCase( w, "colour" ) ) {
        int c[3];
        bool ok = true;
        for( int k = 0; k < 3 && ok; k++ ) {
          char nb[8];
          ok = Word( v, &pos, nb, (int)sizeof( nb ) ) > 0 && Int( nb, 0, 255, c[k] );
        }
        if( !ok ) { if( err ) *err = "write the color as: color R G B (0-255)"; file[0] = 0; flags = 0; return false; }
        flags |= ICOV_COLOR;
        if( rgb ) { rgb[0] = (unsigned char)c[0]; rgb[1] = (unsigned char)c[1]; rgb[2] = (unsigned char)c[2]; }
      }
      else { if( err ) *err = "unknown word after the file (use tint, nooutline, color R G B)"; file[0] = 0; flags = 0; return false; }
    }
    return true;
  }
  enum { LS_CMT = 48, LS_MAX_DEF = 20000 };
  struct TListEnt {
    char key[KEY_LEN];
    char val[VAL_LEN];
    char cmt[LS_CMT];
    int  cnt;
    int  off;
    int  tag;
  };
  struct TListSet {
    TListEnt* e;
    int       n, cap, max;
    int*      h;
    int       hcap;
    int       over;
    int       bad;
  };
  inline void ListInit( TListSet& S, int maxN = LS_MAX_DEF ) {
    S.e = 0; S.n = S.cap = 0; S.max = maxN > 0 ? maxN : 1; S.h = 0; S.hcap = 0; S.over = 0; S.bad = 0;
  }
  inline void ListFree( TListSet& S ) {
    MiniMapCommon::MmFree( S.e ); MiniMapCommon::MmFree( S.h );
    S.e = 0; S.h = 0; S.n = S.cap = S.hcap = 0;
  }
  inline void ListClean( const char* in, char* out, int cap ) {
    if( !out || cap <= 0 ) return;
    out[0] = 0;
    if( !in ) return;
    int n = 0;
    for( int i = 0; in[i] && n < cap - 1; i++ ) {
      char c = in[i];
      if( c == '/' && in[i + 1] == '/' ) {
        out[n++] = ' ';
        if( n < cap - 1 ) out[n++] = ' ';
        i++;
        continue;
      }
      if( (unsigned char)c < 32 || c == '#' || c == ';' ) c = ' ';
      out[n++] = c;
    }
    out[n] = 0;
    int a = 0;
    while( out[a] == ' ' ) a++;
    while( n > a && out[n - 1] == ' ' ) out[--n] = 0;
    if( a > 0 ) memmove( out, out + a, n - a + 1 );
  }
  inline bool ListKeyOk( const char* k ) {
    if( !k || !k[0] ) return false;
    int i = 0;
    for( ; k[i]; i++ )
      if( i >= KEY_LEN - 1 || !KeyChar( k[i] ) || k[i] == '*' ) return false;
    return true;
  }
  inline TListEnt* ListAdd( TListSet& S, const char* key, const char* val, const char* cmt = 0 ) {
    if( !ListKeyOk( key ) ) { S.bad++; return 0; }
    if( !S.h ) {
      int hc = 64;
      while( hc < S.max * 2 ) hc *= 2;
      S.h = MiniMapCommon::MmAlloc<int>( (size_t)( hc ) );
      if( !S.h ) return 0;
      memset( S.h, 0, sizeof( int ) * hc );
      S.hcap = hc;
    }
    unsigned int k = Fnv( key ) & ( S.hcap - 1 );
    for( int probe = 0; probe < S.hcap && S.h[k]; probe++ ) {
      TListEnt& x = S.e[S.h[k] - 1];
      if( EqNoCase( x.key, key ) ) { x.cnt++; return &x; }
      k = ( k + 1 ) & ( S.hcap - 1 );
    }
    if( S.n >= S.max ) { S.over++; return 0; }
    if( S.n >= S.cap ) {
      int nc = S.cap ? S.cap * 2 : 256;
      if( nc > S.max ) nc = S.max;
      TListEnt* ne = MiniMapCommon::MmAlloc<TListEnt>( (size_t)( nc ) );
      if( !ne ) return 0;
      if( S.n ) memcpy( ne, S.e, sizeof( TListEnt ) * S.n );
      MiniMapCommon::MmFree( S.e );
      S.e = ne; S.cap = nc;
    }
    TListEnt& x = S.e[S.n];
    Copy( x.key, KEY_LEN, key );
    ListClean( val, x.val, VAL_LEN );
    ListClean( cmt, x.cmt, LS_CMT );
    x.cnt = 1;
    x.off = 0;
    x.tag = 0;
    S.h[k] = ++S.n;
    return &x;
  }
  inline TListEnt* ListFind( TListSet& S, const char* key ) {
    if( !S.h || !key || !key[0] ) return 0;
    unsigned int k = Fnv( key ) & ( S.hcap - 1 );
    for( int probe = 0; probe < S.hcap && S.h[k]; probe++ ) {
      TListEnt& x = S.e[S.h[k] - 1];
      if( EqNoCase( x.key, key ) ) return &x;
      k = ( k + 1 ) & ( S.hcap - 1 );
    }
    return 0;
  }
  inline int ListCmpKey( const void* a, const void* b ) {
    const char* x = ( (const TListEnt*)a )->key;
    const char* y = ( (const TListEnt*)b )->key;
    for( ; *x && Up( *x ) == Up( *y ); x++, y++ ) {}
    return (int)(unsigned char)Up( *x ) - (int)(unsigned char)Up( *y );
  }
  inline void ListSort( TListSet& S ) {
    if( S.n > 1 ) qsort( S.e, S.n, sizeof( TListEnt ), ListCmpKey );
    MiniMapCommon::MmFree( S.h ); S.h = 0; S.hcap = 0;
  }
  struct TTextBuf { char* p; int n, cap; bool bad; };
  inline void BufInit( TTextBuf& B ) { B.p = 0; B.n = B.cap = 0; B.bad = false; }
  inline void BufFree( TTextBuf& B ) { MiniMapCommon::MmFree( B.p ); BufInit( B ); }
  inline void BufPut( TTextBuf& B, const char* s, int len = -1 ) {
    if( B.bad || !s ) return;
    if( len < 0 ) len = (int)strlen( s );
    if( B.n + len + 1 > B.cap ) {
      int nc = B.cap ? B.cap : 4096;
      while( nc < B.n + len + 1 ) nc *= 2;
      char* np = MiniMapCommon::MmAlloc<char>( (size_t)( nc ) );
      if( !np ) { B.bad = true; return; }
      if( B.n ) memcpy( np, B.p, B.n );
      MiniMapCommon::MmFree( B.p );
      B.p = np; B.cap = nc;
    }
    memcpy( B.p + B.n, s, len );
    B.n += len;
    B.p[B.n] = 0;
  }
  inline void BufInt( TTextBuf& B, int v ) {
    char t[16];
    const int len = snprintf( t, sizeof( t ), "%d", v );
    if( len > 0 ) BufPut( B, t, len );
  }
  inline int ListWrite( const TListSet& S, TTextBuf& B, bool cntCmt ) {
    int skipped = 0;
    for( int i = 0; i < S.n; i++ ) {
      const TListEnt& x = S.e[i];
      if( !ListKeyOk( x.key ) || !x.val[0] ) { skipped++; continue; }
      const int start = B.n;
      if( x.off ) BufPut( B, "# " );
      BufPut( B, x.key );
      BufPut( B, " = " );
      BufPut( B, x.val );
      if( x.cmt[0] || ( cntCmt && x.cnt > 1 ) ) {
        int col = B.n - start;
        while( col < 44 ) { BufPut( B, " ", 1 ); col++; }
        BufPut( B, "  # " );
        if( x.cmt[0] ) BufPut( B, x.cmt );
        else { BufInt( B, x.cnt ); BufPut( B, "x" ); }
      }
      BufPut( B, "\r\n", 2 );
    }
    return skipped;
  }
}
