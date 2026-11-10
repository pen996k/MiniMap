#pragma once
namespace MiniMap {
  static const char* const ICONS_DIR      = "MiniMap\\icons";
  static const char* const ICONS_DEF_DIR  = "MiniMap\\icons\\default";
  static const char* const ICONS_BAK_DIR  = "MiniMap\\icons\\backup";
  static const char* const ICONS_STAMP    = "MiniMap\\icons\\default\\pack.ver";
  inline const unsigned char* IconPackRes( DWORD& size ) {
    size = 0;
    HMODULE self = ClashSelf();
    if( !self ) return 0;
    HRSRC r = FindResourceA( self, "MMICONPACK", MAKEINTRESOURCEA( 10 ) );
    if( !r ) return 0;
    HGLOBAL g = LoadResource( self, r );
    if( !g ) return 0;
    size = SizeofResource( self, r );
    return (const unsigned char*)LockResource( g );
  }
  inline unsigned long IconRdU32( const unsigned char* p ) {
    return (unsigned long)p[0] | ( (unsigned long)p[1] << 8 ) | ( (unsigned long)p[2] << 16 ) | ( (unsigned long)p[3] << 24 );
  }
  inline unsigned char* IconReadFile( const char* path, int& len, int maxLen ) { return MiniMapCfg::ReadFile( path, len, maxLen ); }
  inline bool IconWriteFile( const char* path, const unsigned char* d, int n ) {
    HANDLE h = CreateFileA( path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) return false;
    DWORD wr = 0;
    const BOOL ok = WriteFile( h, d, (DWORD)n, &wr, 0 );
    CloseHandle( h );
    return ok && (int)wr == n;
  }
  inline void IconMakeDirs( const char* file ) {
    char p[MAX_PATH];
    int n = 0;
    for( ; file[n] && n < MAX_PATH - 1; n++ ) {
      p[n] = file[n];
      if( file[n] == '\\' ) {
        p[n] = 0;
        CreateDirectoryA( p, 0 );
        p[n] = '\\';
      }
    }
  }
  static int g_icoPackFiles = 0, g_icoPackWritten = 0, g_icoPackBackup = 0, g_icoPackStale = 0, g_icoPackRemoved = 0;
  static const unsigned long ( *g_iconKnown )[2] = kIconKnown;
  static int g_iconKnownN = kIconKnownN;
  inline bool IconKnownFile( const char* rel, const unsigned char* d, int n ) {
    if( !rel || !d || n < 0 ) return false;
    char low[MAX_PATH];
    int k = 0;
    for( ; rel[k] && k < MAX_PATH - 1; k++ ) {
      char c = rel[k];
      if( c == '/' ) c = '\\';
      if( c >= 'A' && c <= 'Z' ) c = (char)( c - 'A' + 'a' );
      low[k] = c;
    }
    low[k] = 0;
    const unsigned long pc = MiniMapImage::Crc32( (const unsigned char*)low, k );
    const unsigned long dc = MiniMapImage::Crc32( d, n );
    int lo = 0, hi = g_iconKnownN - 1;
    while( lo <= hi ) {
      const int mid = ( lo + hi ) / 2;
      const unsigned long a = g_iconKnown[mid][0], b = g_iconKnown[mid][1];
      if( a == pc && b == dc ) return true;
      if( a < pc || ( a == pc && b < dc ) ) lo = mid + 1;
      else hi = mid - 1;
    }
    return false;
  }
  inline bool IconPackHas( const unsigned char* pk, DWORD size, const char* rel ) {
    const unsigned long cnt = IconRdU32( pk + 8 );
    DWORD at = 12;
    for( unsigned long i = 0; i < cnt && i < 4096; i++ ) {
      if( at + 2 > size ) return false;
      const int nl = pk[at] | ( pk[at + 1] << 8 );
      at += 2;
      if( nl <= 0 || at + nl + 4 > size ) return false;
      if( nl == (int)strlen( rel ) && !_strnicmp( (const char*)pk + at, rel, nl ) ) return true;
      at += nl;
      const DWORD dl = IconRdU32( pk + at );
      at += 4;
      if( dl > size - at ) return false;
      at += dl;
    }
    return false;
  }
  inline int IconPackMoveStale( const unsigned char* pk, DWORD size, const char* want, const char* sub, int depth, int& removed ) {
    if( depth > 3 ) return 0;
    const int removedBefore = removed;
    char dir[MAX_PATH], mask[MAX_PATH];
    if( sub[0] ) { if( !FilePath( dir, MAX_PATH, ICONS_DEF_DIR, sub ) ) return 0; }
    else MiniMapCfg::Copy( dir, MAX_PATH, ICONS_DEF_DIR );
    if( !FilePath( mask, MAX_PATH, dir, "*" ) ) return 0;
    int moved = 0;
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA( mask, &fd );
    if( h == INVALID_HANDLE_VALUE ) return 0;
    do {
      if( fd.cFileName[0] == '.' ) continue;
      char rsub[MAX_PATH];
      if( sub[0] ) { if( !FilePath( rsub, MAX_PATH, sub, fd.cFileName ) ) continue; }
      else MiniMapCfg::Copy( rsub, MAX_PATH, fd.cFileName );
      if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
        moved += IconPackMoveStale( pk, size, want, rsub, depth + 1, removed );
        continue;
      }
      const int fl = (int)strlen( fd.cFileName );
      if( fl < 5 || ( _stricmp( fd.cFileName + fl - 4, ".png" ) && _stricmp( fd.cFileName + fl - 4, ".txt" ) ) ) continue;
      char rel[MAX_PATH];
      if( !FilePath( rel, MAX_PATH, "default", rsub ) || IconPackHas( pk, size, rel ) ) continue;
      char src[MAX_PATH], bdir[MAX_PATH], dst[MAX_PATH];
      if( !FilePath( src, MAX_PATH, dir, fd.cFileName ) || !FilePath( bdir, MAX_PATH, ICONS_BAK_DIR, want )
          || !FilePath( dst, MAX_PATH, bdir, rsub ) )
        continue;
      {
        int ol = 0;
        unsigned char* old = IconReadFile( src, ol, 16 * 1024 * 1024 );
        const bool known = old && IconKnownFile( rel, old, ol );
        MiniMapCommon::MmFree( old );
        if( known ) {
          if( DeleteFileA( src ) )
            removed++;
          continue;
        }
      }
      IconMakeDirs( dst );
      DeleteFileA( dst );
      if( MoveFileA( src, dst ) )
        moved++;
    } while( FindNextFileA( h, &fd ) );
    FindClose( h );
    if( sub[0] && ( moved > 0 || removed > removedBefore ) )
      RemoveDirectoryA( dir );
    return moved;
  }
  static const unsigned long MARKS_TXT_OLD_CRC = 0x399110A6UL;
  static const int           MARKS_TXT_OLD_LEN = 798;
  inline bool IconPackOldOurs( const char* rel, const unsigned char* old, int ol ) {
    return !_stricmp( rel, "marks\\marks.txt" ) && ol == MARKS_TXT_OLD_LEN
        && MiniMapImage::Crc32( old, ol ) == MARKS_TXT_OLD_CRC;
  }
  inline void IconPackInstallFrom( const unsigned char* pk, DWORD size ) {
    if( !pk || size < 12 || memcmp( pk, "MMIP", 4 ) || IconRdU32( pk + 4 ) != 1 ) {
      MM_LOG( "icons: no built-in icon pack in MiniMap.dll - only files in MiniMap\\Icons" );
      return;
    }
    FilesEnsure();
    CreateDirectoryA( ICONS_DIR, 0 );
    CreateDirectoryA( ICONS_DEF_DIR, 0 );
    const unsigned long crc = MiniMapImage::Crc32( pk, (int)size );
    char want[16];
    snprintf( want, sizeof( want ), "%08lx", crc );
    {
      int n = 0;
      unsigned char* cur = IconReadFile( ICONS_STAMP, n, 64 );
      const bool same = cur && n >= 8 && !memcmp( cur, want, 8 );
      MiniMapCommon::MmFree( cur );
      if( same ) {
        g_icoPackFiles = (int)IconRdU32( pk + 8 );
        return;
      }
    }
    const unsigned long cnt = IconRdU32( pk + 8 );
    DWORD at = 12;
    int written = 0, kept = 0, backed = 0, bad = 0;
    for( unsigned long i = 0; i < cnt && i < 4096; i++ ) {
      if( at + 2 > size ) { bad++; break; }
      const int nl = pk[at] | ( pk[at + 1] << 8 );
      at += 2;
      if( nl <= 0 || nl > 200 || at + nl + 4 > size ) { bad++; break; }
      char rel[208];
      memcpy( rel, pk + at, nl );
      rel[nl] = 0;
      at += nl;
      const DWORD dl = IconRdU32( pk + at );
      at += 4;
      if( dl > size - at ) { bad++; break; }
      const unsigned char* data = pk + at;
      at += dl;
      if( strstr( rel, ".." ) || strchr( rel, ':' ) || rel[0] == '\\' || rel[0] == '/' ) { bad++; continue; }
      char path[MAX_PATH];
      if( !FilePath( path, MAX_PATH, ICONS_DIR, rel ) ) { bad++; continue; }
      int ol = 0;
      unsigned char* old = IconReadFile( path, ol, 16 * 1024 * 1024 );
      if( old && ol == (int)dl && !memcmp( old, data, dl ) ) {
        MiniMapCommon::MmFree( old );
        continue;
      }
      const bool ours = !_strnicmp( rel, "Default\\", 8 );
      if( old && !ours && !IconPackOldOurs( rel, old, ol ) ) {
        MiniMapCommon::MmFree( old );
        kept++;
        continue;
      }
      const bool oldOurs = old && ours && IconKnownFile( rel, old, ol );
      if( old && !oldOurs ) {
        char bdir[MAX_PATH], bpath[MAX_PATH];
        FilePath( bdir, MAX_PATH, ICONS_BAK_DIR, want );
        if( FilePath( bpath, MAX_PATH, bdir, ours ? rel + 8 : rel ) ) {
          IconMakeDirs( bpath );
          if( IconWriteFile( bpath, old, ol ) )
            backed++;
        }
      }
      MiniMapCommon::MmFree( old );
      IconMakeDirs( path );
      if( IconWriteFile( path, data, (int)dl ) )
        written++;
      else
        bad++;
    }
    int removed = 0;
    const int stale = bad ? 0 : IconPackMoveStale( pk, size, want, "", 0, removed );
    g_icoPackFiles   = (int)cnt;
    g_icoPackWritten = written;
    g_icoPackBackup  = backed;
    g_icoPackStale   = stale;
    g_icoPackRemoved = removed;
    if( !bad )
      IconWriteFile( ICONS_STAMP, (const unsigned char*)want, 8 );
    MM_LOG( zSTRING( "icons: built-in pack " ) + zSTRING( want ) + ", files " + zSTRING( (int)cnt ) + ", written " + zSTRING( written ) + ", your changed copies kept " + zSTRING( kept ) + ", your changed default files saved to icons\\backup " + zSTRING( backed ) + ", old pack files removed " + zSTRING( removed ) + ", other files not in the pack moved to icons\\backup " + zSTRING( stale ) + ( bad ? zSTRING( ", ERRORS " ) + zSTRING( bad ) : zSTRING( "" ) ) );
  }
  static const unsigned char* ( *g_iconPackSrc )( DWORD& size ) = IconPackRes;
  static bool g_icoPackDone = false;
  inline void IconPackInstall() {
    if( g_icoPackDone ) return;
    g_icoPackDone = true;
    DWORD size = 0;
    const unsigned char* pk = g_iconPackSrc( size );
    IconPackInstallFrom( pk, size );
  }
  enum { ICO_ATLAS = 1024, ICO_LIST_MAX = 64 };
  static const int kIcoCellS[6] = { 48, 32, 24, 16, 12, 8 };
  static int       g_icoCellY[6];
  static int       g_icoAtlasH = ICO_ATLAS;
  inline bool IconUploadRect( zCTexture* tex, const unsigned char* px, int w, int h, int bpp ) { return GenUploadRect( tex, px, w, h, bpp ); }
  enum { ICO_MISS_MAX = 64 };
  static char g_icoMiss[ICO_MISS_MAX][MiniMapIcons::KEY_LEN];
  static int  g_icoMissN = 0;
  static bool g_icoMissLogged = false;
  static bool g_icoHitLogged  = false;
  inline void IconMissFree() { g_icoMissN = 0; g_icoMissLogged = false; g_icoHitLogged = false; }
  static MiniMapIcons::TLists g_icoL;
  static bool          g_icoReady  = false;
  static zCTexture*    g_icoTex    = 0;
  static unsigned char g_icoOk[MiniMapIcons::MAX_IMAGES];
  static unsigned char g_icoFlags[MiniMapIcons::MAX_IMAGES];
  static char          g_icoListName[ICO_LIST_MAX][MAX_PATH];
  static int           g_icoLists  = 0;
  static int           g_icoMsgs   = 0;
  static const MiniMapIcons::TEntry* g_icoFallback[3] = { 0, 0, 0 };
  static short* g_npcIco    = 0;
  static int    g_npcIcoCap = 0;
  static int    g_npcIcoN   = 0;
  inline void NpcIcoFree() {
    MiniMapCommon::MmFree( g_npcIco );
    g_npcIco    = 0;
    g_npcIcoCap = 0;
    g_npcIcoN   = 0;
  }
  static short* g_itemIco    = 0;
  static int    g_itemIcoCap = 0;
  static int    g_itemIcoN   = 0;
  inline void ItemIcoFree() {
    MiniMapCommon::MmFree( g_itemIco );
    g_itemIco    = 0;
    g_itemIcoCap = 0;
    g_itemIcoN   = 0;
  }
  inline void IconMsg( void*, int list, int line, const char* msg ) {
    if( !g_log ) return;
    if( ++g_icoMsgs > 200 ) {
      if( g_icoMsgs == 201 )
        MM_LOG( "icons: more than 200 messages - the rest is not logged" );
      return;
    }
    const char* nm = ( list >= 0 && list < g_icoLists ) ? g_icoListName[list] : "MiniMap\\Icons";
    zSTRING s = zSTRING( "icons: " ) + zSTRING( nm );
    if( line > 0 ) s = s + " line " + zSTRING( line );
    MM_LOG( s + ": " + zSTRING( msg ) );
  }
  inline int IconGuildFn( void*, const char* name ) {
    if( !parser || !name || !*name ) return -1;
    zCPar_Symbol* s = parser->GetSymbol( zSTRING( name ) );
    if( !s ) return -1;
    int v = -1;
    s->GetValue( v, 0 );
    return ( v >= 0 && v < 100000 ) ? v : -1;
  }
  inline void IconListLoad( const char* full, const char* dirRel ) {
    if( g_icoLists >= ICO_LIST_MAX ) {
      if( g_icoLists == ICO_LIST_MAX )
        MM_LOG( "icons: more than 64 lists - the rest is ignored" );
      g_icoLists++;
      return;
    }
    const int idx = g_icoLists++;
    strncpy_s( g_icoListName[idx], MAX_PATH, full ? full : "", _TRUNCATE );
    int len = 0;
    unsigned char* txt = IconReadFile( full, len, 4 * 1024 * 1024 );
    if( !txt ) {
      IconMsg( 0, idx, 0, len == -2 ? "list is larger than 4 MB - skipped" : "cannot read the list" );
      return;
    }
    const int added = MiniMapIcons::Parse( g_icoL, (const char*)txt, len, dirRel, idx, IconMsg, 0 );
    MiniMapCommon::MmFree( txt );
    if( !added )
      IconMsg( 0, idx, 0, "no usable lines in this list" );
  }
  inline int IconCmpStr( const void* a, const void* b ) {
    return _stricmp( (const char*)a, (const char*)b );
  }
  inline void IconListsIn( const char* dir, const char* rel, int depth ) {
    char mask[MAX_PATH];
    if( !FilePath( mask, MAX_PATH, dir, "*" ) ) return;
    enum { MAXN = 128 };
    static char files[3][MAXN][MAX_PATH];
    static char dirs[3][MAXN][MAX_PATH];
    if( depth > 2 ) return;
    int nf = 0, nd = 0;
    WIN32_FIND_DATAA fd;
    HANDLE fh = FindFirstFileA( mask, &fd );
    if( fh == INVALID_HANDLE_VALUE ) return;
    do {
      if( fd.cFileName[0] == '.' ) continue;
      if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
        if( depth == 0 && ( !_stricmp( fd.cFileName, "Default" ) || !_stricmp( fd.cFileName, "Backup" )
                            || !_stricmp( fd.cFileName, "Custom" )
                            || !_stricmp( fd.cFileName, "marks" )
                            || !_stricmp( fd.cFileName, "extra" ) ) )
          continue;
        if( nd < MAXN ) { MiniMapIcons::Copy( dirs[depth][nd++], MAX_PATH, fd.cFileName ); }
        continue;
      }
      const int fl = (int)strlen( fd.cFileName );
      if( fl > 4 && !_stricmp( fd.cFileName + fl - 4, ".txt" ) && _stricmp( fd.cFileName, "README.txt" )
          && _stricmp( fd.cFileName, "README_old.txt" )
          && nf < MAXN )
        MiniMapIcons::Copy( files[depth][nf++], MAX_PATH, fd.cFileName );
    } while( FindNextFileA( fh, &fd ) );
    FindClose( fh );
    qsort( files[depth], nf, MAX_PATH, IconCmpStr );
    qsort( dirs[depth], nd, MAX_PATH, IconCmpStr );
    for( int i = 0; i < nf; i++ ) {
      char full[MAX_PATH];
      if( FilePath( full, MAX_PATH, dir, files[depth][i] ) )
        IconListLoad( full, rel );
    }
    if( depth >= 1 ) return;
    for( int i = 0; i < nd; i++ ) {
      char sub[MAX_PATH], subRel[MAX_PATH];
      if( !FilePath( sub, MAX_PATH, dir, dirs[depth][i] ) ) continue;
      if( !FilePath( subRel, MAX_PATH, rel, dirs[depth][i] ) ) continue;
      IconListsIn( sub, subRel[0] == '\\' ? subRel + 1 : subRel, depth + 1 );
    }
  }
  inline void IconCellXY( int cell, int img, int& x, int& y ) {
    const int S = kIcoCellS[cell];
    const int perRow = ICO_ATLAS / S;
    x = ( img % perRow ) * S;
    y = g_icoCellY[cell] + ( img / perRow ) * S;
  }
  static const char* const kIconStyleDir[4] = { "simple", "textured", "bare", "custom" };
  static const char* const ICONS_CUSTOM_DIR = "MiniMap\\icons\\custom";
  static const char* g_iconGame = "gothic2";
  inline const char* IconGameName() { return zSwitch( "gothic1", "gothic1", g_iconGame, g_iconGame ); }
  inline const char* IconDetectGame() { return zSwitch( "gothic1", "gothic1", "gothic2", "gothic2" ); }
  inline bool IconResolvePathDir( char* out, int cap, const char* dir, const char* file, const char* styleDir, const char* game, int* styled = 0 ) {
    if( styled ) *styled = 0;
    if( !FilePath( out, cap, dir, file ) ) return false;
    if( !styleDir || !*styleDir || !file || _strnicmp( file, "Default\\", 8 ) || strchr( file + 8, '\\' ) || !file[8] ) return true;
    char rel[MAX_PATH], t[MAX_PATH];
    if( game && *game ) {
      snprintf( rel, sizeof( rel ), "Default\\%s\\%s\\%s", styleDir, game, file + 8 );
      if( FilePath( t, MAX_PATH, dir, rel ) && GetFileAttributesA( t ) != INVALID_FILE_ATTRIBUTES ) {
        MiniMapIcons::Copy( out, cap, t );
        if( styled ) *styled = 1;
        return true;
      }
    }
    snprintf( rel, sizeof( rel ), "Default\\%s\\%s", styleDir, file + 8 );
    if( FilePath( t, MAX_PATH, dir, rel ) && GetFileAttributesA( t ) != INVALID_FILE_ATTRIBUTES ) {
      MiniMapIcons::Copy( out, cap, t );
      if( styled ) *styled = 1;
    }
    return true;
  }
  inline bool IconResolveCustom( char* out, int cap, const char* dir, const char* file ) {
    if( !FilePath( out, cap, dir, file ) ) return false;
    if( !file || _strnicmp( file, "Default\\", 8 ) || strchr( file + 8, '\\' ) || !file[8] ) return true;
    char t[MAX_PATH];
    if( FilePath( t, MAX_PATH, ICONS_CUSTOM_DIR, file + 8 ) && GetFileAttributesA( t ) != INVALID_FILE_ATTRIBUTES ) {
      MiniMapIcons::Copy( out, cap, t );
      return true;
    }
    return IconResolvePathDir( out, cap, dir, file, kIconStyleDir[1], g_iconGame );
  }
  inline bool IconResolvePath( char* out, int cap, const char* dir, const char* file, int style, const char* game ) {
    if( style == 3 ) return IconResolveCustom( out, cap, dir, file );
    return IconResolvePathDir( out, cap, dir, file, ( style >= 0 && style <= 2 ) ? kIconStyleDir[style] : "", game );
  }
  inline bool MarkStylePath( char* out, int cap, const char* name, int style, const char* game ) {
    if( style < 0 || style > 3 || !name || !*name ) return false;
    char file[MAX_PATH];
    snprintf( file, sizeof( file ), "Default\\mark_%s.png", name );
    if( style == 3 ) {
      if( !IconResolveCustom( out, cap, ICONS_DIR, file ) ) return false;
      return _strnicmp( out, ICONS_CUSTOM_DIR, strlen( ICONS_CUSTOM_DIR ) ) == 0;
    }
    int styled = 0;
    if( !IconResolvePathDir( out, cap, ICONS_DIR, file, kIconStyleDir[style], game, &styled ) ) return false;
    return styled != 0;
  }
  inline int IconAtlasBuild() {
    const int n = g_icoL.nimg;
    int y = 0;
    for( int c = 0; c < 6; c++ ) {
      g_icoCellY[c] = y;
      const int perRow = ICO_ATLAS / kIcoCellS[c];
      const int rows = ( n + perRow - 1 ) / perRow;
      y += rows * kIcoCellS[c];
    }
    if( !n ) return 0;
    if( y > ICO_ATLAS ) {
      MM_LOG( "icons: too many icon files for the atlas - using built-in icons" );
      return 0;
    }
    zTRnd_TextureFormat tfmt = zRND_TEX_FORMAT_ARGB_4444;
    int bpp = 0;
    if( !PickTexFormat( true, tfmt, bpp ) ) {
      MM_LOG( "icons: no texture format with alpha - using built-in icons" );
      return 0;
    }
    const int fmt = (int)tfmt;
    int H = 64;
    while( H < y ) H <<= 1;
    unsigned long* px = MiniMapCommon::MmAlloc<unsigned long>( (size_t)( ICO_ATLAS * H ) );
    if( !px ) {
      MM_LOG( "icons: not enough memory for the icon atlas - using built-in icons" );
      return 0;
    }
    memset( px, 0, sizeof( unsigned long ) * ICO_ATLAS * H );
    int loaded = 0, keyed = 0;
    for( int i = 0; i < n; i++ ) {
      g_icoOk[i] = 0;
      g_icoFlags[i] = g_icoL.img[i].flags;
      char path[MAX_PATH];
      if( !IconResolvePath( path, MAX_PATH, ICONS_DIR, g_icoL.img[i].file, g_iconStyle, g_iconGame ) ) continue;
      int len = 0;
      unsigned char* file = IconReadFile( path, len, MiniMapImage::IMG_MAX_BYTES );
      if( !file ) {
        IconMsg( 0, g_icoL.img[i].src, g_icoL.img[i].line,
                 len == -2 ? "icon file is larger than 8 MB" : "icon file not found (check the name and folder)" );
        continue;
      }
      unsigned char* rgba = 0;
      int w = 0, h = 0;
      const char* err = MiniMapImage::Decode( file, len, rgba, w, h );
      MiniMapCommon::MmFree( file );
      if( err ) {
        char m[200];
        snprintf( m, sizeof( m ), "%s: %s", g_icoL.img[i].file, err );
        IconMsg( 0, g_icoL.img[i].src, g_icoL.img[i].line, m );
        continue;
      }
      MiniMapImage::TSrc s;
      if( !MiniMapImage::Prep( rgba, w, h, s ) ) {
        char m[200];
        snprintf( m, sizeof( m ), "%s: image is fully transparent", g_icoL.img[i].file );
        IconMsg( 0, g_icoL.img[i].src, g_icoL.img[i].line, m );
        MiniMapImage::Free( rgba );
        continue;
      }
      if( s.keyed )
        keyed++;
      if( g_iconStyle == 3 && !_strnicmp( path, ICONS_CUSTOM_DIR, strlen( ICONS_CUSTOM_DIR ) )
          && MiniMapImage::IsColored( rgba, w, h ) )
        g_icoFlags[i] |= MiniMapIcons::F_KEEPCOLORS;
      unsigned bf = 0;
      if( g_icoFlags[i] & MiniMapIcons::F_KEEPCOLORS ) bf |= MiniMapImage::KEEPCOLORS;
      if( g_icoFlags[i] & MiniMapIcons::F_NOOUTLINE )  bf |= MiniMapImage::NOOUTLINE;
      for( int c = 0; c < 6; c++ ) {
        int cx, cy;
        IconCellXY( c, i, cx, cy );
        MiniMapImage::Bake( s, kIcoCellS[c], bf, px + cy * ICO_ATLAS + cx, ICO_ATLAS );
      }
      MiniMapImage::Free( rgba );
      g_icoOk[i] = 1;
      loaded++;
    }
    if( !loaded ) {
      MiniMapCommon::MmFree( px );
      return 0;
    }
    PackAtlas16InPlace( px, ICO_ATLAS * H, bpp, fmt );
    zCTexture* tex = CreateAndUploadTex( ICO_ATLAS, H, tfmt, zCOLOR( 128, 128, 128, 128 ), (const unsigned char*)px, bpp );
    const bool ok = ( tex != 0 );
    MiniMapCommon::MmFree( px );
    if( !ok ) {
      MM_LOG( "icons: icon atlas texture failed - using built-in icons" );
      memset( g_icoOk, 0, (size_t)( n ) * sizeof( g_icoOk[0] ) );
      return 0;
    }
    g_icoTex = tex;
    g_icoAtlasH = H;
    if( keyed )
      MM_LOG( zSTRING( "icons: " ) + zSTRING( keyed ) + " image(s) without transparency - background color cut out automatically" );
    return loaded;
  }
  inline int IconCustomCopyDir( const char* srcDir ) {
    char mask[MAX_PATH];
    if( !FilePath( mask, MAX_PATH, srcDir, "*.png" ) ) return 0;
    int n = 0;
    WIN32_FIND_DATAA fd;
    HANDLE fh = FindFirstFileA( mask, &fd );
    if( fh == INVALID_HANDLE_VALUE ) return 0;
    do {
      if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) continue;
      char from[MAX_PATH], to[MAX_PATH];
      if( !FilePath( from, MAX_PATH, srcDir, fd.cFileName ) || !FilePath( to, MAX_PATH, ICONS_CUSTOM_DIR, fd.cFileName ) ) continue;
      if( CopyFileA( from, to, TRUE ) )
        n++;
    } while( FindNextFileA( fh, &fd ) );
    FindClose( fh );
    return n;
  }
  inline void IconCustomFill() {
    static bool done = false;
    if( done ) return;
    done = true;
    CreateDirectoryA( ICONS_DIR, 0 );
    CreateDirectoryA( ICONS_CUSTOM_DIR, 0 );
    char d[MAX_PATH];
    int n = 0;
    if( FilePath( d, MAX_PATH, ICONS_DEF_DIR, "textured" ) ) {
      char g[MAX_PATH];
      if( FilePath( g, MAX_PATH, d, IconDetectGame() ) )
        n += IconCustomCopyDir( g );
      n += IconCustomCopyDir( d );
    }
    n += IconCustomCopyDir( ICONS_DEF_DIR );
    MM_LOG( zSTRING( "icons: own set (MiniMap\\icons\\custom) - copied " ) + zSTRING( n ) + " missing picture(s); replace any of them with your own PNG" );
  }
  inline void IconPackLoad() {
    if( g_icoReady ) return;
    g_icoReady = true;
    g_icoMsgs  = 0;
    g_icoLists = 0;
    MiniMapIcons::Init( g_icoL );
    memset( g_icoOk, 0, sizeof( g_icoOk ) );
    g_icoFallback[0] = g_icoFallback[1] = g_icoFallback[2] = 0;
    LARGE_INTEGER t0, t1, f;
    QueryPerformanceCounter( &t0 );
    IconPackInstall();
    g_iconGame = IconDetectGame();
    if( g_iconTex && g_markGameUsed && strcmp( g_markGameUsed, g_iconGame ) ) {
      g_iconTex->Release();
      g_iconTex   = 0;
      g_iconTried = false;
    }
    if( g_iconStyle == 3 )
      IconCustomFill();
    MM_LOG( zSTRING( "icons: style " ) + zSTRING( g_iconStyle >= 0 && g_iconStyle <= 3 ? kIconStyleDir[g_iconStyle] : "Drawn" ) + ", game " + zSTRING( g_iconGame ) );
    IconListsIn( ICONS_DEF_DIR, "Default", 1 );
    const int defLists = g_icoLists;
    IconListsIn( ICONS_DIR, "", 0 );
    MiniMapIcons::Build( g_icoL, IconMsg, 0, IconGuildFn, 0 );
    static const char* const kFb[3] = { "@HUMAN", "@ORC", "@MONSTER" };
    for( int k = 0; k < 3; k++ )
      g_icoFallback[k] = MiniMapIcons::FindStr( g_icoL, MiniMapIcons::K_NAME, kFb[k] );
    const int loaded = IconAtlasBuild();
    QueryPerformanceCounter( &t1 );
    QueryPerformanceFrequency( &f );
    const int ms = f.QuadPart ? (int)( ( t1.QuadPart - t0.QuadPart ) * 1000 / f.QuadPart ) : 0;
    MM_LOG( zSTRING( "icons: lists " ) + zSTRING( g_icoLists ) + " (Default " + zSTRING( defLists ) + ", yours " + zSTRING( g_icoLists - defLists ) + "), entries " + zSTRING( g_icoL.n ) + ", groups " + zSTRING( g_icoL.ng ) + ", images " + zSTRING( loaded ) + "/" + zSTRING( g_icoL.nimg ) + ", errors " + zSTRING( g_icoL.errors ) + ", notes " + zSTRING( g_icoL.notes ) + ", " + zSTRING( ms ) + " ms" );
  }
  inline void IconPackFree( bool release ) {
    if( g_icoTex && release )
      g_icoTex->Release();
    g_icoTex = 0;
    MiniMapIcons::Free( g_icoL );
    memset( g_icoOk, 0, sizeof( g_icoOk ) );
    g_icoFallback[0] = g_icoFallback[1] = g_icoFallback[2] = 0;
    g_icoReady = false;
    g_npcIcoN  = 0;
    g_itemIcoN = 0;
    IconMissFree();
  }
  inline void IconMissAdd( const char* body, const char* mds ) {
    char k[MiniMapIcons::KEY_LEN];
    const char* src = ( body && *body ) ? body : mds;
    if( !src || !*src || !MiniMapIcons::NormKey( src, (int)strlen( src ),
                                                 ( body && *body ) ? MiniMapIcons::K_BODY : MiniMapIcons::K_MDS, k ) )
      return;
    for( int i = 0; i < g_icoMissN; i++ )
      if( !strcmp( g_icoMiss[i], k ) ) return;
    if( g_icoMissN < ICO_MISS_MAX )
      MiniMapIcons::Copy( g_icoMiss[g_icoMissN++], MiniMapIcons::KEY_LEN, k );
  }
  inline void IconZRaw( const zSTRING& z, char* out, int cap ) { MiniMapCfg::Copy( out, cap, z.ToChar() ); }
  inline int IconNpcCore( oCNpc* npc, char* inst, char* body, char* mds, int& guild ) {
    __try {
      inst[0] = body[0] = mds[0] = 0;
      guild = npc->guild;
      IconZRaw( npc->body_visualName, body, MiniMapIcons::KEY_LEN );
      IconZRaw( npc->mds_name, mds, MiniMapIcons::KEY_LEN );
      const int idx = npc->instanz;
      if( parser && idx >= 0 ) {
        zCPar_Symbol* const s = parser->GetSymbol( idx );
        if( s )
          IconZRaw( s->name, inst, MiniMapIcons::KEY_LEN );
      }
      return 1;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return 0;
    }
  }
  inline int IconNpcEntry( oCNpc* npc ) {
    if( !npc || !g_icoL.n ) return -1;
    char inst[MiniMapIcons::KEY_LEN], body[MiniMapIcons::KEY_LEN], mds[MiniMapIcons::KEY_LEN];
    int guild = -1;
    if( !IconNpcCore( npc, inst, body, mds, guild ) ) return -1;
    const MiniMapIcons::TEntry* e = MiniMapIcons::Resolve( g_icoL, inst, body, mds, guild );
    if( !e ) {
      const bool human = npc->IsHuman() != 0;
      const bool orc   = !human && npc->IsOrc() != 0;
      e = g_icoFallback[human ? 0 : orc ? 1 : 2];
      if( !human )
        IconMissAdd( body, mds );
    }
    return e ? (int)( e - g_icoL.e ) : -1;
  }
  inline void NpcIconsBuild() {
    g_npcIcoN = 0;
    if( !g_npcIcons || !g_npcCache || g_npcCount <= 0 ) return;
    IconPackLoad();
    if( !g_icoTex ) return;
    if( g_npcIcoCap < g_npcCount ) {
      short* const a = MiniMapCommon::MmAlloc<short>( (size_t)( g_npcCount + 64 ) );
      if( !a ) return;
      MiniMapCommon::MmFree( g_npcIco );
      g_npcIco    = a;
      g_npcIcoCap = g_npcCount + 64;
    }
    int hit = 0;
    for( int i = 0; i < g_npcCount; i++ ) {
      oCNpc* const npc = g_npcCache[i];
      const int e = ( npc && npc != player ) ? IconNpcEntry( npc ) : -1;
      g_npcIco[i] = (short)e;
      if( e >= 0 ) hit++;
    }
    g_npcIcoN = g_npcCount;
    if( !g_icoMissLogged && g_icoMissN > 0 && g_log ) {
      g_icoMissLogged = true;
      zSTRING s( "icons: no icon for (add to a list in MiniMap\\Icons): " );
      for( int i = 0; i < g_icoMissN; i++ )
        s = s + zSTRING( i ? ", " : "" ) + zSTRING( g_icoMiss[i] );
      MM_LOG( s );
    }
    if( !g_icoHitLogged ) {
      g_icoHitLogged = true;
      MM_LOG( zSTRING( "icons: characters with a file icon " ) + zSTRING( hit ) + " of " + zSTRING( g_npcCount ) );
    }
  }
  inline int NpcIconLevel( float s = 0.0f ) {
    switch( g_npcIcons ) {
      case 1:  return 1;
      case 2:  return 2;
      case 3:  return ( g_full || s >= 15.0f ) ? 2 : 1;
      default: return 0;
    }
  }
  inline void NpcIconUV( int img, float s, float& u0, float& v0, float& u1, float& v1 ) {
    int cell = 0;
    for( int c = 5; c >= 0; c-- )
      if( (float)kIcoCellS[c] >= s - 0.25f ) { cell = c; break; }
    int cx, cy;
    IconCellXY( cell, img, cx, cy );
    const float iu = 1.0f / (float)ICO_ATLAS, iv = 1.0f / (float)g_icoAtlasH;
    const float S = (float)kIcoCellS[cell];
    u0 = (float)cx * iu;  v0 = (float)cy * iv;
    u1 = u0 + S * iu;     v1 = v0 + S * iv;
  }
  inline void IconFileDraw( int img, float px, float py, float s, const zCOLOR& c ) {
    float u0, v0, u1, v1;
    NpcIconUV( img, s, u0, v0, u1, v1 );
    DrawClippedTile( g_icoTex, px, py, s, u0, v0, u1, v1, zCOLOR( c.r, c.g, c.b, 255 ) );
  }
  inline zCOLOR IconFileColor( int img, const zCOLOR& def ) {
    if( g_icoFlags[img] & MiniMapIcons::F_KEEPCOLORS ) return zCOLOR( 255, 255, 255, 255 );
    if( g_icoFlags[img] & MiniMapIcons::F_COLOR ) {
      const MiniMapIcons::TImage& im = g_icoL.img[img];
      return zCOLOR( im.rgb[0], im.rgb[1], im.rgb[2], 255 );
    }
    return def;
  }
  static const float NPC_FILE_SCALE = 1.25f;
  static const float ITEM_FILE_SCALE = 1.25f / 0.8f;
  inline bool NpcFileMark( int i, int grp, float px, float py, float s ) {
    if( !g_icoTex || grp == NPC_GRP_TRADER || i < 0 || i >= g_npcIcoN || g_npcIcoN != g_npcCount ) return false;
    const int ei = g_npcIco[i];
    if( ei < 0 || ei >= g_icoL.n ) return false;
    const float sz = s * NPC_FILE_SCALE;
    const int img = MiniMapIcons::ImageFor( g_icoL, &g_icoL.e[ei], NpcIconLevel( sz ), g_icoOk );
    if( img < 0 ) return false;
    IconFileDraw( img, px, py, sz, IconFileColor( img, ColNpcGroup( grp ) ) );
    return true;
  }
  inline int IconItemCore( oCItem* it, char* inst, unsigned int& mf, unsigned int& fl ) {
    __try {
      inst[0] = 0;
      mf = (unsigned int)it->mainflag;
      fl = (unsigned int)it->flags;
      const int idx = it->instanz;
      if( parser && idx >= 0 ) {
        zCPar_Symbol* const s = parser->GetSymbol( idx );
        if( s )
          IconZRaw( s->name, inst, MiniMapIcons::KEY_LEN );
      }
      return 1;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return 0;
    }
  }
  inline int ItemFileImage( oCItem* it ) {
    if( !it || !g_icoL.n ) return -1;
    char inst[MiniMapIcons::KEY_LEN];
    unsigned int mf = 0, fl = 0;
    if( !IconItemCore( it, inst, mf, fl ) ) return -1;
    int cls = MiniMapIcons::ItemClassOf( inst, mf, fl );
    switch( ItemKindTxt( it ) ) {
      case IK_MELEE_1H: cls = MiniMapIcons::IC_MELEE_1H; break;
      case IK_MELEE_2H: cls = MiniMapIcons::IC_MELEE_2H; break;
      case IK_RANGED:   if( cls != MiniMapIcons::IC_CROSSBOW && cls != MiniMapIcons::IC_AMMO ) cls = MiniMapIcons::IC_BOW; break;
      case IK_ARMOR:    cls = MiniMapIcons::IC_ARMOR; break;
      case IK_POTION:   cls = MiniMapIcons::IC_POTION; break;
      case IK_FOOD:     if( cls != MiniMapIcons::IC_PLANT ) cls = MiniMapIcons::IC_FOOD; break;
      case IK_MAGIC:    if( cls != MiniMapIcons::IC_SCROLL && cls != MiniMapIcons::IC_RUNE ) cls = MiniMapIcons::IC_JEWELRY; break;
      case IK_DOCS:     cls = MiniMapIcons::IC_DOCS; break;
      case IK_GOLD:     cls = MiniMapIcons::IC_GOLD; break;
      case IK_OTHER:    cls = MiniMapIcons::IC_OTHER; break;
      default: break;
    }
    const MiniMapIcons::TEntry* e = MiniMapIcons::ResolveItem( g_icoL, inst, cls );
    if( !e || e->image < 0 || !g_icoOk[e->image] ) return -1;
    return e->image;
  }
  inline void ItemIconsBuild( oCItem* const* cache, int count ) {
    g_itemIcoN = 0;
    if( !g_itemIcons || !cache || count <= 0 ) return;
    IconPackLoad();
    if( !g_icoTex ) return;
    if( g_itemIcoCap < count ) {
      short* const a = MiniMapCommon::MmAlloc<short>( (size_t)( count + 64 ) );
      if( !a ) return;
      MiniMapCommon::MmFree( g_itemIco );
      g_itemIco    = a;
      g_itemIcoCap = count + 64;
    }
    for( int i = 0; i < count; i++ )
      g_itemIco[i] = (short)( cache[i] ? ItemFileImage( cache[i] ) : -1 );
    g_itemIcoN = count;
  }
  inline bool ItemFileMark( int i, int count, float px, float py, float s, const zCOLOR& c ) {
    if( !g_icoTex || !g_itemIcons || i < 0 || i >= g_itemIcoN || g_itemIcoN != count ) return false;
    const int img = g_itemIco[i];
    if( img < 0 || img >= g_icoL.nimg || !g_icoOk[img] ) return false;
    IconFileDraw( img, px, py, s, IconFileColor( img, c ) );
    return true;
  }
}
