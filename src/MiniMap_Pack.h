#pragma once
namespace MiniMapCommon {
  enum {
    PK_ALGO_STORE       = 0,
    PK_ALGO_XPRESS      = 3,
    PK_ALGO_XPRESS_HUFF = 4,
    PK_ALGO_LZMS        = 5
  };
  const unsigned int PK_MAGIC      = 0x4B504D4Du;
  const unsigned int PK_VERSION    = 1u;
  const unsigned int PK_BLOCK      = 256u * 1024u;
  const unsigned int PK_BLOCK_MIN  = 4096u;
  const unsigned int PK_BLOCK_MAX  = 8u * 1024u * 1024u;
  const unsigned int PK_HEAD_BYTES = 20u;
  const unsigned int PK_PATH_MAX   = 320u;
  typedef BOOL ( WINAPI *PkCreateFn )( DWORD, PVOID, PVOID* );
  typedef BOOL ( WINAPI *PkCodecFn )( PVOID, LPCVOID, SIZE_T, PVOID, SIZE_T, SIZE_T* );
  typedef BOOL ( WINAPI *PkCloseFn )( PVOID );
  struct TPkApi {
    int        state;
    HMODULE    dll;
    PkCreateFn createC;
    PkCreateFn createD;
    PkCodecFn  compress;
    PkCodecFn  decompress;
    PkCloseFn  closeC;
    PkCloseFn  closeD;
    int        algoState[6];
  };
  inline TPkApi g_pk = { 0, 0, 0, 0, 0, 0, 0, 0, { 0, 0, 0, 0, 0, 0 } };
  inline bool   g_pkOff = false;
  inline unsigned char* PkAlloc( unsigned n ) {
    return (unsigned char*)HeapAlloc( GetProcessHeap(), 0, n ? n : 1 );
  }
  inline void PkFree( void* p ) {
    if( p )
      HeapFree( GetProcessHeap(), 0, p );
  }
  inline unsigned int PkSum( const void* data, unsigned n ) {
    const unsigned char* p = (const unsigned char*)data;
    unsigned int h = 0x811C9DC5u ^ n;
    unsigned int i = 0;
    for( ; i + 4u <= n; i += 4u ) {
      h = ( h ^ *(const unsigned int*)( p + i ) ) * 0x01000193u;
      h ^= h >> 15;
    }
    for( ; i < n; i++ ) {
      h = ( h ^ (unsigned int)p[i] ) * 0x01000193u;
      h ^= h >> 15;
    }
    return h;
  }
  inline DWORD PkFlags( int algo ) {
    return (DWORD)algo | ( ( algo == PK_ALGO_XPRESS || algo == PK_ALGO_XPRESS_HUFF ) ? 0x20000000u : 0u );
  }
  inline bool PkAlgoValid( int algo ) {
    return algo == PK_ALGO_STORE || algo == PK_ALGO_XPRESS
        || algo == PK_ALGO_XPRESS_HUFF || algo == PK_ALGO_LZMS;
  }
  inline BOOL PkCall( PkCodecFn fn, PVOID h, LPCVOID in, SIZE_T inN,
                      PVOID out, SIZE_T outN, SIZE_T* used ) {
    BOOL r = FALSE;
    __try { r = fn( h, in, inN, out, outN, used ); }
    __except( EXCEPTION_EXECUTE_HANDLER ) { r = FALSE; }
    return r;
  }
  inline TPkApi& PkApi() {
    if( g_pk.state != 0 ) return g_pk;
    g_pk.state = -1;
    if( g_pkOff ) return g_pk;
    char path[MAX_PATH + 16];
    const UINT n = GetSystemDirectoryA( path, MAX_PATH );
    if( n == 0 || n >= MAX_PATH ) return g_pk;
    strncat_s( path, sizeof( path ), "\\cabinet.dll", _TRUNCATE );
    HMODULE d = LoadLibraryA( path );
    if( !d ) return g_pk;
    TPkApi t;
    t.dll        = d;
    t.createC    = (PkCreateFn)GetProcAddress( d, "CreateCompressor" );
    t.createD    = (PkCreateFn)GetProcAddress( d, "CreateDecompressor" );
    t.compress   = (PkCodecFn)GetProcAddress( d, "Compress" );
    t.decompress = (PkCodecFn)GetProcAddress( d, "Decompress" );
    t.closeC     = (PkCloseFn)GetProcAddress( d, "CloseCompressor" );
    t.closeD     = (PkCloseFn)GetProcAddress( d, "CloseDecompressor" );
    if( !t.createC || !t.createD || !t.compress || !t.decompress || !t.closeC || !t.closeD ) {
      FreeLibrary( d );
      return g_pk;
    }
    g_pk.dll        = d;
    g_pk.createC    = t.createC;
    g_pk.createD    = t.createD;
    g_pk.compress   = t.compress;
    g_pk.decompress = t.decompress;
    g_pk.closeC     = t.closeC;
    g_pk.closeD     = t.closeD;
    g_pk.state      = 1;
    return g_pk;
  }
  inline bool PkSelfTest( int algo ) {
    TPkApi& a = g_pk;
    PVOID c = 0, d = 0;
    bool ok = false;
    unsigned char src[4096], mid[8192], dst[4096];
    for( int i = 0; i < 4096; i++ )
      src[i] = (unsigned char)( ( i >> 3 ) & 0x1F );
    if( a.createC( PkFlags( algo ), 0, &c ) && c && a.createD( PkFlags( algo ), 0, &d ) && d ) {
      SIZE_T used = 0, back = 0;
      if( PkCall( a.compress, c, src, 4096, mid, 8192, &used ) && used > 0 && used < 4096
          && PkCall( a.decompress, d, mid, used, dst, 4096, &back ) && back == 4096 ) {
        ok = memcmp( src, dst, 4096 ) == 0;
      }
    }
    if( c ) a.closeC( c );
    if( d ) a.closeD( d );
    return ok;
  }
  inline bool PkReady( int algo ) {
    if( algo == PK_ALGO_STORE ) return true;
    if( !PkAlgoValid( algo ) ) return false;
    TPkApi& a = PkApi();
    if( a.state != 1 ) return false;
    if( a.algoState[algo] == 0 ) a.algoState[algo] = PkSelfTest( algo ) ? 1 : -1;
    return a.algoState[algo] == 1;
  }
  inline bool PkPutAll( HANDLE h, const void* p, unsigned n ) {
    if( n == 0 ) return true;
    DWORD w = 0;
    return WriteFile( h, p, n, &w, 0 ) != 0 && w == n;
  }
  inline bool PkGetAll( HANDLE h, void* p, unsigned n ) {
    if( n == 0 ) return true;
    DWORD r = 0;
    return ReadFile( h, p, n, &r, 0 ) != 0 && r == n;
  }
  inline volatile LONG g_pkRepackStop = 0;
  inline bool PkEncode( HANDLE h, const unsigned char* mem, HANDLE src,
                        unsigned size, int algo, bool verify ) {
    if( h == INVALID_HANDLE_VALUE || !h || ( !mem && !src && size ) ) return false;
    if( !PkReady( algo ) ) algo = PK_ALGO_STORE;
    unsigned int hd[5];
    hd[0] = PK_MAGIC;
    hd[1] = PK_VERSION | ( (unsigned int)algo << 8 );
    hd[2] = size;
    hd[3] = PK_BLOCK;
    hd[4] = PkSum( hd, 16 );
    if( !PkPutAll( h, hd, PK_HEAD_BYTES ) ) return false;
    PVOID          comp = 0, dec = 0;
    unsigned char* buf  = 0;
    unsigned char* pkSrc  = 0;
    unsigned char* pkChk = 0;
    const unsigned cap  = PK_BLOCK + PK_BLOCK / 4u + 1024u;
    bool ok = true;
    if( algo != PK_ALGO_STORE && size > 0 ) {
      buf = PkAlloc( 8u + cap );
      if( !buf || !g_pk.createC( PkFlags( algo ), 0, &comp ) || !comp ) comp = 0;
      if( comp && verify ) {
        pkChk = PkAlloc( PK_BLOCK );
        if( !pkChk || !g_pk.createD( PkFlags( algo ), 0, &dec ) || !dec ) ok = false;
      }
    }
    if( !mem && size > 0 ) {
      pkSrc = PkAlloc( PK_BLOCK );
      if( !pkSrc ) ok = false;
    }
    for( unsigned off = 0; off < size && ok; off += PK_BLOCK ) {
      const unsigned n = ( size - off < PK_BLOCK ) ? ( size - off ) : PK_BLOCK;
      const unsigned char* blk;
      if( mem ) {
        blk = mem + off;
      }
      else {
        if( g_pkRepackStop || !PkGetAll( src, pkSrc, n ) ) {
          ok = false;
          break;
        }
        blk = pkSrc;
      }
      const unsigned sum = PkSum( blk, n );
      bool stored = true;
      if( comp && buf ) {
        SIZE_T used = 0;
        if( PkCall( g_pk.compress, comp, blk, n, buf + 8, cap, &used )
            && used > 0 && used < n ) {
          if( verify && dec && pkChk ) {
            SIZE_T back = 0;
            const bool same = PkCall( g_pk.decompress, dec, buf + 8, used, pkChk, n, &back ) && back == n && memcmp( pkChk, blk, n ) == 0;
            if( !same ) {
              ok = false;
              break;
            }
          }
          *(unsigned int*)( buf )     = (unsigned int)used;
          *(unsigned int*)( buf + 4 ) = sum;
          ok = PkPutAll( h, buf, 8u + (unsigned)used );
          stored = false;
        }
      }
      if( stored && ok ) {
        unsigned int bh[2];
        bh[0] = n | 0x80000000u;
        bh[1] = sum;
        ok = PkPutAll( h, bh, 8 ) && PkPutAll( h, blk, n );
      }
    }
    if( comp ) g_pk.closeC( comp );
    if( dec )  g_pk.closeD( dec );
    PkFree( buf );
    PkFree( pkSrc );
    PkFree( pkChk );
    return ok;
  }
  inline bool PkWrite( HANDLE h, const void* data, unsigned size, int algo ) {
    return PkEncode( h, (const unsigned char*)data, 0, size, algo, false );
  }
  inline bool PkWriteAuto( HANDLE h, const void* data, unsigned size, int algo ) {
    if( !PkReady( algo ) || algo == PK_ALGO_STORE ) {
      if( h == INVALID_HANDLE_VALUE || !h || ( !data && size ) ) return false;
      return PkPutAll( h, data, size );
    }
    return PkWrite( h, data, size, algo );
  }
  inline bool PkRead( HANDLE h, void* out, unsigned size ) {
    if( h == INVALID_HANDLE_VALUE || !h || ( !out && size ) ) return false;
    unsigned int hd[5];
    if( !PkGetAll( h, hd, PK_HEAD_BYTES ) ) return false;
    if( hd[0] != PK_MAGIC || hd[4] != PkSum( hd, 16 ) ) return false;
    const unsigned int ver  = hd[1] & 0xFFu;
    const int          algo = (int)( ( hd[1] >> 8 ) & 0xFFu );
    if( ver != PK_VERSION || ( hd[1] >> 16 ) != 0 || !PkAlgoValid( algo ) ) return false;
    if( hd[2] != size ) return false;
    const unsigned block = hd[3];
    if( block < PK_BLOCK_MIN || block > PK_BLOCK_MAX ) return false;
    unsigned char* dst  = (unsigned char*)out;
    unsigned char* tmp  = 0;
    PVOID          dec  = 0;
    bool           ok   = true;
    for( unsigned off = 0; off < size && ok; off += block ) {
      const unsigned n = ( size - off < block ) ? ( size - off ) : block;
      unsigned int bh[2];
      if( !PkGetAll( h, bh, 8 ) ) {
        ok = false;
        break;
      }
      const bool     raw = ( bh[0] & 0x80000000u ) != 0;
      const unsigned len = bh[0] & 0x7FFFFFFFu;
      if( raw ) {
        if( len != n || !PkGetAll( h, dst + off, n ) ) {
          ok = false;
          break;
        }
      }
      else {
        if( algo == PK_ALGO_STORE || len == 0 || len >= n || !PkReady( algo ) ) {
          ok = false;
          break;
        }
        if( !tmp ) {
          tmp = PkAlloc( block );
          if( !tmp || !g_pk.createD( PkFlags( algo ), 0, &dec ) || !dec ) {
            ok = false;
            break;
          }
        }
        SIZE_T back = 0;
        if( !PkGetAll( h, tmp, len )
            || !PkCall( g_pk.decompress, dec, tmp, len, dst + off, n, &back )
            || back != n ) {
          ok = false;
          break;
        }
      }
      if( PkSum( dst + off, n ) != bh[1] ) ok = false;
    }
    if( dec )
      g_pk.closeD( dec );
    PkFree( tmp );
    return ok;
  }
  inline bool PkReadAny( HANDLE h, void* out, unsigned size, bool* packed = 0 ) {
    if( packed ) *packed = false;
    if( h == INVALID_HANDLE_VALUE || !h || ( !out && size ) ) return false;
    const DWORD pos = SetFilePointer( h, 0, 0, FILE_CURRENT );
    const DWORD len = GetFileSize( h, 0 );
    if( pos == INVALID_SET_FILE_POINTER || len == INVALID_FILE_SIZE || pos > len ) return false;
    if( len - pos >= PK_HEAD_BYTES ) {
      unsigned int magic = 0;
      if( !PkGetAll( h, &magic, 4 ) ) return false;
      if( SetFilePointer( h, (LONG)pos, 0, FILE_BEGIN ) != pos ) return false;
      if( magic == PK_MAGIC && PkRead( h, out, size ) ) {
        if( packed ) *packed = true;
        return true;
      }
      if( magic == PK_MAGIC && len - pos != size ) return false;
      if( SetFilePointer( h, (LONG)pos, 0, FILE_BEGIN ) != pos ) return false;
    }
    if( len - pos != size ) return false;
    return PkGetAll( h, out, size );
  }
  inline void PkAbsPath( const char* in, char* out ) {
    const DWORD n = GetFullPathNameA( in, PK_PATH_MAX - 8u, out, 0 );
    if( n == 0 || n >= PK_PATH_MAX - 8u ) strncpy_s( out, PK_PATH_MAX - 8u, in ? in : "", _TRUNCATE );
  }
  inline int PkFileKB( const char* path ) {
    WIN32_FILE_ATTRIBUTE_DATA a;
    if( !GetFileAttributesExA( path, GetFileExInfoStandard, &a ) ) return 0;
    return (int)( ( ( (unsigned __int64)a.nFileSizeHigh << 32 ) | a.nFileSizeLow ) / 1024u );
  }
  inline bool PkTmpName( const char* path, char* tmp ) {
    const int n = ( path && path[0] ) ? snprintf( tmp, PK_PATH_MAX, "%s.tmp", path ) : -1;
    return n > 4 && (unsigned)n < PK_PATH_MAX - 4u;
  }
  inline bool PkFinishTmp( HANDLE h, const char* tmp, const char* path, bool ok ) {
    CloseHandle( h );
    if( ok ) {
      for( int t = 0; t < 8; t++ ) {
        if( MoveFileExA( tmp, path, MOVEFILE_REPLACE_EXISTING ) ) return true;
        if( GetFileAttributesA( tmp ) == INVALID_FILE_ATTRIBUTES ) break;
        Sleep( 50 );
      }
    }
    DeleteFileA( tmp );
    return false;
  }
  inline void PkRepackStopFor( const char* path );
  inline bool PkSaveFile( const char* path, const void* head, unsigned headBytes,
                          const void* data, unsigned size, int algo ) {
    char tmp[PK_PATH_MAX];
    if( !PkTmpName( path, tmp ) || ( !head && headBytes ) || ( !data && size ) ) return false;
    PkRepackStopFor( path );
    HANDLE h = CreateFileA( tmp, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) return false;
    const bool ok = PkPutAll( h, head, headBytes ) && PkWriteAuto( h, data, size, algo );
    return PkFinishTmp( h, tmp, path, ok );
  }
  inline bool PkRepackFile( const char* path, unsigned headBytes, unsigned size, int algo,
                            unsigned packedMagic = 0 ) {
    char tmp[PK_PATH_MAX];
    if( algo == PK_ALGO_STORE || headBytes > 64u || !PkTmpName( path, tmp ) || !PkReady( algo ) ) return false;
    HANDLE s = CreateFileA( path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( s == INVALID_HANDLE_VALUE ) return false;
    unsigned char head[64];
    const DWORD len = GetFileSize( s, 0 );
    FILETIME ft;
    const bool haveTime = GetFileTime( s, 0, 0, &ft ) != 0;
    bool ok = ( len != INVALID_FILE_SIZE ) && len == headBytes + size && PkGetAll( s, head, headBytes );
    if( ok && packedMagic && headBytes >= 4 )
      *(unsigned int*)head = packedMagic;
    if( ok && size >= 4 ) {
      unsigned int magic = 0;
      ok = PkGetAll( s, &magic, 4 )
        && SetFilePointer( s, (LONG)headBytes, 0, FILE_BEGIN ) == headBytes
        && magic != PK_MAGIC;
    }
    HANDLE d = INVALID_HANDLE_VALUE;
    if( ok ) {
      d = CreateFileA( tmp, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
      ok = ( d != INVALID_HANDLE_VALUE );
    }
    if( ok ) ok = PkPutAll( d, head, headBytes ) && PkEncode( d, 0, s, size, algo, true );
    if( ok && haveTime )
      SetFileTime( d, 0, 0, &ft );
    CloseHandle( s );
    if( d == INVALID_HANDLE_VALUE ) return false;
    return PkFinishTmp( d, tmp, path, ok );
  }
  struct TPkRepack {
    char     path[PK_PATH_MAX];
    unsigned headBytes;
    unsigned size;
    int      algo;
    unsigned packedMagic;
  };
  inline volatile LONG g_pkRepackBusy = 0;
  inline char g_pkRepackPath[PK_PATH_MAX];
  inline volatile LONG g_pkRepackCut = 0;
  inline char g_pkBad[16][PK_PATH_MAX];
  inline int  g_pkBadCount = 0;
  inline bool PkSameStr( const char* a, const char* b ) { return strcmp( a, b ) == 0; }
  inline bool PkIsBad( const char* absPath ) {
    for( int i = 0; i < g_pkBadCount; i++ )
      if( PkSameStr( g_pkBad[i], absPath ) ) return true;
    return false;
  }
  inline bool PkJobBusyPath( const char* absPath );
  inline void PkMarkBad( const char* absPath ) {
    if( g_pkBadCount >= 16 || PkIsBad( absPath ) ) return;
    strncpy_s( g_pkBad[g_pkBadCount++], PK_PATH_MAX, absPath, _TRUNCATE );
  }
  inline DWORD WINAPI PkRepackMain( LPVOID p ) {
    TPkRepack* r = (TPkRepack*)p;
    const bool done = PkRepackFile( r->path, r->headBytes, r->size, r->algo, r->packedMagic );
    if( !done && !g_pkRepackStop && !g_pkRepackCut && g_pkBadCount < 16 && !PkIsBad( r->path ) ) {
      strncpy_s( g_pkBad[g_pkBadCount++], PK_PATH_MAX, r->path, _TRUNCATE );
    }
    PkFree( r );
    InterlockedExchange( &g_pkRepackBusy, 0 );
    return 0;
  }
  inline bool PkRepackAsync( const char* path, unsigned headBytes, unsigned size, int algo,
                             unsigned packedMagic = 0 ) {
    if( !path || algo == PK_ALGO_STORE || !PkReady( algo ) ) return false;
    if( InterlockedCompareExchange( &g_pkRepackBusy, 1, 0 ) != 0 ) return false;
    TPkRepack* r = (TPkRepack*)PkAlloc( sizeof( TPkRepack ) );
    bool ok = ( r != 0 );
    if( ok ) {
      PkAbsPath( path, r->path );
      ok = !PkJobBusyPath( r->path );
    }
    if( ok ) {
      strncpy_s( g_pkRepackPath, PK_PATH_MAX, r->path, _TRUNCATE );
      InterlockedExchange( &g_pkRepackCut, 0 );
      r->headBytes = headBytes;
      r->size = size;
      r->algo = algo;
      r->packedMagic = packedMagic;
    }
    HANDLE t = 0;
    if( ok ) t = CreateThread( 0, 256 * 1024, PkRepackMain, r, 0, 0 );
    if( !t ) {
      PkFree( r );
      InterlockedExchange( &g_pkRepackBusy, 0 );
      return false;
    }
    SetThreadPriority( t, THREAD_PRIORITY_LOWEST );
    CloseHandle( t );
    return true;
  }
  const int PK_DIR_OPEN_MAX = 4;
  inline bool PkRawSizeOk( unsigned fileBytes, unsigned headBytes ) {
    if( fileBytes <= headBytes ) return false;
    const unsigned px = fileBytes - headBytes;
    for( unsigned bpp = 1; bpp <= 4; bpp++ ) {
      if( px % bpp ) continue;
      const unsigned q = px / bpp;
      unsigned lo = 16, hi = 16384;
      while( lo < hi ) {
        const unsigned mid = ( lo + hi ) / 2u;
        if( mid * mid < q ) lo = mid + 1u;
        else                hi = mid;
      }
      if( lo * lo == q ) return true;
    }
    return false;
  }
  inline int PkRepackDirNext( const char* dir, unsigned magic, int headInts, int nIdx,
                              int bppIdx, int algo, unsigned packedMagic = 0 ) {
    if( !dir || headInts < 3 || headInts > 8 || nIdx < 0 || nIdx >= headInts
        || bppIdx < 0 || bppIdx >= headInts )
      return 0;
    if( algo == PK_ALGO_STORE || !PkReady( algo ) || g_pkRepackBusy ) return -1;
    char mask[PK_PATH_MAX];
    const int mn = snprintf( mask, PK_PATH_MAX, "%s\\*.cache", dir );
    if( mn <= 0 || (unsigned)mn >= PK_PATH_MAX - 8u ) return 0;
    WIN32_FIND_DATAA fd;
    HANDLE fh = FindFirstFileA( mask, &fd );
    if( fh == INVALID_HANDLE_VALUE ) return 0;
    int result = 0;
    int opened = 0;
    do {
      if( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) || fd.nFileSizeHigh != 0 ) continue;
      if( !PkRawSizeOk( fd.nFileSizeLow, (unsigned)headInts * 4u ) ) continue;
      char full[PK_PATH_MAX];
      const int fn = snprintf( full, PK_PATH_MAX, "%s\\%s", dir, fd.cFileName );
      if( fn <= 0 || (unsigned)fn >= PK_PATH_MAX ) continue;
      char absFull[PK_PATH_MAX];
      PkAbsPath( full, absFull );
      if( PkIsBad( absFull ) ) continue;
      if( opened >= PK_DIR_OPEN_MAX ) {
        result = -1;
        break;
      }
      opened++;
      HANDLE h = CreateFileA( full, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
      int head[8];
      bool got = false;
      if( h != INVALID_HANDLE_VALUE ) {
        got = PkGetAll( h, head, (unsigned)headInts * 4u );
        CloseHandle( h );
      }
      const int n = got ? head[nIdx] : 0, bpp = got ? head[bppIdx] : 0;
      const unsigned size = ( n >= 16 && n <= 16384 && bpp >= 1 && bpp <= 4 )
                          ? (unsigned)n * (unsigned)n * (unsigned)bpp : 0u;
      if( !got || (unsigned)head[0] != magic || !size
          || fd.nFileSizeLow != (unsigned)headInts * 4u + size ) {
        PkMarkBad( absFull );
        continue;
      }
      result = PkRepackAsync( full, (unsigned)headInts * 4u, size, algo, packedMagic ) ? 1 : -1;
      break;
    } while( FindNextFileA( fh, &fd ) );
    FindClose( fh );
    return result;
  }
  inline void PkRepackAbort() {
    InterlockedExchange( &g_pkRepackStop, 1 );
    for( unsigned t = 0; g_pkRepackBusy && t < 5000; t += 10 )
      Sleep( 10 );
    InterlockedExchange( &g_pkRepackStop, 0 );
  }
  inline void PkRepackStopFor( const char* path ) {
    if( !path || !g_pkRepackBusy ) return;
    char abs[PK_PATH_MAX];
    PkAbsPath( path, abs );
    if( !g_pkRepackBusy || !PkSameStr( abs, g_pkRepackPath ) ) return;
    InterlockedExchange( &g_pkRepackCut, 1 );
    InterlockedExchange( &g_pkRepackStop, 1 );
    for( unsigned t = 0; g_pkRepackBusy && t < 5000; t += 5 )
      Sleep( 5 );
    InterlockedExchange( &g_pkRepackStop, 0 );
  }
  inline bool PkRepackWait( unsigned ms ) {
    for( unsigned t = 0; g_pkRepackBusy && t < ms; t += 20 )
      Sleep( 20 );
    return g_pkRepackBusy == 0;
  }
  inline int PkCleanTmp( const char* dir ) {
    if( !dir ) return 0;
    char mask[PK_PATH_MAX];
    const int mn = snprintf( mask, PK_PATH_MAX, "%s\\*.tmp", dir );
    if( mn <= 0 || (unsigned)mn >= PK_PATH_MAX - 8u ) return 0;
    WIN32_FIND_DATAA fd;
    HANDLE fh = FindFirstFileA( mask, &fd );
    if( fh == INVALID_HANDLE_VALUE ) return 0;
    int removed = 0;
    do {
      if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) continue;
      const size_t len = strlen( fd.cFileName );
      if( len < 11 || _stricmp( fd.cFileName + len - 10, ".cache.tmp" ) != 0 ) continue;
      char full[PK_PATH_MAX];
      const int fn = snprintf( full, PK_PATH_MAX, "%s\\%s", dir, fd.cFileName );
      if( fn > 0 && (unsigned)fn < PK_PATH_MAX && DeleteFileA( full ) ) removed++;
    } while( FindNextFileA( fh, &fd ) );
    FindClose( fh );
    return removed;
  }
  struct TPkJob {
    HANDLE         thread;
    HANDLE         file;
    const void*    data;
    unsigned       size;
    int            algo;
    unsigned       headBytes;
    unsigned char  head[64];
    char           path[PK_PATH_MAX];
    char           tmp[PK_PATH_MAX];
    volatile LONG  active;
    volatile LONG  done;
    volatile LONG  ok;
  };
  inline DWORD WINAPI PkJobMain( LPVOID p ) {
    TPkJob* j = (TPkJob*)p;
    const bool ok = PkPutAll( j->file, j->head, j->headBytes )
                 && PkWriteAuto( j->file, j->data, j->size, j->algo );
    const bool fin = PkFinishTmp( j->file, j->tmp, j->path, ok );
    j->file = INVALID_HANDLE_VALUE;
    InterlockedExchange( &j->ok, fin ? 1 : 0 );
    InterlockedExchange( &j->done, 1 );
    return 0;
  }
  inline TPkJob* g_pkJobs[4] = { 0, 0, 0, 0 };
  inline void PkJobTrack( TPkJob* j, bool on ) {
    for( int i = 0; i < 4; i++ )
      if( g_pkJobs[i] == j ) g_pkJobs[i] = 0;
    for( int i = 0; on && i < 4; i++ )
      if( !g_pkJobs[i] ) {
        g_pkJobs[i] = j;
        break;
      }
  }
  inline bool PkJobBusyPath( const char* absPath ) {
    for( int i = 0; i < 4; i++ )
      if( g_pkJobs[i] && g_pkJobs[i]->active && !g_pkJobs[i]->done
          && PkSameStr( g_pkJobs[i]->path, absPath ) )
        return true;
    return false;
  }
  inline bool PkJobDone( const TPkJob& j ) {
    return j.active == 0 || j.done != 0;
  }
  inline bool PkJobWait( TPkJob& j ) {
    if( j.thread ) {
      WaitForSingleObject( j.thread, INFINITE );
      CloseHandle( j.thread );
      j.thread = 0;
    }
    const bool r = ( j.active == 0 ) ? true : ( j.ok != 0 );
    j.active = 0;
    PkJobTrack( &j, false );
    return r;
  }
  inline bool PkJobStart( TPkJob& j, const char* path, const void* head, unsigned headBytes,
                          const void* data, unsigned size, int algo ) {
    if( !path || headBytes > sizeof( j.head ) || ( !data && size ) || ( !head && headBytes ) ) return false;
    PkJobWait( j );
    PkReady( algo );
    PkAbsPath( path, j.path );
    if( !PkTmpName( j.path, j.tmp ) ) return false;
    PkRepackStopFor( j.path );
    j.file = CreateFileA( j.tmp, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
    if( j.file == INVALID_HANDLE_VALUE ) return false;
    if( headBytes ) memcpy( j.head, head, headBytes );
    j.headBytes = headBytes;
    j.data = data;
    j.size = size;
    j.algo = algo;
    j.ok = 0;
    j.done = 0;
    j.active = 1;
    PkJobTrack( &j, true );
    j.thread = CreateThread( 0, 256 * 1024, PkJobMain, &j, 0, 0 );
    if( !j.thread )
      PkJobMain( &j );
    else
      SetThreadPriority( j.thread, THREAD_PRIORITY_BELOW_NORMAL );
    return true;
  }
}
