#pragma once
namespace MiniMap {
  static const char* const CFG_DIR      = "MiniMap\\config";
  static const char* const CFG_MODS_DIR = "MiniMap\\mods";
  static const char* const CFG_SECTIONS =
    "colors traders hide names item_kinds place_words points terrain_colors textures icons frames "
    "text text_pl text_en text_de text_cs menu menu_pl menu_en menu_de menu_cs";
  static const char* const CFG_ICONS_DIR = "MiniMap\\icons";
  enum { CFG_FILES_MAX = 64, CFG_FILE_MAX_BYTES = 2 * 1024 * 1024 };
  static MiniMapCfg::TCfg g_cfg;
  static bool  g_cfgInit   = false;
  static char  g_modName[48] = "";
  static char  g_cfgFile[CFG_FILES_MAX][MAX_PATH];
  static int   g_cfgFiles  = 0;
  static int   g_cfgMsgs   = 0;
  static int   g_cfgGen    = 0;
  inline void CfgModName() {
    char raw[MAX_PATH];
    if( MiniMapCfg::GameParam( GetCommandLineA(), raw, MAX_PATH ) )
      MiniMapCfg::ModFolderName( raw, g_modName, (int)sizeof( g_modName ) );
    else
      g_modName[0] = 0;
    if( !g_modName[0] ) {
      MiniMapCfg::Copy( g_modName, (int)sizeof( g_modName ), zSwitch( "gothic", "gothic", "gothic2", "gothic2" ) );
    }
    for( char* p = g_modName; *p; p++ )
      if( *p >= 'A' && *p <= 'Z' ) *p = (char)( *p - 'A' + 'a' );
  }
  inline void CfgMsg( void*, int src, int line, const char* msg ) {
    if( !g_log ) return;
    if( ++g_cfgMsgs > 200 ) {
      if( g_cfgMsgs == 201 )
        MM_LOG( "config files: more than 200 messages - the rest is not logged" );
      return;
    }
    const char* nm = ( src >= 0 && src < g_cfgFiles ) ? g_cfgFile[src] : "MiniMap\\config";
    zSTRING s = zSTRING( "config files: " ) + zSTRING( nm );
    if( line > 0 ) s = s + " line " + zSTRING( line );
    MM_LOG( s + ": " + zSTRING( msg ) );
  }
  inline int CfgCmpStr( const void* a, const void* b ) {
    return _stricmp( (const char*)a, (const char*)b );
  }
  inline unsigned char* CfgReadFile( const char* path, int& len, int maxLen ) { return MiniMapCfg::ReadFile( path, len, maxLen ); }
  inline int CfgLoadDir( const char* dir ) {
    char mask[MAX_PATH];
    if( !FilePath( mask, MAX_PATH, dir, "*.txt" ) ) return 0;
    enum { MAXN = 64 };
    static char names[MAXN][MAX_PATH];
    int n = 0;
    WIN32_FIND_DATAA fd;
    HANDLE fh = FindFirstFileA( mask, &fd );
    if( fh == INVALID_HANDLE_VALUE ) return 0;
    do {
      if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) continue;
      const int fl = (int)strlen( fd.cFileName );
      if( fl < 5 || _stricmp( fd.cFileName + fl - 4, ".txt" ) || !_stricmp( fd.cFileName, "README.txt" )
          || !_stricmp( fd.cFileName, "README_old.txt" ) )
        continue;
      if( n < MAXN )
        MiniMapCfg::Copy( names[n++], MAX_PATH, fd.cFileName );
    } while( FindNextFileA( fh, &fd ) );
    FindClose( fh );
    qsort( names, n, MAX_PATH, CfgCmpStr );
    int done = 0;
    for( int i = 0; i < n; i++ ) {
      char full[MAX_PATH];
      if( !FilePath( full, MAX_PATH, dir, names[i] ) ) continue;
      if( g_cfgFiles >= CFG_FILES_MAX ) {
        if( g_cfgFiles == CFG_FILES_MAX )
          MM_LOG( "config files: more than 64 files - the rest is ignored" );
        g_cfgFiles++;
        break;
      }
      const int idx = g_cfgFiles++;
      MiniMapCfg::Copy( g_cfgFile[idx], MAX_PATH, full );
      int len = 0;
      unsigned char* txt = CfgReadFile( full, len, CFG_FILE_MAX_BYTES );
      if( !txt ) {
        CfgMsg( 0, idx, 0, len == -2 ? "file is larger than 2 MB - skipped" : "cannot read the file" );
        continue;
      }
      MiniMapCfg::Parse( g_cfg, (const char*)txt, len, idx, CfgMsg, 0 );
      MiniMapCommon::MmFree( txt );
      done++;
    }
    return done;
  }
  static const char* const CFG_README_FILE = "MiniMap\\readme.txt";
  static const char* const CFG_README =
    "MiniMap 2.0a - readme\r\n"
    "=====================\r\n"
    "\r\n"
    "Minimap and world map for Gothic and Gothic II (also for many mods).\r\n"
    "All settings are in the game menu (MiniMap). You do not need the files\r\n"
    "below to play.\r\n"
    "\r\n"
    "FOLDERS\r\n"
    "  examples\\      ready-made settings files - start with examples\\readme.txt\r\n"
    "  config\\        your settings files for every game (empty at first)\r\n"
    "  mods\\<mod>\\    your settings files for one mod, and lists of names\r\n"
    "                 from that mod (auto_*.txt)\r\n"
    "  icons\\custom\\  appears when you choose Icon look = Own in the menu.\r\n"
    "                 Replace any picture there with your own (same name).\r\n"
    "  icons\\marks\\   pictures and names of your map markers 1-10 (marks.txt)\r\n"
    "  icons\\default\\ simple\\, textured\\, bare\\ pictures and lists (do not edit)\r\n"
    "  export\\        pictures saved with the menu option Export map\r\n"
    "  engine\\, maps\\, details\\, fog\\  used by MiniMap (do not edit)\r\n"
    "\r\n"
    "FILES\r\n"
    "  MiniMap.marks  your markers and notes on the map - keep it\r\n"
    "  MiniMap.log    what happened in the last game (errors in your files too)\r\n"
    "\r\n"
    "CREDITS\r\n"
    "  MiniMap: PEN996.\r\n"
    "  Wisp picture: game-icons.net by Lorc (CC BY 3.0,\r\n"
    "  https://creativecommons.org/licenses/by/3.0/), placed in a badge,\r\n"
    "  recolored and resized. Framed / No frame pictures: rendered from the\r\n"
    "  3D models of the games and drawn by MiniMap.\r\n";
  inline void CfgReadmeOld( const char* dir, const char* head ) {
    char path[MAX_PATH], old[MAX_PATH];
    if( !FilePath( path, MAX_PATH, dir, "README.txt" ) || !FilePath( old, MAX_PATH, dir, "README_old.txt" ) ) return;
    if( GetFileAttributesA( path ) == INVALID_FILE_ATTRIBUTES ) return;
    int len = 0;
    unsigned char* cur = CfgReadFile( path, len, 64 * 1024 );
    const int hl = (int)strlen( head );
    const bool ours = cur && len >= hl && !memcmp( cur, head, hl );
    MiniMapCommon::MmFree( cur );
    if( ours )
      MoveFileA( path, old );
  }
  inline void CfgReadmeEnsure() {
    CfgReadmeOld( CFG_DIR, "MiniMap config files - README" );
    CfgReadmeOld( CFG_ICONS_DIR, "MiniMap - creature and item icons" );
    const int want = (int)strlen( CFG_README );
    int len = 0;
    unsigned char* cur = CfgReadFile( CFG_README_FILE, len, 64 * 1024 );
    const bool same = cur && len == want && !memcmp( cur, CFG_README, want );
    MiniMapCommon::MmFree( cur );
    CreateDirectoryA( FILES_DIR, 0 );
    CreateDirectoryA( CFG_DIR, 0 );
    CreateDirectoryA( CFG_MODS_DIR, 0 );
    if( same ) return;
    HANDLE h = CreateFileA( CFG_README_FILE, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) return;
    DWORD wr = 0;
    WriteFile( h, CFG_README, (DWORD)want, &wr, 0 );
    CloseHandle( h );
  }
  static const char* const CFG_EXAMPLES_DIR = "MiniMap\\examples";
  inline unsigned long ExampleCrc( const char* s ) {
    return MiniMapImage::Crc32( (const unsigned char*)s, (int)strlen( s ) );
  }
  inline int ExampleLine( char* out, int cap, const char* name, unsigned long crc ) {
    if( !out || cap <= 0 ) return 0;
    const int n = snprintf( out, (size_t)cap, "%s %08lX", name ? name : "", crc );
    return ( n > 0 && n < cap ) ? n : (int)strlen( out );
  }
  static int g_exDone = -1;
  inline void ExamplesEnsure() {
    if( g_exDone == EXAMPLES_VERSION ) return;
    g_exDone = EXAMPLES_VERSION;
    CreateDirectoryA( FILES_DIR, 0 );
    CreateDirectoryA( CFG_EXAMPLES_DIR, 0 );
    char ver[MAX_PATH];
    if( !FilePath( ver, MAX_PATH, CFG_EXAMPLES_DIR, "written.ver" ) ) return;
    int vlen = 0;
    unsigned char* vt = CfgReadFile( ver, vlen, 16 * 1024 );
    int wrote = 0, kept = 0;
    char out[2048];
    int on = 0;
    out[on++] = (char)( '0' + EXAMPLES_VERSION % 10 );
    out[on++] = '\r'; out[on++] = '\n';
    for( int i = 0; i < kExamplesN; i++ ) {
      char path[MAX_PATH];
      if( !FilePath( path, MAX_PATH, CFG_EXAMPLES_DIR, kExamples[i].name ) ) continue;
      const unsigned long want = ExampleCrc( kExamples[i].text );
      int len = 0;
      unsigned char* cur = CfgReadFile( path, len, 256 * 1024 );
      bool write = !cur;
      if( cur ) {
        const unsigned long have = MiniMapImage::Crc32( cur, len );
        char key[96];
        ExampleLine( key, (int)sizeof( key ), kExamples[i].name, have );
        const bool ours = vt && strstr( (const char*)vt, key ) != 0;
        write = ( have != want ) && ours;
        if( have != want && !ours ) kept++;
      }
      MiniMapCommon::MmFree( cur );
      if( write ) {
        HANDLE h = CreateFileA( path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
        if( h != INVALID_HANDLE_VALUE ) {
          DWORD wr = 0;
          WriteFile( h, kExamples[i].text, (DWORD)strlen( kExamples[i].text ), &wr, 0 );
          CloseHandle( h );
          wrote++;
        }
      }
      if( on < (int)sizeof( out ) - 100 ) {
        on += ExampleLine( out + on, (int)sizeof( out ) - on - 2, kExamples[i].name, want );
        out[on++] = '\r'; out[on++] = '\n';
      }
    }
    MiniMapCommon::MmFree( vt );
    HANDLE h = CreateFileA( ver, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, 0 );
    if( h != INVALID_HANDLE_VALUE ) {
      DWORD wr = 0;
      WriteFile( h, out, (DWORD)on, &wr, 0 );
      CloseHandle( h );
    }
    if( wrote || kept )
      MM_LOG( zSTRING( "examples: wrote " ) + zSTRING( wrote ) + " file(s) to MiniMap\\examples, kept " + zSTRING( kept ) + " changed by you" );
  }
  inline void ColorsApply();
  inline void TerrainColorsApply();
  inline void TexturesApply();
  inline void CfgLoad() {
    if( !g_cfgInit ) {
      MiniMapCfg::Init( g_cfg, CFG_SECTIONS );
      g_cfgInit = true;
    }
    MiniMapCfg::Free( g_cfg );
    g_cfgFiles = 0;
    g_cfgMsgs  = 0;
    g_cfgGen++;
    if( !g_modFiles ) {
      MM_LOG( "config files: off (ModFiles=0 in Gothic.ini)" );
      ColorsApply();
      TerrainColorsApply();
      TexturesApply();
      return;
    }
    CfgModName();
    CfgReadmeEnsure();
    ExamplesEnsure();
    LARGE_INTEGER t0, t1, f;
    QueryPerformanceCounter( &t0 );
    const int a = CfgLoadDir( CFG_DIR );
    char modDir[MAX_PATH];
    int b = 0;
    if( FilePath( modDir, MAX_PATH, CFG_MODS_DIR, g_modName ) ) {
      CreateDirectoryA( modDir, 0 );
      b = CfgLoadDir( modDir );
    }
    QueryPerformanceCounter( &t1 );
    QueryPerformanceFrequency( &f );
    const int ms = f.QuadPart ? (int)( ( t1.QuadPart - t0.QuadPart ) * 1000 / f.QuadPart ) : 0;
    if( a + b > 0 || g_cfg.errors > 0 )
      MM_LOG( zSTRING( "config files: mod '" ) + zSTRING( g_modName ) + "', files " + zSTRING( a ) + " in Config + " + zSTRING( b ) + " in Mods\\" + zSTRING( g_modName ) + ", entries " + zSTRING( g_cfg.n ) + ", errors " + zSTRING( g_cfg.errors ) + ", " + zSTRING( ms ) + " ms" );
    else
      MM_LOG( zSTRING( "config files: none (mod '" ) + zSTRING( g_modName ) + "')" );
    ColorsApply();
    TerrainColorsApply();
    TexturesApply();
  }
  inline void CfgFree() {
    if( g_cfgInit )
      MiniMapCfg::Free( g_cfg );
    g_cfgFiles = 0;
  }
  enum {
    COLK_PLAYER, COLK_ENEMY, COLK_FRIEND, COLK_NEUTRAL, COLK_TRADER, COLK_CORPSE,
    COLK_ITEM, COLK_ITEM_1H, COLK_ITEM_2H, COLK_ITEM_RANGED, COLK_ITEM_ARMOR,
    COLK_ITEM_POTION, COLK_ITEM_PLANT, COLK_ITEM_MAGIC,
    COLK_CHEST_OPEN, COLK_CHEST_KEY, COLK_CHEST_LOCKPICK, COLK_CHEST_BOTH, COLK_CHEST_SEALED,
    COLK_SWITCH, COLK_SWITCH_TARGET,
    COLK_SHRINE, COLK_BOARD, COLK_FORGE, COLK_ORE, COLK_OTHER_PLACE, COLK_ALCHEMY,
    COLK_KITCHEN, COLK_WORKSHOP, COLK_TREASURE, COLK_EXIT,
    COLK_RUNE, COLK_SCROLL, COLK_DEMON,
    COLK_COUNT
  };
  static const char* const kColKey[COLK_COUNT] = {
    "player", "enemy", "friend", "neutral", "trader", "corpse",
    "item", "item_1h", "item_2h", "item_ranged", "item_armor",
    "item_potion", "item_plant", "item_magic",
    "chest_open", "chest_key", "chest_lockpick", "chest_both", "chest_sealed",
    "switch", "switch_target",
    "shrine", "board", "forge", "ore", "other_place", "alchemy",
    "kitchen", "workshop", "treasure", "exit",
    "rune", "scroll", "demon"
  };
  static zCOLOR g_colOv[COLK_COUNT];
  static bool   g_colOn[COLK_COUNT] = {};
  static int    g_colSet = 0;
  inline int ColKeyIndex( const char* key ) {
    for( int k = 0; k < COLK_COUNT; k++ )
      if( MiniMapCfg::EqNoCase( kColKey[k], key ) ) return k;
    return -1;
  }
  inline void ColorsApply() {
    memset( g_colOn, 0, sizeof( g_colOn ) );
    g_colSet = 0;
    const int sec = g_modFiles ? MiniMapCfg::SecFind( g_cfg, "colors" ) : -1;
    int bad = 0;
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      const int k = ColKeyIndex( t.key );
      if( k < 0 ) {
        CfgMsg( 0, t.src, t.line, "[colors]: unknown name (list: examples\\01_colors.txt) - ignored" );
        bad++;
        continue;
      }
      unsigned char c[4];
      if( !MiniMapCfg::Color( t.val, c ) ) {
        CfgMsg( 0, t.src, t.line, "[colors]: write the color as R, G, B (0-255), e.g. 230, 60, 50 - ignored" );
        bad++;
        continue;
      }
      g_colOv[k].r     = c[0];
      g_colOv[k].g     = c[1];
      g_colOv[k].b     = c[2];
      g_colOv[k].alpha = 255;
      if( !g_colOn[k] )
        g_colSet++;
      g_colOn[k] = true;
    }
    if( g_colSet || bad )
      MM_LOG( zSTRING( "colors: from txt files " ) + zSTRING( g_colSet ) + ", bad lines " + zSTRING( bad ) );
  }
  enum { TC_COUNT = 14 };
  static const char* const kTerrainKey[TC_COUNT] = {
    "water", "snow", "roof", "stonework", "building", "field", "path",
    "grass", "forest", "rock", "plant", "swamp", "beach", "cave"
  };
  static unsigned char g_tcRGB[TC_COUNT][3];
  static bool          g_tcOn[TC_COUNT] = {};
  static unsigned int  g_tcSig = 0;
  inline void TerrainColorsApply() {
    memset( g_tcOn, 0, sizeof( g_tcOn ) );
    g_tcSig = 0;
    const int sec = g_modFiles ? MiniMapCfg::SecFind( g_cfg, "terrain_colors" ) : -1;
    int set = 0, bad = 0;
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      int k = -1;
      for( int j = 0; j < TC_COUNT && k < 0; j++ )
        if( MiniMapCfg::EqNoCase( kTerrainKey[j], t.key ) ) k = j;
      if( k < 0 ) {
        CfgMsg( 0, t.src, t.line, "[terrain_colors]: unknown name (water snow roof stonework building field path grass forest rock plant swamp beach cave) - ignored" );
        bad++;
        continue;
      }
      unsigned char c[4];
      if( !MiniMapCfg::Color( t.val, c ) ) {
        CfgMsg( 0, t.src, t.line, "[terrain_colors]: write the color as R, G, B (0-255) - ignored" );
        bad++;
        continue;
      }
      g_tcRGB[k][0] = c[0];
      g_tcRGB[k][1] = c[1];
      g_tcRGB[k][2] = c[2];
      if( !g_tcOn[k] ) set++;
      g_tcOn[k] = true;
    }
    if( set ) {
      unsigned int h = 2166136261u ^ 0x7C0F0000u;
      for( int k = 0; k < TC_COUNT; k++ ) {
        h ^= g_tcOn[k] ? ( 0x01000000u | ( (unsigned int)g_tcRGB[k][0] << 16 )
                                       | ( (unsigned int)g_tcRGB[k][1] << 8 ) | g_tcRGB[k][2] ) : 0u;
        h *= 16777619u;
      }
      g_tcSig = h ? h : 1u;
    }
    if( set || bad )
      MM_LOG( zSTRING( "terrain colors: from txt files " ) + zSTRING( set ) + ", bad lines " + zSTRING( bad ) + zSTRING( set ? " - the map is calculated again once" : "" ) );
  }
  inline bool TerrainColor( int k, float& r, float& g, float& b ) {
    if( k < 0 || k >= TC_COUNT || !g_tcOn[k] ) return false;
    r = (float)g_tcRGB[k][0];
    g = (float)g_tcRGB[k][1];
    b = (float)g_tcRGB[k][2];
    return true;
  }
  enum { TX_MAX = 512, TX_LEN = 64, TX_TERRAIN = 14 };
  static char          g_txPat[TX_MAX][TX_LEN];
  static unsigned char g_txCls[TX_MAX];
  static volatile int  g_txCount = 0;
  static unsigned int  g_txSig   = 0;
  inline void TexturesApply() {
    const int sec = g_modFiles ? MiniMapCfg::SecFind( g_cfg, "textures" ) : -1;
    int n = 0, bad = 0, over = 0;
    unsigned int h = 2166136261u ^ 0x7E7E0000u;
    g_txCount = 0;
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      if( MiniMapCfg::PatPrefix( t.key ) < 0 || (int)strlen( t.key ) >= TX_LEN ) {
        CfgMsg( 0, t.src, t.line, "[textures]: bad texture name (NAME or PREFIX* with 3+ letters, max 63) - ignored" );
        bad++;
        continue;
      }
      int k = -1;
      if( MiniMapCfg::EqNoCase( t.val, "terrain" ) ) k = TX_TERRAIN;
      for( int j = 0; j < TC_COUNT && k < 0; j++ )
        if( MiniMapCfg::EqNoCase( kTerrainKey[j], t.val ) ) k = j;
      if( k < 0 ) {
        CfgMsg( 0, t.src, t.line, "[textures]: unknown class (terrain water snow roof stonework building field path grass forest rock plant swamp beach cave) - ignored" );
        bad++;
        continue;
      }
      if( n >= TX_MAX ) { over++; continue; }
      char* d = g_txPat[n];
      int c = 0;
      for( ; t.key[c] && c < TX_LEN - 1; c++ )
        d[c] = t.key[c];
      d[c] = 0;
      d[TX_LEN - 1] = 0;
      g_txCls[n] = (unsigned char)k;
      for( int q = 0; d[q]; q++ ) {
        h ^= (unsigned char)MiniMapCfg::Up( d[q] );
        h *= 16777619u;
      }
      h ^= 0x100u | (unsigned int)k;
      h *= 16777619u;
      n++;
    }
    if( over )
      CfgMsg( 0, 0, 0, "[textures]: more than 512 entries - the rest ignored" );
    g_txSig = n ? ( h ? h : 1u ) : 0u;
    g_txCount = n;
    if( n || bad )
      MM_LOG( zSTRING( "textures: from txt files " ) + zSTRING( n ) + ", bad lines " + zSTRING( bad ) + zSTRING( n ? " - the map is calculated again once" : "" ) );
  }
  inline int TextureClassTxt( const char* name ) {
    const int n = g_txCount;
    if( n <= 0 || !name || !name[0] ) return -1;
    int best = -1, bestLen = -1;
    for( int i = 0; i < n && i < TX_MAX; i++ ) {
      const char* p = g_txPat[i];
      if( !MiniMapCfg::PatMatch( p, name ) ) continue;
      const int pre = MiniMapCfg::PatPrefix( p );
      const int len = ( pre == 0 ) ? 1000 : pre;
      if( len >= bestLen ) { bestLen = len; best = g_txCls[i]; }
    }
    return best;
  }
  static const char* const kLangSuffix[LANG_COUNT] = { "pl", "en", "de", "cs" };
  static int g_ltGen = -1, g_ltLang = -1;
  inline int LangTxtKey( const char* k ) {
    for( int i = 0; i < TX_COUNT; i++ )
      if( MiniMapCfg::EqNoCase( TEXT_KEY[i], k ) ) return i;
    return -1;
  }
  inline int LangMenuKey( const char* k, int* part ) {
    char base[MiniMapCfg::KEY_LEN];
    MiniMapCfg::Copy( base, (int)sizeof( base ), k );
    *part = 0;
    const int n = (int)strlen( base );
    if( n > 5 && MiniMapCfg::EqNoCase( base + n - 5, ".INFO" ) ) {
      base[n - 5] = 0;
      *part = 1;
    }
    for( int i = 0; i < MENU_TR_COUNT; i++ )
      if( MiniMapCfg::EqNoCase( MENU_TR[i].item, base ) ) return i;
    return -1;
  }
  inline void LangTxtSection( const char* sec, int& nText, int& nMenu, int& bad ) {
    const int s = MiniMapCfg::SecFind( g_cfg, sec );
    for( int i = MiniMapCfg::Next( g_cfg, s, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, s, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      if( !t.val[0] ) continue;
      if( sec[0] == 't' ) {
        const int id = LangTxtKey( t.key );
        if( id < 0 ) { CfgMsg( 0, t.src, t.line, "[text]: unknown text name (list: mods\\<mod>\\auto_texts_*.txt) - ignored" ); bad++; continue; }
        if( !g_textTxt[id] ) nText++;
        g_textTxt[id] = t.val;
      }
      else {
        int part = 0;
        const int mi = LangMenuKey( t.key, &part );
        if( mi < 0 ) { CfgMsg( 0, t.src, t.line, "[menu]: no menu entry with this name - ignored" ); bad++; continue; }
        if( MenuLocked( mi ) ) { CfgMsg( 0, t.src, t.line, "[menu]: author and version cannot be changed - ignored" ); bad++; continue; }
        if( !g_menuTxt[mi][part] ) nMenu++;
        g_menuTxt[mi][part] = t.val;
      }
    }
  }
  inline void LangTxtEnsure() {
    const int want = ( g_lang >= 0 && g_lang < LANG_COUNT ) ? g_lang : LANG_PL;
    if( g_ltGen == g_cfgGen && g_ltLang == want ) return;
    g_ltGen  = g_cfgGen;
    g_ltLang = want;
    memset( g_textTxt, 0, (size_t)( TX_COUNT ) * sizeof( g_textTxt[0] ) );
    for( int i = 0; i < MENU_TR_COUNT; i++ ) g_menuTxt[i][0] = g_menuTxt[i][1] = 0;
    g_textTxtN = 0;
    g_menuTxtGen++;
    if( !g_modFiles || !g_cfgInit ) return;
    int nText = 0, nMenu = 0, bad = 0;
    char sec[16];
    LangTxtSection( "text", nText, nMenu, bad );
    snprintf( sec, sizeof( sec ), "text_%s", kLangSuffix[want] );
    LangTxtSection( sec, nText, nMenu, bad );
    LangTxtSection( "menu", nText, nMenu, bad );
    snprintf( sec, sizeof( sec ), "menu_%s", kLangSuffix[want] );
    LangTxtSection( sec, nText, nMenu, bad );
    g_textTxtN = nText;
    if( nText || nMenu || bad )
      MM_LOG( zSTRING( "texts: from txt files " ) + zSTRING( nText ) + " texts, " + zSTRING( nMenu ) + " menu lines (language " + zSTRING( LANG_NAME[want] ) + "), bad lines " + zSTRING( bad ) );
  }
  enum { PT_MAX = 2048 };
  static MiniMapCfg::TPoint* g_pts    = 0;
  static int                 g_ptsN   = 0;
  static int                 g_ptsAll = 0;
  inline void PointsFree() {
    MiniMapCommon::MmFree( g_pts );
    g_pts  = 0;
    g_ptsN = 0;
  }
  inline void PointsForWorld( const char* world ) {
    PointsFree();
    g_ptsAll = 0;
    const int sec = g_modFiles ? MiniMapCfg::SecFind( g_cfg, "points" ) : -1;
    if( sec < 0 || !world || !world[0] ) return;
    int mine = 0, bad = 0;
    MiniMapCfg::TPoint p;
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const char* err = 0;
      if( !MiniMapCfg::ParsePoint( g_cfg.e[i].val, p, &err ) ) {
        CfgMsg( 0, g_cfg.e[i].src, g_cfg.e[i].line, err ? err : "bad point" );
        bad++;
        continue;
      }
      g_ptsAll++;
      if( MiniMapCfg::EqNoCase( p.world, world ) )
        mine++;
    }
    if( mine > PT_MAX ) mine = PT_MAX;
    if( mine > 0 ) g_pts = MiniMapCommon::MmAlloc<MiniMapCfg::TPoint>( (size_t)( mine ) );
    if( g_pts ) {
      for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0 && g_ptsN < mine; i = MiniMapCfg::Next( g_cfg, sec, i ) )
        if( MiniMapCfg::ParsePoint( g_cfg.e[i].val, p, 0 ) && MiniMapCfg::EqNoCase( p.world, world ) )
          g_pts[g_ptsN++] = p;
    }
    if( g_ptsAll || bad )
      MM_LOG( zSTRING( "points: from txt files " ) + zSTRING( g_ptsAll ) + " (in this world " + zSTRING( g_ptsN ) + "), bad lines " + zSTRING( bad ) );
  }
  inline const MiniMapCfg::TItem* CfgFind( const char* section, const char* key ) {
    if( !g_cfgInit || !g_modFiles ) return 0;
    return MiniMapCfg::Find( g_cfg, section, key );
  }
  inline int CfgBool( const char* section, const char* key ) {
    const MiniMapCfg::TItem* t = CfgFind( section, key );
    if( !t ) return -1;
    const int b = MiniMapCfg::Bool( t->val );
    if( b < 0 )
      CfgMsg( 0, t->src, t->line, "value must be yes or no" );
    return b;
  }
}
