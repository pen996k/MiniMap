#pragma once
namespace MiniMap {
  using namespace MiniMapCommon;
  inline void StyleFree( bool release );
  inline void DrawTopProgressBanner( const char* line, const char* line2, bool showBar, float f );
  const unsigned int ADDR_ViewBlit         = reinterpret_cast<unsigned int>( SIGNATURE_OF( &zCView::Blit ) );
  const unsigned int ADDR_ShowVobDebugInfo = zSwitch( 0x006D2BC0u, 0x00709C80u, 0x0071BD30u, 0x0077B770u );
  const unsigned int ADDR_GetMousePosW32   = reinterpret_cast<unsigned int>( SIGNATURE_OF( &zCInput_Win32::GetMousePos ) );
  const unsigned int ADDR_KeyPressedW32    = reinterpret_cast<unsigned int>( SIGNATURE_OF( &zCInput_Win32::KeyPressed ) );
  const unsigned int ADDR_KeyToggledW32    = reinterpret_cast<unsigned int>( SIGNATURE_OF( &zCInput_Win32::KeyToggled ) );
  static int g_enabled     = 1;
  static int g_anchor      = ANCHOR_TOPRIGHT;
  static int g_corner      = CORNER_TOPRIGHT;
  static int g_margin      = 16;
  static int g_posX        = 0;
  static int g_posY        = 0;
  static int g_size        = 200;
  static int g_sizeY       = 0;
  static int g_sizeMin     = 96;
  static int g_sizeMax     = 440;
  static int g_sizeStep    = 24;
  static int g_zoom        = 6000;
  static int g_zoomMin     = 1200;
  static int g_zoomMax     = 40000;
  static int g_zoomStep    = 125;
  static int g_alpha       = 255;
  static int g_bright      = 255;
  static int g_backLevel   = 14;
  static int g_border      = 1;
  static int g_showPlayer  = 1;
  static int g_playerRing  = 1;
  static int g_playerSize  = 0;
  static int g_playerEdge  = 1;
  static int g_arrowSize   = 0;
  static int g_iconScale   = 1;
  static int g_sizeNpc     = 3;
  static int g_sizeItem    = 3;
  static int g_sizeMark    = 3;
  static int g_iconZoom    = 1;
  static int g_npcIcons    = 2;
  static int g_iconSet     = 1;
static int g_iconStyle   = 1;
static int g_colMenu[40] = {};
  static int g_itemIcons   = 1;
  static int g_modFiles    = 1;
  static int g_modAuto     = 1;
  static int g_genSize     = 8192;
  static int g_genSizeUsed = 0;
  static int g_genOomSize = 0;
  static int g_genDetail    = 0;
  static int g_genPatchSize = 2048;
  static int g_genCache    = 1;
  static int g_genPack     = 4;
  static int g_genDetailCacheMB = 1024;
  static int g_genIndoor   = 1;
  static int g_genIndoorLvl   = 2;
  static int g_genIndoorAlpha = 100;
  static int g_genVivid    = 108;
  static int g_genBright   = 100;
  static int g_genRelief   = 170;
  static int g_genEdge     = 18;
  static int g_genVobs     = 1;
  static int g_genVobMin   = 300;
  static int g_genPlantMin = 110;
  static int g_genFill     = 1;
  static int g_genOutside  = 0;
  static int g_genSmooth   = 0;
  static int g_genPaths    = GEN_PATHS_DEF;
  static int g_genPathWide = 480;
  static int g_genPathsWater = 1;
  static int g_genContour  = 500;
  static int g_genContourDark = 12;
  static int g_genSculpt   = 1;
  static int g_genTexColors = 1;
  static int g_genTreeShapes = 3;
  static int g_showNorth   = 1;
  static int g_showDetailBar = 0;
  static const double kDetailSpeedMs[5] = { 5.0, 15.0, 40.0, 100.0, 250.0 };
  const int GEN_MAX_THREADS = 16;
  static double g_dpFrameMs = 5.0;
  static const int kDqRays[4]      = { 30000, 120000, 400000, 1000000 };
  static const int kDqVobPct[4]    = { 100, 70, 50, 35 };
  static const int kDqEdgeAdd[4]   = { 0, 6, 12, 18 };
  static const int kDqReliefPct[4] = { 100, 115, 130, 145 };
  static int g_detailQuality = 0;
  inline float DqVobScale() {
    return (float)kDqVobPct[( g_detailQuality >= 0 && g_detailQuality <= 3 ) ? g_detailQuality : 0] / 100.0f;
  }
  static int g_showLabel   = 1;
  static int g_showScale     = 1;
  static int g_scaleFrames   = 0;
  static int g_showNpc     = 0;
  static int g_showNpcKind = 0;
  static int g_npcRange    = 5000;
  static int g_npcLimit    = 120;
  static int g_showWaynet  = 0;
  static int g_rememberMaps = 1;
  static int g_hideInInv   = 1;
  static int g_hideInModal = 1;
  static int g_hideInStatus = 0;
  static int g_startHidden = 0;
  static int g_log         = 1;
  static int g_shape       = 0;
  static int g_rotate      = 0;
  static int g_rotSmooth   = 25;
  static int g_mapCells    = 0;
  static int g_showItems   = 0;
  static int g_itemRange   = 3000;
  static int g_itemLimit   = 60;
  static int g_itemColors  = 1;
  static int g_showChests  = 2;
  static int g_chestCombo  = 0;
  static int g_hideEmpty   = 0;
  static int g_itemList    = 0;
  static int g_showTeleports = 1;
  static int g_fog       = 0;
  static int g_exportZoom   = 0;
  static int g_exportDetail = 1;
  static int g_exportFormat  = 2;
  static int g_exportQuality = 2;
  static int g_expChests = 1, g_expItems = 1, g_expPlants = 1, g_expNpcs = 1, g_expFoes = 0;
  static int g_expFriends = 1;
  static int g_expSwitches = 1, g_expPlaces = 1, g_expExits = 1, g_expTeleports = 1;
  static int g_expMarksOn = 1, g_expPlayer = 1;
  static int g_fogRadius = 3000;
  static int g_fogAlpha  = 100;
  static int g_showSwitches = 2;
  static int g_showPlaces = 2;
  static int g_showFurniture = 0;
  static int g_showCraft = 1;
  static int g_showAlchemy  = 1;
  static int g_showRunes    = 1;
  static int g_showScrolls  = 1;
  static int g_showDemon    = 1;
  static int g_showKitchen  = 1;
  static int g_showWorkshop = 1;
  static int g_showLadders  = 1;
  static int g_showFire     = 0;
  static int g_showClutter  = 0;
  static int g_placeLearn = 1;
  static int g_showExits  = 2;
  const int EXIT_MAX = 64;
  struct TExit { zCVob* vob; float x, z; char dest[40]; };
  inline const TExit* ExitOf( zCVob* v );
  inline void ExitFree();
  inline void WtpFree();
  inline int  WtpHoverLines( zCVob* vob, zSTRING* lines, int max );
  inline bool WtpIsPlatform( zCVob* v );
  inline zCOLOR ColTeleport();
  enum { SHOW_NONE = 0, SHOW_SIGHT = 1, SHOW_WORLD = 2 };
  static int g_sightRays         = 1;
  static int g_sightRange        = 0;
  static int g_sightNear         = 600;
  static int g_sightRaysPerFrame = 12;
  static int g_rayBudget         = 0;
  static int g_rayBudgetNpc      = 0;
  static int g_rayBudgetItem     = 0;
  static int g_keyToggle   = 0;
  static int g_keyZoomIn   = 0;
  static int g_keyZoomOut  = 0;
  static int g_keySizeUp   = 0;
  static int g_keySizeDown = 0;
  static int g_keyRotate   = 0;
  static int g_keyFull     = 0;
  static int g_keySearch   = 0;
  static int g_keyScreen   = 0;
  static int g_keyItemList = 0;
  static bool g_ilShown    = false;
  static int g_searchOthers = 0;
  static int g_showTraders  = 2;
  static int g_showCorpses  = 1;
  static int g_hoverInv     = 0;
  static int g_searchInv    = 0;
  static int  g_align      = -1;
  static int  g_alignAuto  = 0;
  static bool g_alignKnown = false;
  static bool g_mirror     = false;
  static int  g_fullAlign  = -1;
  static int  g_keyAlign   = 0;
  static int  g_useFullMap  = 1;
  static int  g_wheelZoom   = 1;
  static int  g_wheelMarkIcon = 1;
  static int  g_wheelFull = 2;
  static bool g_full       = false;
  static int  g_frameNo      = 0;
  static int  g_lateFrame    = -100;
  static int  g_pendingFrame = -100;
  static bool g_lateLogged   = false;
  static int  g_fullPct    = 100;
  static int  g_fullMargin = 40;
  static int  g_fullFit    = 1;
  static int  g_miniStyle   = 0;
  static int  g_fullStyle   = 0;
  static int  g_fullSizeIdx = 4;
  static int  g_fullScreen  = 0;
  static int  g_fullStart  = 75;
  static float g_fullCamX   = 0.0f;
  static float g_fullCamZ   = 0.0f;
  static bool  g_fullCamSet = false;
  static bool  g_fullMapped = false;
  static int   g_fullEdgePan  = 40;
  static int   g_fullEdgeZone = 24;
  static int   g_fullEdgeScroll = 0;
  static int   g_mapDragOn = 1;
  static int  g_fullLockCam = 1;
  static int  g_fullBlockKeys = 1;
  static int  g_fullPause   = 1;
  static int  g_markGuide   = 0;
  static bool g_pausedByUs  = false;
  static int  g_fullCenter = 1;
  static float g_walkX1 = 0.0f, g_walkZ1 = 0.0f;
  static float g_walkX2 = 0.0f, g_walkZ2 = 0.0f;
  static bool  g_walkReady = false;
  static bool  g_walkTried = false;
  static float g_landX1 = 0.0f, g_landZ1 = 0.0f;
  static float g_landX2 = 0.0f, g_landZ2 = 0.0f;
  static bool  g_landReady = false;
  struct TLandRect {
    char  world[NAME_MAX];
    float x1, z1, x2, z2;
    unsigned int tag;
  };
  const int   LAND_SLOTS = MAP_SLOTS;
  static TLandRect g_landSaved[LAND_SLOTS];
  static int       g_landSavedCount = 0;
  static char g_fontName[NAME_MAX]  = "FONT_OLD_10_WHITE.TGA";
  static char g_playerTex[NAME_MAX] = "";
  static bool g_configLoaded = false;
  static bool g_visible      = true;
  static bool g_viewTouched  = false;
  const char* const FILES_DIR     = "MiniMap";
  const char* const FILES_GEN_DIR = "MiniMap\\maps";
  const char* const FILES_DET_DIR = "MiniMap\\details";
  const char* const LOG_FILE      = "MiniMap\\MiniMap.log";
  const char* const LOG_PREV_FILE = "MiniMap\\MiniMap_previous.log";
  static bool g_filesReady = false;
  static int  g_filesMoved = 0, g_filesKept = 0;
  inline bool FilePath( char* out, int max, const char* dir, const char* name ) {
    if( !out || max <= 0 ) return false;
    const bool hasDir = dir && dir[0];
    const int n = snprintf( out, (size_t)max, "%s%s%s", hasDir ? dir : "", hasDir ? "\\" : "", name ? name : "" );
    return n >= 0 && n < max;
  }
  inline void FilesMoveOne( const char* from, const char* to ) {
    if( GetFileAttributesA( from ) == INVALID_FILE_ATTRIBUTES ) return;
    if( MoveFileA( from, to ) )
      g_filesMoved++;
    else
      g_filesKept++;
  }
  inline void FilesMoveMask( const char* mask, const char* srcDir, const char* dir ) {
    WIN32_FIND_DATAA fd;
    HANDLE fh = FindFirstFileA( mask, &fd );
    if( fh == INVALID_HANDLE_VALUE ) return;
    do {
      if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) continue;
      char from[260], to[260];
      if( FilePath( from, 260, srcDir, fd.cFileName ) && FilePath( to, 260, dir, fd.cFileName ) )
        FilesMoveOne( from, to );
    } while( FindNextFileA( fh, &fd ) );
    FindClose( fh );
  }
  inline void FilesDirs() {
    CreateDirectoryA( FILES_DIR, 0 );
    CreateDirectoryA( FILES_GEN_DIR, 0 );
    CreateDirectoryA( FILES_DET_DIR, 0 );
  }
  inline HANDLE FileCreate( const char* file, DWORD access, DWORD share, DWORD disp ) {
    HANDLE h = CreateFileA( file, access, share, 0, disp, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) {
      FilesDirs();
      h = CreateFileA( file, access, share, 0, disp, FILE_ATTRIBUTE_NORMAL, 0 );
    }
    return h;
  }
  inline void FilesLowerOne( const char* parent, const char* name ) {
    char path[MAX_PATH];
    if( !FilePath( path, MAX_PATH, parent, name ) ) return;
    WIN32_FIND_DATAA fd;
    HANDLE fh = FindFirstFileA( path, &fd );
    if( fh == INVALID_HANDLE_VALUE ) return;
    FindClose( fh );
    if( !( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) || !strcmp( fd.cFileName, name ) ) return;
    char from[MAX_PATH];
    if( FilePath( from, MAX_PATH, parent, fd.cFileName ) && MoveFileA( from, path ) )
      g_filesMoved++;
  }
  inline void FilesLowerMods() {
    char names[32][64];
    int cnt = 0;
    WIN32_FIND_DATAA fd;
    HANDLE fh = FindFirstFileA( "MiniMap\\mods\\*", &fd );
    if( fh == INVALID_HANDLE_VALUE ) return;
    do {
      if( !( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) || fd.cFileName[0] == '.' ) continue;
      bool upper = false;
      int n = 0;
      for( ; fd.cFileName[n] && n < 63; n++ ) {
        const char c = fd.cFileName[n];
        if( c >= 'A' && c <= 'Z' ) upper = true;
        names[cnt][n] = ( c >= 'A' && c <= 'Z' ) ? (char)( c - 'A' + 'a' ) : c;
      }
      names[cnt][n] = 0;
      if( upper && !fd.cFileName[n] )
        cnt++;
    } while( cnt < 32 && FindNextFileA( fh, &fd ) );
    FindClose( fh );
    for( int i = 0; i < cnt; i++ )
      FilesLowerOne( "MiniMap\\mods", names[i] );
  }
  inline void FilesLower() {
    FilesLowerOne( FILES_DIR, "config" );
    FilesLowerOne( FILES_DIR, "mods" );
    FilesLowerMods();
    FilesLowerOne( FILES_DIR, "icons" );
    FilesLowerOne( "MiniMap\\icons", "default" );
    FilesLowerOne( "MiniMap\\icons", "backup" );
    FilesLowerOne( "MiniMap\\icons", "custom" );
    FilesLowerOne( "MiniMap\\icons\\default", "textured" );
    FilesLowerOne( "MiniMap\\icons\\default", "bare" );
  }
  inline void FilesEnsure() {
    if( g_filesReady ) return;
    g_filesReady = true;
    FilesDirs();
    FilesLower();
    FilesMoveOne( "MiniMap.maps",  "MiniMap\\MiniMap.maps" );
    FilesMoveOne( "MiniMap.marks", "MiniMap\\MiniMap.marks" );
    FilesMoveOne( "MiniMap.log",   LOG_PREV_FILE );
    FilesMoveMask( "MiniMap_gen_*.cache", "", FILES_GEN_DIR );
    FilesMoveMask( "MiniMap_det\\*.cache", "MiniMap_det", FILES_DET_DIR );
    RemoveDirectoryA( "MiniMap_det" );
    FilesMoveMask( "MiniMap\\mapy\\*.cache", "MiniMap\\mapy", FILES_GEN_DIR );
    FilesMoveMask( "MiniMap\\szczegoly\\*.cache", "MiniMap\\szczegoly", FILES_DET_DIR );
    RemoveDirectoryA( "MiniMap\\mapy" );
    RemoveDirectoryA( "MiniMap\\szczegoly" );
    FilesMoveOne( "MiniMap\\MiniMap_poprzedni.log", LOG_PREV_FILE );
    PkCleanTmp( FILES_GEN_DIR );
    PkCleanTmp( FILES_DET_DIR );
  }
  inline unsigned __int64 FileTime64( const FILETIME& ft ) {
    return ( (unsigned __int64)ft.dwHighDateTime << 32 ) | ft.dwLowDateTime;
  }
  inline bool FileAgeOld( const FILETIME& ft, int days ) {
    FILETIME now;
    GetSystemTimeAsFileTime( &now );
    const unsigned __int64 a = FileTime64( ft ), n = FileTime64( now );
    const unsigned __int64 span = (unsigned __int64)days * 24u * 3600u * 10000000u;
    return a < n && n - a > span;
  }
  inline void FileTouch( const char* file ) {
    HANDLE h = CreateFileA( file, FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) return;
    FILETIME ft;
    GetSystemTimeAsFileTime( &ft );
    SetFileTime( h, 0, 0, &ft );
    CloseHandle( h );
  }
  inline bool HexChar( char c ) {
    return ( c >= '0' && c <= '9' ) || ( c >= 'A' && c <= 'F' ) || ( c >= 'a' && c <= 'f' );
  }
  static bool g_logStarted = false;
  const DWORD LOG_SHARE = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
  static HANDLE g_logH      = INVALID_HANDLE_VALUE;
  static DWORD  g_logBytes  = 0;
  static bool   g_logCapped = false;
  const DWORD   LOG_LIMIT_BYTES = 2u * 1024u * 1024u;
  inline bool LogKeepLine( const char* s ) {
    return s && ( strncmp( s, "state:", 6 ) == 0 || strncmp( s, "stan:", 5 ) == 0 || strncmp( s, "===", 3 ) == 0 );
  }
  inline void LogRaw( HANDLE h, const char* s ) {
    DWORD w = 0;
    const DWORD n = (DWORD)lstrlenA( s );
    WriteFile( h, s, n, &w, 0 );
    WriteFile( h, "\r\n", 2, &w, 0 );
    g_logBytes += n + 2;
  }
  inline void LogFlush() {
    if( g_logH != INVALID_HANDLE_VALUE )
      FlushFileBuffers( g_logH );
  }
  inline void LogClose() {
    if( g_logH != INVALID_HANDLE_VALUE ) {
      FlushFileBuffers( g_logH );
      CloseHandle( g_logH );
      g_logH = INVALID_HANDLE_VALUE;
    }
  }
  inline void Log( const char* txt ) {
    if( !g_log || !txt ) {
      if( !g_log ) LogClose();
      return;
    }
    if( g_logBytes >= LOG_LIMIT_BYTES
        && ( !LogKeepLine( txt ) || g_logBytes >= LOG_LIMIT_BYTES + 65536u ) )
      return;
    FilesEnsure();
    if( !g_logStarted ) {
      g_logStarted = true;
      if( GetFileAttributesA( LOG_FILE ) != INVALID_FILE_ATTRIBUTES )
        MoveFileExA( LOG_FILE, LOG_PREV_FILE, MOVEFILE_REPLACE_EXISTING );
      SYSTEMTIME st;
      GetLocalTime( &st );
      char head[96];
      snprintf( head, sizeof( head ), "=== MiniMap: session start %04u-%02u-%02u %02u:%02u:%02u ===",
                (unsigned)st.wYear, (unsigned)st.wMonth, (unsigned)st.wDay,
                (unsigned)st.wHour, (unsigned)st.wMinute, (unsigned)st.wSecond );
      HANDLE h0 = FileCreate( LOG_FILE, GENERIC_WRITE, LOG_SHARE, CREATE_ALWAYS );
      if( h0 != INVALID_HANDLE_VALUE ) {
        g_logBytes = 0;
        g_logCapped = false;
        LogRaw( h0, head );
        if( g_filesMoved > 0 || g_filesKept > 0 ) {
          char mv[128];
          snprintf( mv, sizeof( mv ), "files: moved %d, left in place %d (already present)", g_filesMoved, g_filesKept );
          LogRaw( h0, mv );
        }
        g_logH = h0;
      }
    }
    if( g_logH == INVALID_HANDLE_VALUE ) {
      g_logH = FileCreate( LOG_FILE, FILE_APPEND_DATA, LOG_SHARE,
                           OPEN_ALWAYS );
      if( g_logH == INVALID_HANDLE_VALUE ) return;
      SetFilePointer( g_logH, 0, 0, FILE_END );
      const DWORD sz = GetFileSize( g_logH, 0 );
      g_logBytes = ( sz == INVALID_FILE_SIZE ) ? 0 : sz;
    }
    LogRaw( g_logH, txt );
    if( LogKeepLine( txt ) )
      FlushFileBuffers( g_logH );
    if( g_logBytes >= LOG_LIMIT_BYTES && !g_logCapped ) {
      g_logCapped = true;
      LogRaw( g_logH, "log: 2 MB limit reached, only state: and === lines from now on" );
      FlushFileBuffers( g_logH );
    }
  }
#define MM_LOG( x ) do { if( MiniMap::g_log ) MiniMap::Log( x ); } while( 0 )
  inline bool LayLogTick( int ms ) {
    static DWORD winStart = 0;
    static int   cnt = 0, sum = 0, mx = 0;
    bool full = false;
    if( ms >= 0 ) {
      cnt++;
      sum += ms;
      if( ms > mx ) mx = ms;
      const DWORD now = GetTickCount();
      if( winStart != 0 && (DWORD)( now - winStart ) < 30000u ) return false;
      winStart = now;
      full = true;
    }
    if( cnt > 1 || ( ms < 0 && cnt > 0 ) ) {
      MM_LOG( zSTRING( "player layer: summary, updates " ) + zSTRING( cnt ) + ", avg " + zSTRING( sum / cnt ) + " ms, max " + zSTRING( mx ) + " ms" );
    }
    if( ms < 0 || full ) {
      cnt = 0;
      sum = 0;
      mx  = 0;
    }
    return full;
  }
}
#include "MiniMap_Lang.h"
#include "MiniMap_LangKeys.h"
#include "MiniMap_MenuInfo.h"
#include "MiniMap_ModFiles.h"
namespace MiniMap {
  inline char UpChar( char c ) { return MiniMapCfg::Up( c ); }
  inline void StrCopy( char* dst, const char* src, int max ) { MiniMapCfg::Copy( dst, max, src ); }
  inline bool StrEqUp( const char* a, const char* b ) { return MiniMapCfg::EqNoCase( a, b ); }
  inline bool StrHasUp( const char* hay, const char* needle ) { return MiniMapCfg::HasNoCase( hay, needle ); }
  inline bool StrEqAnyUp( const char* s, const char* list ) {
    if( !s || !list ) return false;
    const size_t slen = strlen( s );
    for( const char* p = list; *p; ) {
      const char* e = strchr( p, '|' );
      const size_t n = e ? (size_t)( e - p ) : strlen( p );
      if( n == slen && _strnicmp( s, p, n ) == 0 ) return true;
      if( !e ) break;
      p = e + 1;
    }
    return false;
  }
  inline bool StrHasAnyUp( const char* hay, const char* list ) {
    if( !hay || !*hay || !list ) return false;
    for( const char* p = list; *p; ) {
      const char* e = strchr( p, '|' );
      const size_t n = e ? (size_t)( e - p ) : strlen( p );
      if( n > 0 ) {
        for( const char* h = hay; *h; h++ )
          if( _strnicmp( h, p, n ) == 0 ) return true;
      }
      if( !e ) break;
      p = e + 1;
    }
    return false;
  }
  inline void NormalizeKey( const char* src, char* dst, int max ) {
    if( !dst || max <= 0 ) return;
    dst[0] = 0;
    if( !src ) return;
    const char* base = MiniMapCfg::BaseName( src );
    int i = 0;
    while( base[i] && base[i] != '.' && i < max - 1 ) { dst[i] = UpChar( base[i] ); i++; }
    dst[i] = 0;
  }
  inline zCTree<zCVob>* VobTreeNext( zCTree<zCVob>* node, zCTree<zCVob>* root ) {
    if( node->firstChild ) return node->firstChild;
    while( node && node != root && !node->next ) node = node->parent;
    return ( node && node != root ) ? node->next : 0;
  }
  struct TMapInfo {
    char  world[NAME_MAX];
    float x1, y1, x2, y2;
    bool  hasCoords;
    unsigned int tag;
  };
  static TMapInfo g_maps[MAP_SLOTS];
  static int      g_mapCount = 0;
  static unsigned int g_worldTag = 0;
  inline bool MatchTag( unsigned int entry, unsigned int now ) {
    return entry == 0 || now == 0 || entry == now;
  }
  inline int FindMap( const char* world ) {
    for( int pass = 0; pass < 2; pass++ )
      for( int i = 0; i < g_mapCount; i++ ) {
        if( !StrEqUp( g_maps[i].world, world ) ) continue;
        const bool exact = ( g_maps[i].tag != 0 && g_maps[i].tag == g_worldTag );
        if( pass == 0 ? exact : MatchTag( g_maps[i].tag, g_worldTag ) ) return i;
      }
    return -1;
  }
  inline int MakeMapSlot( const char* world ) {
    if( g_mapCount >= MAP_SLOTS ) return -1;
    TMapInfo& m = g_maps[g_mapCount];
    StrCopy( m.world, world, NAME_MAX );
    m.x1 = m.y1 = m.x2 = m.y2 = 0.0f;
    m.hasCoords  = false;
    m.tag        = g_worldTag;
    return g_mapCount++;
  }
  inline int MakeMap( const char* world ) {
    const int found = FindMap( world );
    if( found >= 0 ) return found;
    return MakeMapSlot( world );
  }
  static zCView*      g_screenSeen = 0;
  static oCWorld*     g_worldSeen  = 0;
  static char         g_world[NAME_MAX] = "";
  static int          g_mapIdx     = -1;
  static zCViewDraw*  g_blit       = 0;
  static bool         g_texReady   = false;
  static bool         g_regenAsk   = false;
  static int          g_regenWait  = 0;
  const int           GEN_MS_SLOTS = 6;
  static int          g_genMs[GEN_MS_SLOTS] = { 0, 0, 0, 0, 0, 0 };
  inline int GenMsIdx( int n ) {
    int i = 0;
    for( int s = 512; s <= 16384; s *= 2, i++ )
      if( s == n ) return i;
    return -1;
  }
  static bool         g_genRunning = false;
  static bool         g_barKeysLogged = false;
  static bool         g_genHeld    = false;
  static int          g_genPhase   = 0;
  static float        g_genFrac    = 0.0f;
  static int          g_genEtaMs   = -1;
  static int          g_vidW       = 0;
  static int          g_vidH       = 0;
  static zCTexture*   g_genTex     = 0;
  static int          g_genTexSize = 0;
  static int          g_genFmt     = -1;
  static int          g_genBpp     = 0;
  static char         g_genWorld[NAME_MAX] = "";
  static bool         g_texTried   = false;
  static float        g_genHMin    = 0.0f;
  static float        g_genHMax    = 0.0f;
  static bool         g_genHKnown  = false;
  const float LAY_CEIL_MARGIN = 225.0f;
  const float LAY_UP_RAY      = 3000.0f;
  const int   LAY_VOB_RAYS    = 12000;
  static zCTexture*     g_layTex      = 0;
  static int            g_layTexSize  = 0;
  static bool           g_layReady    = false;
  static bool           g_layCovered  = false;
  static float          g_layFloor    = 0.0f;
  static float          g_layCX       = 0.0f, g_layCZ = 0.0f, g_laySide = 0.0f;
  static DWORD          g_layNext     = 0;
  static DWORD          g_layLast     = 0;
  struct TLayLevel { int rayLen, onPct, offPct, kOn, kOff, hold; };
  static const TLayLevel kLayLevels[4] = {
    {    0,  0,  0, 1, 1,  0 },
    { 1500, 45, 20, 3, 4,  0 },
    { 1000, 65, 35, 3, 6,  8 },
    {  800, 90, 55, 4, 6, 12 }
  };
  const float LAY_PROBE_R    = 300.0f;
  const float LAY_FLOOR_STEP = 200.0f;
  static int   g_layVote   = 0;
  static int   g_layHold   = 0;
  static int   g_layCovPct = 0;
  static float g_layFade   = 0.0f;
  static DWORD g_layFadeT  = 0;
  static TMapInfo       g_layRect;
  enum { LAY_IDLE = 0, LAY_RAYS, LAY_SHADE };
  static int            g_layPhase    = LAY_IDLE;
  static TMapInfo       g_layPend;
  static int            g_layN        = 0;
  static int            g_layS        = 0;
  static int            g_layCur      = 0;
  static float          g_layTop      = 0.0f;
  static float          g_layCeil     = 0.0f;
  static float          g_layPFloor   = 0.0f;
  static float          g_layPSide    = 0.0f;
  static float          g_layPCX      = 0.0f, g_layPCZ = 0.0f;
  static DWORD          g_layT0       = 0;
  static double         g_layWorkMs   = 0.0;
  static int            g_layFrames   = 0;
  static int            g_layRaysN    = 0;
  static int            g_layPolysN   = 0;
  static int            g_layLongest  = 0;
  static zCPolygon**    g_layPolys    = 0;
  static int            g_layPolyCap  = 0;
  static float*         g_layHgt      = 0;
  static unsigned char* g_layLit      = 0;
  static unsigned char* g_layMat      = 0;
  static unsigned char* g_layTint     = 0;
  static int            g_layTintN    = 0;
  static bool           g_layTintOn   = false;
  static unsigned char* g_layMask     = 0;
  static unsigned char* g_layPx       = 0;
  static int            g_layBufN     = 0;
  static zCTexture*      g_curTex   = 0;
  static const TMapInfo* g_curRect  = 0;
  static bool            g_curBlend = false;
  static int             g_curAlpha = -1;
  static bool         g_paint      = false;
  static bool         g_paintBar   = false;
  static DWORD        g_stateTime  = 0;
  static float g_vsx = 1.0f;
  static float g_vsy = 1.0f;
  static float g_clipX0 = 0.0f, g_clipY0 = 0.0f, g_clipX1 = 0.0f, g_clipY1 = 0.0f;
  struct TEdge {
    float ax, az, bx, bz;
  };
  static TEdge* g_edges     = 0;
  static int    g_edgeCount = 0;
  static bool   g_edgesTried = false;
  const int     EDGE_MAX    = 16000;
  inline void FreeEdges() {
    if( g_edges ) {
      MiniMapCommon::MmFree( g_edges );
      g_edges = 0;
    }
    g_edgeCount  = 0;
    g_edgesTried = false;
    g_walkReady = false;
    g_walkTried = false;
  }
  inline void BuildEdges( oCWorld* world ) {
    g_edgesTried = true;
    if( !world || !world->wayNet ) return;
    zCWayNet* net = world->wayNet;
    for( int pass = 0; pass < 2; pass++ ) {
      int count = 0;
      for( zCListSort<zCWaypoint>* node = &net->wplist; node; node = node->next ) {
        zCWaypoint* wp = node->data;
        if( !wp ) continue;
        for( zCList<zCWay>* wnode = &wp->wayList; wnode; wnode = wnode->next ) {
          zCWay* way = wnode->data;
          if( !way ) continue;
          zCWaypoint* a = way->left;
          zCWaypoint* b = way->right;
          if( !a || !b || a == b ) continue;
          if( wp != a ) continue;
          if( count >= EDGE_MAX ) break;
          if( pass == 1 ) {
            TEdge& e = g_edges[count];
            e.ax = a->pos[0];
            e.az = a->pos[2];
            e.bx = b->pos[0];
            e.bz = b->pos[2];
          }
          count++;
        }
        if( count >= EDGE_MAX ) break;
      }
      if( pass == 0 ) {
        if( count <= 0 ) return;
        g_edges = MiniMapCommon::MmAlloc<TEdge>( (size_t)( count ) );
        if( !g_edges ) return;
      }
      else {
        g_edgeCount = count;
      }
    }
    MM_LOG( zSTRING( "waynet: edges " ) + zSTRING( g_edgeCount ) );
  }
  inline void EnsureEdges() {
    if( g_edgesTried || !ogame ) return;
    BuildEdges( ogame->GetGameWorld() );
  }
  inline void BuildWalkArea() {
    g_walkTried = true;
    g_walkReady = false;
    oCWorld* const world = ogame ? ogame->GetGameWorld() : 0;
    if( !world || !world->wayNet ) return;
    zCWayNet* const net = world->wayNet;
    float xLo = 0.0f, xHi = 0.0f, zLo = 0.0f, zHi = 0.0f;
    int   count = 0;
    for( zCListSort<zCWaypoint>* node = &net->wplist; node; node = node->next ) {
      zCWaypoint* const wp = node->data;
      if( !wp ) continue;
      const float wx = wp->pos[0];
      const float wz = wp->pos[2];
      if( count == 0 ) {
        xLo = xHi = wx;
        zLo = zHi = wz;
      }
      else {
        if( wx < xLo ) xLo = wx;
        if( wx > xHi ) xHi = wx;
        if( wz < zLo ) zLo = wz;
        if( wz > zHi ) zHi = wz;
      }
      count++;
    }
    if( count < 16 ) return;
    if( xHi - xLo < 1000.0f || zHi - zLo < 1000.0f ) return;
    const int   BINS = 256;
    int         hx[BINS], hz[BINS];
    memset( hx, 0, (size_t)( BINS ) * sizeof( hx[0] ) );
    memset( hz, 0, (size_t)( BINS ) * sizeof( hz[0] ) );
    const float kx = (float)( BINS - 1 ) / ( xHi - xLo );
    const float kz = (float)( BINS - 1 ) / ( zHi - zLo );
    for( zCListSort<zCWaypoint>* node = &net->wplist; node; node = node->next ) {
      zCWaypoint* const wp = node->data;
      if( !wp ) continue;
      int bx = (int)( ( wp->pos[0] - xLo ) * kx );
      int bz = (int)( ( wp->pos[2] - zLo ) * kz );
      if( bx < 0 ) bx = 0;
      if( bx >= BINS ) bx = BINS - 1;
      if( bz < 0 ) bz = 0;
      if( bz >= BINS ) bz = BINS - 1;
      hx[bx]++;
      hz[bz]++;
    }
    const int cut = count / 100;
    int loBin, hiBin;
    TrimHistRange( hx, BINS, cut, loBin, hiBin );
    xHi = xLo + (float)( hiBin + 1 ) / kx;
    xLo += (float)loBin / kx;
    TrimHistRange( hz, BINS, cut, loBin, hiBin );
    zHi = zLo + (float)( hiBin + 1 ) / kz;
    zLo += (float)loBin / kz;
    if( xHi - xLo < 1000.0f || zHi - zLo < 1000.0f ) return;
    g_walkX1 = xLo;
    g_walkX2 = xHi;
    g_walkZ1 = zLo;
    g_walkZ2 = zHi;
    g_walkReady = true;
    MM_LOG( zSTRING( "play area from waynet: " ) + zSTRING( (int)g_walkX1 ) + " " + zSTRING( (int)g_walkZ1 ) + " .. " + zSTRING( (int)g_walkX2 ) + " " + zSTRING( (int)g_walkZ2 ) + "  (points " + zSTRING( count ) + ")" );
  }
  inline void UseSavedLand() {
    if( g_landReady || !g_world[0] ) return;
    for( int pass = 0; pass < 2; pass++ )
      for( int i = 0; i < g_landSavedCount; i++ ) {
        const TLandRect& L = g_landSaved[i];
        if( !StrEqUp( L.world, g_world ) ) continue;
        const bool exact = ( L.tag != 0 && L.tag == g_worldTag );
        if( pass == 0 ? !exact : !MatchTag( L.tag, g_worldTag ) ) continue;
        g_landX1 = L.x1;
        g_landZ1 = L.z1;
        g_landX2 = L.x2;
        g_landZ2 = L.z2;
        g_landReady = true;
        MM_LOG( zSTRING( "land area from file: " ) + zSTRING( (int)L.x1 ) + " " + zSTRING( (int)L.z1 ) + " .. " + zSTRING( (int)L.x2 ) + " " + zSTRING( (int)L.z2 ) );
        return;
      }
  }
  inline bool WalkArea( float& x1, float& z1, float& x2, float& z2 ) {
    if( !g_walkTried )
      BuildWalkArea();
    if( !g_walkReady ) return false;
    x1 = g_walkX1;
    z1 = g_walkZ1;
    x2 = g_walkX2;
    z2 = g_walkZ2;
    return true;
  }
  inline void RememberLand() {
    if( !g_landReady || !g_world[0] ) return;
    for( int i = 0; i < g_landSavedCount; i++ ) {
      TLandRect& L = g_landSaved[i];
      if( !StrEqUp( L.world, g_world ) || L.tag != g_worldTag ) continue;
      L.x1 = g_landX1;
      L.z1 = g_landZ1;
      L.x2 = g_landX2;
      L.z2 = g_landZ2;
      return;
    }
    if( g_landSavedCount >= LAND_SLOTS ) return;
    TLandRect& L = g_landSaved[g_landSavedCount++];
    StrCopy( L.world, g_world, NAME_MAX );
    L.x1  = g_landX1;
    L.z1  = g_landZ1;
    L.x2  = g_landX2;
    L.z2  = g_landZ2;
    L.tag = g_worldTag;
  }
  inline void LoadConfig();
  const char* MAPS_FILE = "MiniMap\\MiniMap.maps";
  const char* const MAPS_HEAD =
    "# MiniMap - map view and land areas (created automatically)\r\n"
    "# view <zoom> <size> <grid> <height>\r\n"
    "# land <world> <x1> <z1> <x2> <z2> <world id>\r\n"
    "# gentime <map size> <last map build time in ms>\r\n";
  const char* const MARKS_HEAD =
    "# MiniMap - marks placed on the full map (created automatically)\r\n"
    "# mark2 <world> <world id> <x> <z> <icon> <note to end of line>\r\n"
    "# icons: 0 target, 1 treasure, 2 warning, 3 star, 4 point, "
    "5 fight, 6 home, 7 cave, 8 herb, 9 quest\r\n";
  inline bool FileHeadFix( const char* file, const char* data, DWORD n, const char* head ) {
    static const char kSig[] = "# MiniMap";
    if( !file || !data || !head || n < sizeof( kSig ) - 1 || memcmp( data, kSig, sizeof( kSig ) - 1 ) != 0 ) return false;
    DWORD end = 0;
    while( end < n && data[end] == '#' ) {
      while( end < n && data[end] != '\n' ) end++;
      if( end >= n ) return false;
      end++;
    }
    const DWORD hl = (DWORD)strlen( head );
    if( end == hl && memcmp( data, head, hl ) == 0 ) return false;
    char tmp[260];
    snprintf( tmp, sizeof( tmp ), "%s.tmp", file );
    HANDLE h = FileCreate( tmp, GENERIC_WRITE, 0, CREATE_ALWAYS );
    if( h == INVALID_HANDLE_VALUE ) return false;
    DWORD w1 = 0, w2 = 0;
    const BOOL ok = WriteFile( h, head, hl, &w1, 0 )
                    && ( n == end || WriteFile( h, data + end, n - end, &w2, 0 ) )
                    && w1 == hl && w2 == n - end && FlushFileBuffers( h );
    CloseHandle( h );
    if( !ok || !MoveFileExA( tmp, file, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH ) ) {
      DeleteFileA( tmp );
      return false;
    }
    MM_LOG( zSTRING( "file header updated: " ) + zSTRING( MiniMapCfg::BaseName( file ) ) );
    return true;
  }
  inline const char* SkipBlank( const char* p ) {
    while( *p == ' ' || *p == '\t' )
      p++;
    return p;
  }
  inline const char* ReadWord( const char* p, char* out, int max ) {
    p = SkipBlank( p );
    int i = 0;
    while( *p && *p != ' ' && *p != '\t' && *p != '\r' && *p != '\n' ) {
      if( i < max - 1 )
        out[i++] = *p;
      p++;
    }
    out[i] = 0;
    return p;
  }
  inline const char* ReadNumber( const char* p, float& out, bool& ok ) {
    char word[32];
    p = ReadWord( p, word, 32 );
    ok = false;
    if( !word[0] ) return p;
    char* end = 0;
    const float val = strtof( word, &end );
    if( end == word ) return p;
    out = val;
    ok  = true;
    return p;
  }
  inline unsigned int ParseTag( const char* s ) {
    if( !s || !s[0] ) return 0;
    char* end = 0;
    const unsigned long v = strtoul( s, &end, 16 );
    return ( end && end != s && *end == 0 ) ? (unsigned int)v : 0u;
  }
  inline void SaveMaps() {
    if( !g_rememberMaps ) return;
    FilesEnsure();
    HANDLE h = FileCreate( MAPS_FILE, GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS );
    if( h == INVALID_HANDLE_VALUE ) return;
    MiniMapCfg::TTextBuf B;
    MiniMapCfg::BufInit( B );
    MiniMapCfg::BufPut( B, MAPS_HEAD );
    char line[256];
    if( g_viewTouched ) {
      snprintf( line, sizeof( line ), "view %d %d %d %d\r\n", g_zoom, g_size, g_mapCells, g_sizeY );
      MiniMapCfg::BufPut( B, line );
    }
    RememberLand();
    for( int i = 0; i < g_landSavedCount; i++ ) {
      const TLandRect& L = g_landSaved[i];
      snprintf( line, sizeof( line ), "land %s %d %d %d %d %08X\r\n", L.world, (int)L.x1, (int)L.z1, (int)L.x2, (int)L.z2, L.tag );
      MiniMapCfg::BufPut( B, line );
    }
    for( int i = 0; i < GEN_MS_SLOTS; i++ )
      if( g_genMs[i] > 0 ) {
        snprintf( line, sizeof( line ), "gentime %d %d\r\n", 512 << i, g_genMs[i] );
        MiniMapCfg::BufPut( B, line );
      }
    DWORD written = 0;
    if( !B.bad && B.p ) WriteFile( h, B.p, (DWORD)B.n, &written, 0 );
    MiniMapCfg::BufFree( B );
    CloseHandle( h );
  }
  inline void RequestSave();
  const int   USER_MAX   = 128;
  struct TUserMark {
    char  world[NAME_MAX];
    float x, z;
    int   icon;
    char  note[48];
    unsigned int tag;
  };
  const char* OWN_FILE = "MiniMap\\MiniMap.marks";
  static TUserMark g_ownMarks[USER_MAX];
  inline bool MarkHere( const TUserMark& u ) {
    return StrEqUp( u.world, g_world ) && MatchTag( u.tag, g_worldTag );
  }
  static int       g_ownCount = 0;
  static int       g_guide    = -1;
  const float      GUIDE_REACH = 400.0f;
  static bool      g_guideFileSet = false;
  static char      g_guideFileWorld[NAME_MAX];
  static unsigned  g_guideFileTag = 0;
  static float     g_guideFileX = 0.0f, g_guideFileZ = 0.0f;
  inline int GuideFind( const char* world, unsigned tag, float x, float z ) {
    for( int i = 0; i < g_ownCount; i++ ) {
      const TUserMark& u = g_ownMarks[i];
      const float dx = u.x - x, dz = u.z - z;
      if( StrEqUp( u.world, world ) && u.tag == tag && dx * dx + dz * dz <= 100.0f * 100.0f ) return i;
    }
    return -1;
  }
  static float g_mdx = 0.0f, g_mdy = 0.0f, g_mdz = 0.0f;
  static float g_curX     = 0.0f;
  static float g_curY     = 0.0f;
  static bool  g_curReady = false;
  static int   g_markIcon = 0;
  static int   g_markSize   = 16;
  static int   g_markLabels = 1;
  static int   g_markHint   = 1;
  static int   g_marks      = 0;
  static int   g_hoverInfo  = 1;
  static int   g_hoverDetail = 1;
  inline void HoverConsider( zCVob* vob, float mx, float my, float s, bool isNpc );
  inline bool VobIsChest( zCVob* v );
  inline int  ChestKind( oCMobContainer* ch );
  inline int  SwitchKindOf( zCVob* v );
  inline int  PlaceKindOf( zCVob* v );
  inline const char* PlaceKindName( int kind );
  inline void MarkAtlasFree( bool release );
  inline void IconPackFree( bool release );
  inline void NpcIconsBuild();
  inline void NpcIcoFree();
  inline void ItemIcoFree();
  inline zCOLOR IconColor( int icon );
  static zCVob* g_hovVob    = 0;
  static int   g_keyMarkIcon = 0;
  static int   g_cursorSpeed = 100;
  static int   g_cursorSize  = 22;
  static int   g_cursorFlipX = 0;
  static int   g_cursorFlipY = 0;
    inline DWORD TrimPartialLine( char* buf, DWORD read, DWORD cap,
                                      const char* file ) {
      if( read < cap ) return read;
      DWORD end = read;
      while( end > 0 && buf[end - 1] != '\n' )
        end--;
      const char* name = file;
      for( const char* p = file; p && *p; p++ )
        if( *p == '\\' || *p == '/' ) name = p + 1;
      if( end == 0 ) {
        MM_LOG( zSTRING( "file " ) + zSTRING( name ) + zSTRING( " larger than buffer and no full line"
                           " - skipped" ) );
        return 0;
      }
      MM_LOG( zSTRING( "file " ) + zSTRING( name ) + " larger than buffer (" + zSTRING( (int)cap ) + " B): partial last line dropped, reading " + zSTRING( (int)end ) + " B" );
      return end;
    }
  inline void SaveOwnMarks() {
    FilesEnsure();
    HANDLE h = FileCreate( OWN_FILE, GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS );
    if( h == INVALID_HANDLE_VALUE ) return;
    MiniMapCfg::TTextBuf B;
    MiniMapCfg::BufInit( B );
    MiniMapCfg::BufPut( B, MARKS_HEAD );
    char line[256];
    for( int i = 0; i < g_ownCount; i++ ) {
      const TUserMark& u = g_ownMarks[i];
      snprintf( line, sizeof( line ), "mark2 %s %08X %d %d %d%s%s\r\n", u.world, u.tag, (int)u.x, (int)u.z, u.icon, u.note[0] ? " " : "", u.note );
      MiniMapCfg::BufPut( B, line );
    }
    if( g_guide >= 0 && g_guide < g_ownCount ) {
      const TUserMark& u = g_ownMarks[g_guide];
      snprintf( line, sizeof( line ), "target %s %08X %d %d\r\n", u.world, u.tag, (int)u.x, (int)u.z );
      MiniMapCfg::BufPut( B, line );
    }
    DWORD written = 0;
    if( !B.bad && B.p ) WriteFile( h, B.p, (DWORD)B.n, &written, 0 );
    MiniMapCfg::BufFree( B );
    CloseHandle( h );
    MM_LOG( zSTRING( "own marks saved: " ) + zSTRING( g_ownCount ) );
  }
  inline bool LoadTextFileBuf( const char* file, bool fixHead, const char* head, char* buf, DWORD cap ) {
    FilesEnsure();
    HANDLE h = CreateFileA( file, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) return false;
    DWORD read = 0;
    const BOOL ok = ReadFile( h, buf, cap - 1, &read, 0 );
    CloseHandle( h );
    if( !ok || !read ) return false;
    if( fixHead && read < cap - 1 ) FileHeadFix( file, buf, read, head );
    read = TrimPartialLine( buf, read, cap - 1, file );
    buf[read] = 0;
    return read > 0;
  }
  inline const char* NextCfgLine( const char*& p, char* line, int cap ) {
    while( *p ) {
      int n = 0;
      while( *p && *p != '\n' ) { if( *p != '\r' && n < cap - 1 ) line[n++] = *p; p++; }
      if( *p == '\n' ) p++;
      line[n] = 0;
      const char* q = SkipBlank( line );
      if( *q && *q != '#' ) return q;
    }
    return 0;
  }
  inline void LoadOwnMarks() {
    g_ownCount = 0; g_guide = -1; g_guideFileSet = false;
    char buf[32768], line[512];
    if( !LoadTextFileBuf( OWN_FILE, g_marks != 0, MARKS_HEAD, buf, sizeof( buf ) ) ) return;
    const char *p = buf, *q = 0;
    while( g_ownCount < USER_MAX && ( q = NextCfgLine( p, line, sizeof( line ) ) ) != 0 ) {
      char kind[16];
      q = ReadWord( q, kind, 16 );
      if( StrEqUp( kind, "target" ) ) {
        char raw[NAME_MAX], tw[NAME_MAX];
        q = ReadWord( q, raw, NAME_MAX );
        NormalizeKey( raw, g_guideFileWorld, NAME_MAX );
        q = ReadWord( q, tw, NAME_MAX );
        g_guideFileTag = ParseTag( tw );
        float x = 0.0f, z = 0.0f;
        bool  o1 = false, o2 = false;
        q = ReadNumber( q, x, o1 );
        q = ReadNumber( q, z, o2 );
        if( o1 && o2 ) {
          g_guideFileX   = x;
          g_guideFileZ   = z;
          g_guideFileSet = true;
        }
        continue;
      }
      const bool withTag = StrEqUp( kind, "mark2" );
      if( !withTag && !StrEqUp( kind, "mark" ) ) continue;
      TUserMark& u = g_ownMarks[g_ownCount];
      char raw[NAME_MAX];
      q = ReadWord( q, raw, NAME_MAX );
      NormalizeKey( raw, u.world, NAME_MAX );
      u.tag = 0;
      if( withTag ) {
        char tw[NAME_MAX];
        q = ReadWord( q, tw, NAME_MAX );
        u.tag = ParseTag( tw );
      }
      float x = 0.0f, z = 0.0f, ic = 0.0f;
      bool  o1 = false, o2 = false, o3 = false;
      q = ReadNumber( q, x, o1 );
      q = ReadNumber( q, z, o2 );
      q = ReadNumber( q, ic, o3 );
      if( !o1 || !o2 ) continue;
      u.x    = x;
      u.z    = z;
      u.icon = o3 ? (int)ic : 0;
      q = SkipBlank( q );
      StrCopy( u.note, q, 48 );
      g_ownCount++;
    }
    if( g_guideFileSet ) g_guide = GuideFind( g_guideFileWorld, g_guideFileTag, g_guideFileX, g_guideFileZ );
    MM_LOG( zSTRING( "own marks: " ) + zSTRING( g_ownCount ) );
  }
  inline void GuideSet( int idx ) {
    if( idx < 0 || idx >= g_ownCount || idx == g_guide ) return;
    g_guide = idx;
    SaveOwnMarks();
    MM_LOG( zSTRING( "guide: target " ) + zSTRING( (int)g_ownMarks[idx].x ) + " " + zSTRING( (int)g_ownMarks[idx].z ) );
  }
  inline bool GuideReached( float wx, float wz ) {
    if( g_guide < 0 || g_guide >= g_ownCount ) return false;
    const float dx = g_ownMarks[g_guide].x - wx, dz = g_ownMarks[g_guide].z - wz;
    return dx * dx + dz * dz <= GUIDE_REACH * GUIDE_REACH;
  }
  inline void GuideOnRemove( int idx ) {
    if( g_guide == idx )
      g_guide = -1;
    else if( g_guide > idx )
      g_guide--;
  }
  inline void LoadMaps() {
    char buf[32768], line[512];
    if( !LoadTextFileBuf( MAPS_FILE, g_rememberMaps != 0, MAPS_HEAD, buf, sizeof( buf ) ) ) return;
    const char *p = buf, *q = 0;
    while( ( q = NextCfgLine( p, line, sizeof( line ) ) ) != 0 ) {
      char kind[16];
      q = ReadWord( q, kind, 16 );
      if( StrEqUp( kind, "view" ) ) {
        float zoom = 0.0f, size = 0.0f, cells = 0.0f, sizeY = 0.0f;
        bool  ok1 = false, ok2 = false, ok3 = false, ok4 = false;
        q = ReadNumber( q, zoom, ok1 );
        q = ReadNumber( q, size, ok2 );
        q = ReadNumber( q, cells, ok3 );
        q = ReadNumber( q, sizeY, ok4 );
        if( ok1 ) g_zoom     = (int)zoom;
        if( ok2 ) g_size     = (int)size;
        if( ok3 ) g_mapCells = (int)cells;
        if( ok4 ) g_sizeY    = (int)sizeY;
        if( ok1 || ok2 || ok3 || ok4 ) g_viewTouched = true;
        continue;
      }
      if( StrEqUp( kind, "land" ) ) {
        char lw[NAME_MAX];
        q = ReadWord( q, lw, NAME_MAX );
        float v[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        bool  okv[4] = { false, false, false, false };
        for( int k = 0; k < 4; k++ )
          q = ReadNumber( q, v[k], okv[k] );
        char lt[NAME_MAX];
        q = ReadWord( q, lt, NAME_MAX );
        if( lw[0] && okv[0] && okv[1] && okv[2] && okv[3]
            && v[2] - v[0] > 1.0f && v[3] - v[1] > 1.0f
            && g_landSavedCount < LAND_SLOTS ) {
          TLandRect& L = g_landSaved[g_landSavedCount++];
          StrCopy( L.world, lw, NAME_MAX );
          L.x1 = v[0];
          L.z1 = v[1];
          L.x2 = v[2];
          L.z2 = v[3];
          L.tag = ParseTag( lt );
        }
        continue;
      }
      if( StrEqUp( kind, "gentime" ) ) {
        float gn = 0.0f, gms = 0.0f;
        bool  ok1 = false, ok2 = false;
        q = ReadNumber( q, gn, ok1 );
        q = ReadNumber( q, gms, ok2 );
        const int gi = ok1 ? GenMsIdx( (int)gn ) : -1;
        if( gi >= 0 && ok2 && gms > 0.0f && gms < 3600000.0f ) g_genMs[gi] = (int)gms;
        continue;
      }
    }
    MM_LOG( zSTRING( "maps file: land areas " ) + zSTRING( g_landSavedCount ) );
  }
  inline bool ZoomStepOk( int i ) {
    const int z = kZoomLadderM[i] * 100;
    return z >= g_zoomMin && z <= g_zoomMax;
  }
  inline int ZoomNearestStep( int zoom ) {
    int   best  = -1;
    float bestD = 0.0f;
    const float lz = logf( (float)( zoom > 1 ? zoom : 1 ) );
    for( int i = 0; i < kZoomLadderN; i++ ) {
      if( !ZoomStepOk( i ) ) continue;
      float d = logf( (float)( kZoomLadderM[i] * 100 ) ) - lz;
      if( d < 0.0f ) d = -d;
      if( best < 0 || d < bestD ) {
        best  = i;
        bestD = d;
      }
    }
    return best;
  }
  inline void ZoomSnap() {
    const int i = ZoomNearestStep( g_zoom );
    if( i >= 0 ) g_zoom = kZoomLadderM[i] * 100;
  }
  static int g_menuSizeLvl = -2;
  inline bool SizeLevelOk( int lvl ) {
    if( lvl < 0 || lvl >= kSizeLevels ) return false;
    bool any = false;
    for( int i = 0; i < kSizeLevels; i++ ) {
      const int p = SizeLevelPx( i );
      if( p >= g_sizeMin && p <= g_sizeMax ) {
        any = true;
        break;
      }
    }
    if( !any ) return true;
    const int p = SizeLevelPx( lvl );
    return p >= g_sizeMin && p <= g_sizeMax;
  }
  inline int SizeLevelOf( int px ) {
    int best = -1, bestD = 0;
    for( int i = 0; i < kSizeLevels; i++ ) {
      if( !SizeLevelOk( i ) ) continue;
      int d = SizeLevelPx( i ) - px;
      if( d < 0 ) d = -d;
      if( best < 0 || d < bestD ) {
        best  = i;
        bestD = d;
      }
    }
    return ( best < 0 ) ? 5 : best;
  }
  inline void ClampConfig() {
    if( g_sizeMin < 48 )   g_sizeMin = 48;
    if( g_sizeMax > 1024 ) g_sizeMax = 1024;
    if( g_sizeMax < g_sizeMin ) g_sizeMax = g_sizeMin;
    if( g_size < g_sizeMin ) g_size = g_sizeMin; else if( g_size > g_sizeMax ) g_size = g_sizeMax;
    {
      const int snapped = SizeLevelPx( SizeLevelOf( g_size ) );
      if( snapped != g_size && g_size > 0 && g_sizeY > 0 ) g_sizeY = (int)( (float)g_sizeY * (float)snapped / (float)g_size + 0.5f );
      g_size = snapped;
    }
    if( g_sizeStep < 1 )   g_sizeStep = 1;
    if( g_sizeY < 0 ) g_sizeY = 0; else if( g_sizeY > 1024 ) g_sizeY = 1024;
    if( g_cursorSpeed < 10 ) g_cursorSpeed = 10; else if( g_cursorSpeed > 500 ) g_cursorSpeed = 500;
    if( g_zoomMin < 200 )     g_zoomMin = 200;
    if( g_zoomMax > 500000 )  g_zoomMax = 500000;
    if( g_zoomMax < g_zoomMin ) g_zoomMax = g_zoomMin;
    if( g_zoom < g_zoomMin ) g_zoom = g_zoomMin; else if( g_zoom > g_zoomMax ) g_zoom = g_zoomMax;
    if( g_zoomStep < 105 ) g_zoomStep = 105; else if( g_zoomStep > 300 ) g_zoomStep = 300;
    ZoomSnap();
    if( g_alpha < 16 ) g_alpha = 16; else if( g_alpha > 255 ) g_alpha = 255;
    if( g_bright < 32 ) g_bright = 32; else if( g_bright > 255 ) g_bright = 255;
    if( g_backLevel < 0 ) g_backLevel = 0; else if( g_backLevel > 255 ) g_backLevel = 255;
    if( g_margin < 0 )   g_margin = 0;
    if( g_npcRange < 100 ) g_npcRange = 100;
    if( g_npcLimit < 1 ) g_npcLimit = 1; else if( g_npcLimit > 2000 ) g_npcLimit = 2000;
    if( g_showNpc < 0 ) g_showNpc = 0; else if( g_showNpc > 2 ) g_showNpc = 2;
    if( g_showNpcKind < 0 || g_showNpcKind > 3 ) g_showNpcKind = 0;
    if( g_showNpcKind == 3 ) g_showNpc = 0;
    if( g_anchor < 0 || g_anchor > ANCHOR_CUSTOM ) g_anchor = ANCHOR_TOPRIGHT;
    if( g_shape  != 0 ) g_shape  = 1;
    if( g_playerSize < 0 )  g_playerSize = 0;
    if( g_playerSize > 0 && g_playerSize < 6 ) g_playerSize = 6;
    if( g_playerSize > 64 ) g_playerSize = 64;
    if( g_mapCells < 0 ) g_mapCells = 0; else if( g_mapCells > 32 ) g_mapCells = 32;
    if( g_rotate != 0 ) g_rotate = 1;
    if( g_rotSmooth < 1 ) g_rotSmooth = 1; else if( g_rotSmooth > 100 ) g_rotSmooth = 100;
    if( g_itemRange < 100 ) g_itemRange = 100;
    if( g_itemLimit < 1 ) g_itemLimit = 1; else if( g_itemLimit > 2000 ) g_itemLimit = 2000;
    if( g_showItems < 0 ) g_showItems = 0; else if( g_showItems > 2 ) g_showItems = 2;
    if( g_itemColors != 0 ) g_itemColors = 1;
    if( g_showChests < 0 ) g_showChests = 0; else if( g_showChests > 2 ) g_showChests = 2;
    if( g_showSwitches < 0 ) g_showSwitches = 0; else if( g_showSwitches > 2 ) g_showSwitches = 2;
    if( g_showPlaces < 0 ) g_showPlaces = 0; else if( g_showPlaces > 3 ) g_showPlaces = 3;
    if( g_sightRays != 0 ) g_sightRays = 1;
    if( g_sightRange < 0 ) g_sightRange = 0; else if( g_sightRange > 30000 ) g_sightRange = 30000;
    if( g_sightNear < 0 ) g_sightNear = 0; else if( g_sightNear > 5000 ) g_sightNear = 5000;
    if( g_sightRaysPerFrame < 1 ) g_sightRaysPerFrame = 1; else if( g_sightRaysPerFrame > 64 ) g_sightRaysPerFrame = 64;
  }
  inline int Opt( const char* key, int def ) {
    return zoptions ? zoptions->ReadInt( zSTRING( "MINIMAP" ), key, def ) : def;
  }
  inline int OptKey( const char* key, int def ) {
    if( !zoptions ) return def;
    const zSTRING s = zoptions->ReadString( zSTRING( "MINIMAP" ), key, "" );
    const char* p = s.ToChar();
    if( !p || !p[0] ) return def;
    const int len = (int)strlen( p );
    if( len >= 4 && len % 4 == 0 ) {
      int found = 0;
      bool hex = true;
      for( int b = 0; b + 4 <= len && hex; b += 4 ) {
        const int v = MiniMapCfg::DecodeKeyHex4( p + b );
        if( v < 0 ) hex = false;
        else if( v > 0 && !found ) found = v;
      }
      if( hex ) return found;
    }
    char* end = 0;
    const long val = strtol( p, &end, 10 );
    return ( end && end != p && !*end && val >= 0 ) ? (int)val : def;
  }
  inline void ReloadKeys() {
    g_keyToggle   = OptKey( "KeyToggle",   0 );
    g_keyZoomIn   = OptKey( "KeyZoomIn",   0 );
    g_keyZoomOut  = OptKey( "KeyZoomOut",  0 );
    g_keySizeUp   = OptKey( "KeySizeUp",   0 );
    g_keySizeDown = OptKey( "KeySizeDown", 0 );
    g_keyRotate   = OptKey( "KeyRotate",   0 );
    g_keyFull     = OptKey( "KeyFullMap",  0 );
    g_keyMarkIcon = OptKey( "KeyMarkIcon", 0 );
    g_keyAlign    = OptKey( "KeyAlign",    0 );
    g_keySearch   = OptKey( "KeySearch",   0 );
    g_keyScreen   = OptKey( "KeyFullScreen", 0 );
    g_keyItemList = OptKey( "KeyItemList", 0 );
  }
  inline int IconSizeStart() {
    static const int kOld[5] = { 2, 3, 4, 5, 7 };
    const int is = Opt( "IconScale", -1 );
    return ( is >= 0 && is <= 4 ) ? kOld[is] : ICON_SIZE_DEF;
  }
  inline int IconSetStart() {
    const int ni = Opt( "NpcIcons", -1 ), ist = Opt( "IconStyle", -1 );
    if( ni == 0 || ist == 0 ) return 0;
    if( ist == 2 || ist == 6 ) return 2;
    return 1;
  }
  inline int CursorSpeedLvlStart() {
    const int oi = Opt( "CursorSpeedIdx", -1 );
    if( oi >= 0 ) return CursorSpeedLvlOf( kCursorSpeedOldPct[oi > 6 ? 6 : oi] );
    const int cs = Opt( "CursorSpeed", -1 );
    if( cs > 0 ) return CursorSpeedLvlOf( cs );
    return CURSOR_LVL_DEF;
  }
  struct TColMenu { const char* key; int colk; };
  static const TColMenu kColMenu[] = {
    { "ColEnemy",   COLK_ENEMY },       { "ColNeutral", COLK_NEUTRAL },
    { "ColFriend",  COLK_FRIEND },      { "ColPlayer",  COLK_PLAYER },
    { "ColTrader",  COLK_TRADER },      { "ColCorpse",  COLK_CORPSE },
    { "ColItem1H",  COLK_ITEM_1H },     { "ColItem2H",  COLK_ITEM_2H },
    { "ColRanged",  COLK_ITEM_RANGED }, { "ColPotion",  COLK_ITEM_POTION },
    { "ColPlant",   COLK_ITEM_PLANT },  { "ColMagic",   COLK_ITEM_MAGIC },
  };
  static const int kColMenuCount = (int)( sizeof( kColMenu ) / sizeof( kColMenu[0] ) );
  static_assert( COLK_COUNT <= 40, "g_colMenu too small" );
  inline int WheelFullDefault() {
    const int wi = Opt( "WheelMarkIcon", -1 );
    if( wi < 0 ) return 2;
    return wi ? 1 : 0;
  }
  inline int ExpDetailDef( const char* key ) {
    if( Opt( "ExportDetail", 1 ) == 0 ) return 0;
    if( StrEqUp( key, "ExpFriends" ) ) return Opt( "ExpNpcs", 1 ) ? 1 : 0;
    return StrEqUp( key, "ExpFoes" ) ? 0 : 1;
  }
  inline int SysDefaultLang() {
    const Union::LANGUAGE s = Union::Locale::GetUserLocale().Language;
    return s == Union::LANGUAGE::POLISH ? LANG_PL
         : s == Union::LANGUAGE::GERMAN ? LANG_DE
         : s == Union::LANGUAGE::CZECH  ? LANG_CS : LANG_EN;
  }
  inline int MenuDerivedIdx( const char* key ) {
    if( StrEqAnyUp( key, "ExpChests|ExpItems|ExpPlants|ExpNpcs|ExpFoes|ExpFriends|ExpSwitches|ExpPlaces|ExpExits|ExpTeleports|ExpMarks|ExpPlayer" ) ) return ExpDetailDef( key );
    if( StrEqUp( key, "GenPaths" ) ) return GEN_PATHS_DEF;
    if( StrEqUp( key, "Corner" ) ) return AnchorToCorner( Opt( "Anchor", ANCHOR_TOPRIGHT ), -1 );
    if( StrEqUp( key, "AlignIdx" ) ) {
      const int a = Opt( "Align", -1 );
      if( a < 0 )  return 0;
      if( a <= 3 ) return a + 1;
      return -1;
    }
    if( StrEqUp( key, "FullAlignIdx" ) ) return 0;
    if( StrEqUp( key, "Language" ) ) return SysDefaultLang();
    if( StrEqUp( key, "GenDetailLvl" ) ) {
      int gd = Opt( "GenDetail", -1 );
      if( gd < 0 ) gd = ( Opt( "GenSizeIdx", -1 ) >= 6 ) ? 1 : 0;
      return ( gd <= 1 ) ? 0 : ( gd == 2 ) ? 1 : 2;
    }
    if( StrEqUp( key, "GenIndoorLvl" ) ) return Opt( "GenIndoor", 1 ) ? 2 : 0;
    if( StrEqUp( key, "GenOutside" ) ) return Opt( "GenSea", 0 ) ? 3 : 0;
    if( StrEqUp( key, "WheelMini" ) ) return ( Opt( "WheelZoom", 1 ) >= 2 ) ? 1 : 0;
    if( StrEqUp( key, "WheelFull" ) ) return WheelFullDefault();
    if( StrEqUp( key, "CursorSpeedLvl" ) ) return CursorSpeedLvlStart();
    if( StrEqUp( key, "IconSet" ) ) return IconSetStart();
    if( StrEqAnyUp( key, "IconSizeNpc|IconSizeItem|IconSizeMark" ) ) return IconSizeStart();
    if( StrEqUp( key, "SizeLevel" ) ) {
      static const int kOldPx[4] = { 160, 200, 260, 320 };
      const int si = Opt( "SizeIdx", -1 );
      const int px = ( si >= 0 && si < 4 ) ? kOldPx[si] : Opt( "Size", g_size );
      return SizeLevelOf( px );
    }
    return -1;
  }
  inline bool MenuEntryMissing( const zSTRING& sec, const char* key ) {
    const zSTRING s = zoptions->ReadString( sec, key, "" );
    const char*   p = s.ToChar();
    if( !p || !p[0] || p[0] == '-' ) return true;
    for( int i = 0; p[i]; i++ )
      if( p[i] < '0' || p[i] > '9' ) return true;
    return false;
  }
  inline void MenuDefaults() {
    static bool done = false;
    if( done || !zoptions ) return;
    done = true;
    const zSTRING sec( "MINIMAP" );
    int written = 0;
    for( int i = 0; i < kMenuDefCount; i++ ) {
      const TMenuDef& d = kMenuDefs[i];
      if( !MenuEntryMissing( sec, d.key ) ) continue;
      int idx = ( d.def == MD_DERIVED ) ? MenuDerivedIdx( d.key ) : Opt( d.key, d.def );
      if( d.def != MD_DERIVED && ( idx < 0 || idx >= d.count ) ) idx = d.def;
      if( idx < 0 || idx >= d.count ) continue;
      zoptions->WriteInt( sec, d.key, idx, 0 );
      written++;
    }
    if( written > 0 )
      MM_LOG( zSTRING( "menu: default options written to ini: " ) + zSTRING( written ) );
  }
  inline void LoadConfig() {
    if( g_configLoaded || !zoptions ) return;
    MenuDefaults();
    const int keepZoom = g_zoom;
    const int keepSize = g_size;
    const int keepRot  = g_rotate;
    const int keepCells = g_mapCells;
    const int keepSizeY = g_sizeY;
    static bool menuSeen   = false;
    static int  menuRotate = -2;
    const zSTRING sec( "MINIMAP" );
    g_enabled      = Opt( "Enabled",       1 );
    g_anchor       = Opt( "Anchor", ANCHOR_TOPRIGHT );
    g_margin       = Opt( "Margin", 16 );
    g_posX         = Opt( "PosX", 0 );
    g_posY         = Opt( "PosY", 0 );
    {
      int c = Opt( "Corner", -1 );
      if( c < 0 || c > CORNER_BOTTOMLEFT ) {
        c = AnchorToCorner( g_anchor, CORNER_CUSTOM );
        if( c != CORNER_TOPRIGHT && c != CORNER_CUSTOM )
          zoptions->WriteInt( sec, "Corner", c, 0 );
      }
      g_corner = c;
    }
    g_size         = Opt( "Size", g_size );
    g_sizeY        = Opt( "SizeY", 0 );
    g_sizeMin      = Opt( "SizeMin", 96 );
    g_sizeMax      = Opt( "SizeMax", 440 );
    g_sizeStep     = Opt( "SizeStep", 24 );
    g_zoom         = Opt( "Zoom", g_zoom );
    g_zoomMin      = Opt( "ZoomMin", 1200 );
    g_zoomMax      = Opt( "ZoomMax", 90000 );
    g_zoomStep     = Opt( "ZoomStep", 125 );
    g_alpha        = Opt( "Alpha", 255 );
    g_bright       = Opt( "Brightness", 255 );
    g_backLevel    = Opt( "BackLevel", 14 );
    g_border       = Opt( "Border", 1 );
    g_showPlayer   = Opt( "ShowPlayer", 1 );
    g_playerRing   = ( Opt( "PlayerRing", 1 ) != 0 ) ? 1 : 0;
    g_playerSize   = Opt( "PlayerSize", 0 );
    g_playerEdge   = Opt( "PlayerOutline", 1 );
    g_arrowSize    = Opt( "ArrowSize", 0 );
    {
      int is = Opt( "IconScale", -1 );
      if( is < 0 ) {
        is = 1;
        zoptions->WriteInt( sec, "IconScale", is, 0 );
      }
      g_iconScale = ( is > 4 ) ? 4 : is;
      {
        int set = Opt( "IconSet", -1 );
        if( set < 0 || set > 3 ) set = IconSetStart();
        g_iconSet   = set;
        g_iconStyle = set;
        g_npcIcons  = set ? 2 : 0;
      }
      g_sizeNpc  = IconSizeIdx( Opt( "IconSizeNpc", -1 ) );
      g_sizeItem = IconSizeIdx( Opt( "IconSizeItem", -1 ) );
      g_sizeMark = IconSizeIdx( Opt( "IconSizeMark", -1 ) );
      memset( g_colMenu, 0, (size_t)( 40 ) * sizeof( g_colMenu[0] ) );
      for( int c = 0; c < kColMenuCount; c++ ) {
        const int v = Opt( kColMenu[c].key, 0 );
        g_colMenu[kColMenu[c].colk] = ( v < 0 || v > 11 ) ? 0 : v;
      }
      g_modFiles = ( Opt( "ModFiles", 1 ) != 0 ) ? 1 : 0;
      g_iconZoom = ( Opt( "IconZoom", 1 ) != 0 ) ? 1 : 0;
      g_itemIcons = ( Opt( "ItemIcons", 1 ) != 0 && g_iconSet != 0 ) ? 1 : 0;
      g_modAuto  = ( Opt( "ModAutoFiles", 1 ) != 0 ) ? 1 : 0;
    }
    {
      static const int kWinSize[4] = { 160, 200,  260,  320 };
      int lvl = Opt( "GenDetailLvl", -1 );
      if( lvl < 0 ) {
        int gd = Opt( "GenDetail", -1 );
        if( gd < 0 ) gd = ( Opt( "GenSizeIdx", -1 ) >= 6 ) ? 1 : 0;
        lvl = ( gd <= 1 ) ? 0 : ( gd == 2 ) ? 1 : 2;
        zoptions->WriteInt( sec, "GenDetailLvl", lvl, 0 );
      }
      if( lvl > 2 ) lvl = 2;
      const int gd = lvl + 1;
      g_genDetail = gd;
      g_genSize = 8192;
      {
        const int ps = Opt( "GenPatchSize", 2048 );
        g_genPatchSize = ( ps >= 4096 ) ? 4096 : ( ps >= 2048 ) ? 2048 : 1024;
      }
      {
        int ds = Opt( "DetailSpeed", 0 );
        if( ds < 0 ) ds = 0; else if( ds > 4 ) ds = 4;
        g_dpFrameMs = kDetailSpeedMs[ds];
      }
      {
        int dq = Opt( "DetailQuality", 0 );
        if( dq < 0 ) dq = 0; else if( dq > 3 ) dq = 3;
        g_detailQuality = dq;
      }
      const int sl = Opt( "SizeLevel", -1 );
      if( sl >= 0 && sl < kSizeLevels )
        g_size = SizeLevelPx( sl );
      else {
        const int si = Opt( "SizeIdx", -1 );
        if( si >= 0 && si < (int)( sizeof( kWinSize ) / sizeof( kWinSize[0] ) ) ) g_size = kWinSize[si];
      }
    }
    if( g_arrowSize < 0 )   g_arrowSize = 0;
    if( g_arrowSize > 0 && g_arrowSize < 8 ) g_arrowSize = 8;
    if( g_arrowSize > 96 )  g_arrowSize = 96;
    g_genVivid     = Opt( "GenVivid", 108 );
    g_genBright    = Opt( "GenBright", 100 );
    g_genRelief    = Opt( "GenRelief", 170 );
    g_genEdge      = Opt( "GenEdge", 18 );
    {
      const int tc = Opt( "GenTexColors", 1 );
      g_genTexColors = ( tc == 2 ) ? 2 : ( ( tc != 0 ) ? 1 : 0 );
    }
    g_genVobs      = Opt( "GenVobs", 1 );
    g_genVobMin    = Opt( "GenVobMin", 300 );
    g_genPlantMin  = Opt( "GenPlantMin", 110 );
    {
      g_genFill   = Opt( "GenFillHoles", 1 );
      g_genOutside = Opt( "GenOutside", Opt( "GenSea", 0 ) ? 3 : 0 );
      if( g_genOutside < 0 ) g_genOutside = 0; else if( g_genOutside > 3 ) g_genOutside = 3;
      g_cursorSpeed = Opt( "CursorSpeed", 100 );
      {
        const int ci = Opt( "CursorSpeedLvl", -1 );
        if( ci >= 0 ) g_cursorSpeed = CursorSpeedPct( ci );
      }
      g_cursorSize  = Opt( "CursorSize",  22 );
      g_cursorFlipX = Opt( "CursorFlipX", 0 );
      g_cursorFlipY = Opt( "CursorFlipY", 0 );
      if( g_cursorSize < 8 ) g_cursorSize = 8; else if( g_cursorSize > 48 ) g_cursorSize = 48;
      g_markIcon    = Opt( "MarkIcon",    0 );
      g_markSize    = Opt( "MarkSize",    16 );
      g_markLabels  = Opt( "MarkLabels",  1 );
      g_markHint    = Opt( "MarkHint",    1 );
      if( g_markIcon < 0 ) g_markIcon = 0; else if( g_markIcon > MARK_ICONS - 1 ) g_markIcon = MARK_ICONS - 1;
      g_marks       = ( Opt( "Marks",     0 ) != 0 ) ? 1 : 0;
      g_hoverInfo   = Opt( "HoverInfo", 1 );
      if( g_hoverInfo < 0 || g_hoverInfo > 3 ) g_hoverInfo = 1;
      g_hoverDetail = Opt( "HoverDetail", 1 );
      if( g_hoverDetail < 0 ) g_hoverDetail = 0; else if( g_hoverDetail > 2 ) g_hoverDetail = 2;
      if( g_markSize < 8 ) g_markSize = 8; else if( g_markSize > 48 ) g_markSize = 48;
      if( g_markLabels < 0 ) g_markLabels = 0; else if( g_markLabels > 2 ) g_markLabels = 2;
      g_genSmooth = Opt( "GenSmooth",    0 );
      g_genPaths    = Opt( "GenPaths",    GEN_PATHS_DEF );
      g_genPathWide = Opt( "GenPathWidth", 480 );
      g_genPathsWater = Opt( "GenPathsWater", 1 );
      if( g_genPathWide < 20 ) g_genPathWide = 20; else if( g_genPathWide > 2000 ) g_genPathWide = 2000;
      if( g_genSmooth < 0 ) g_genSmooth = 0;
      if( g_genSmooth > 2 ) {
        g_genSmooth = 0;
        zoptions->WriteInt( sec, "GenSmooth", 0, 0 );
      }
    }
    g_genCache     = Opt( "GenCache", 1 );
    g_genPack      = Opt( "GenCachePack", 4 );
    if( g_genPack != 0 && g_genPack != 3 ) g_genPack = 4;
    g_genDetailCacheMB = Opt( "GenDetailCacheMB", 1024 );
    if( g_genDetailCacheMB < 0 ) g_genDetailCacheMB = 0; else if( g_genDetailCacheMB > 65536 ) g_genDetailCacheMB = 65536;
    {
      int il = Opt( "GenIndoorLvl", -1 );
      if( il < 0 ) {
        il = Opt( "GenIndoor", 1 ) ? 2 : 0;
        if( zoptions )
          zoptions->WriteInt( sec, "GenIndoorLvl", il, 0 );
      }
      if( il > 3 ) il = 3;
      g_genIndoorLvl   = il;
      g_genIndoor      = ( il > 0 ) ? 1 : 0;
      g_genIndoorAlpha = Opt( "GenIndoorAlpha", 100 );
      if( g_genIndoorAlpha < 20 ) g_genIndoorAlpha = 20; else if( g_genIndoorAlpha > 100 ) g_genIndoorAlpha = 100;
    }
    g_genContour   = Opt( "GenContour", 500 );
    g_genContourDark = Opt( "GenContourDark", 12 );
    g_genSculpt    = Opt( "GenSculpt", 1 );
    if( g_genSculpt < 0 ) g_genSculpt = 0; else if( g_genSculpt > 2 ) g_genSculpt = 2;
    g_genTreeShapes = Opt( "GenTreeShapes", 3 );
    if( g_genTreeShapes < 0 ) g_genTreeShapes = 0; else if( g_genTreeShapes > 3 ) g_genTreeShapes = 3;
    if( g_genSize < 64 ) g_genSize = 64; else if( g_genSize > 8192 ) g_genSize = 8192;
    if( g_genVivid < 0 ) g_genVivid = 0; else if( g_genVivid > 300 ) g_genVivid = 300;
    if( g_genBright < 25 ) g_genBright = 25; else if( g_genBright > 200 ) g_genBright = 200;
    if( g_genRelief < 10 ) g_genRelief = 10; else if( g_genRelief > 600 ) g_genRelief = 600;
    if( g_genEdge < 0 ) g_genEdge = 0; else if( g_genEdge > 60 ) g_genEdge = 60;
    if( g_genContour < 0 )     g_genContour = 0;
    if( g_genContourDark < 0 ) g_genContourDark = 0; else if( g_genContourDark > 60 ) g_genContourDark = 60;
    if( g_genPlantMin < 30 ) g_genPlantMin = 30; else if( g_genPlantMin > 2000 ) g_genPlantMin = 2000;
    if( g_genVobMin < 50 ) g_genVobMin = 50; else if( g_genVobMin > 5000 ) g_genVobMin = 5000;
    g_showNorth    = Opt( "ShowNorth", 1 );
    g_showDetailBar = Opt( "ShowDetailBar", 0 ) ? 1 : 0;
    g_showLabel    = Opt( "ShowLabel", 1 );
    g_showScale     = Opt( "ShowScale", 1 );
    if( g_showScale < 0 ) g_showScale = 0; else if( g_showScale > 2 ) g_showScale = 2;
    g_showNpc      = Opt( "ShowNpc", 0 );
    g_showNpcKind  = Opt( "ShowNpcKind", 0 );
    g_npcRange     = Opt( "NpcRange", 5000 );
    g_npcLimit     = Opt( "NpcLimit", 120 );
    g_showWaynet   = Opt( "ShowWaynet", 0 );
    if( Opt( "WaynetOverMap", 0 ) != 0 ) g_showWaynet = 1;
    if( g_showWaynet != 0 ) g_showWaynet = 1;
    g_rememberMaps = Opt( "RememberMaps", 1 );
    g_hideInInv    = Opt( "HideInInventory", 1 );
    g_hideInModal  = Opt( "HideInDialog", 1 );
    g_hideInStatus = Opt( "HideInStatus", 0 );
    g_startHidden  = Opt( "StartHidden", 0 );
    g_log          = Opt( "Log", 1 );
    if( !g_log )
      LogClose();
    g_shape        = Opt( "Shape", 0 );
    g_rotate       = Opt( "Rotate", 0 );
    g_rotSmooth    = Opt( "RotateSmooth", 25 );
    g_mapCells     = Opt( "MapCells", 0 );
    g_showItems    = Opt( "ShowItems", 0 );
    g_itemRange    = Opt( "ItemRange", 3000 );
    g_itemLimit    = Opt( "ItemLimit", 60 );
    g_itemColors   = Opt( "ItemColors", 1 );
    g_showChests   = Opt( "ShowChests", 2 );
    g_chestCombo   = Opt( "ChestCombo", 0 );
    if( g_chestCombo < 0 ) g_chestCombo = 0; else if( g_chestCombo > 2 ) g_chestCombo = 2;
    g_hideEmpty    = Opt( "HideEmpty", 0 ) ? 1 : 0;
    g_itemList     = Opt( "ItemList", 0 ) ? 1 : 0;
    g_showTeleports = Opt( "ShowTeleports", 1 ) ? 1 : 0;
    g_fog          = Opt( "Fog", 0 ) ? 1 : 0;
    g_exportZoom   = Opt( "ExportZoom", 0 );
    if( g_exportZoom < 0 || g_exportZoom > 2 ) g_exportZoom = 0;
    g_exportDetail = Opt( "ExportDetail", 1 ) ? 1 : 0;
    g_exportFormat  = Opt( "ExportFormat", 2 );
    if( g_exportFormat < 0 || g_exportFormat > 2 ) g_exportFormat = 2;
    g_exportQuality = Opt( "ExportQuality", 2 );
    if( g_exportQuality < 0 || g_exportQuality > 3 ) g_exportQuality = 2;
    g_expChests    = Opt( "ExpChests",    ExpDetailDef( "ExpChests" ) )    ? 1 : 0;
    g_expItems     = Opt( "ExpItems",     ExpDetailDef( "ExpItems" ) )     ? 1 : 0;
    g_expPlants    = Opt( "ExpPlants",    ExpDetailDef( "ExpPlants" ) )    ? 1 : 0;
    g_expNpcs      = Opt( "ExpNpcs",      ExpDetailDef( "ExpNpcs" ) )      ? 1 : 0;
    g_expFoes      = Opt( "ExpFoes",      ExpDetailDef( "ExpFoes" ) )      ? 1 : 0;
    g_expFriends   = Opt( "ExpFriends",   ExpDetailDef( "ExpFriends" ) )   ? 1 : 0;
    g_expSwitches  = Opt( "ExpSwitches",  ExpDetailDef( "ExpSwitches" ) )  ? 1 : 0;
    g_expPlaces    = Opt( "ExpPlaces",    ExpDetailDef( "ExpPlaces" ) )    ? 1 : 0;
    g_expExits     = Opt( "ExpExits",     ExpDetailDef( "ExpExits" ) )     ? 1 : 0;
    g_expTeleports = Opt( "ExpTeleports", ExpDetailDef( "ExpTeleports" ) ) ? 1 : 0;
    g_expMarksOn   = Opt( "ExpMarks",     ExpDetailDef( "ExpMarks" ) )     ? 1 : 0;
    g_expPlayer    = Opt( "ExpPlayer",    ExpDetailDef( "ExpPlayer" ) )    ? 1 : 0;
    g_fogRadius    = Opt( "FogRadius", 3000 );
    if( g_fogRadius < 500 ) g_fogRadius = 500; else if( g_fogRadius > 20000 ) g_fogRadius = 20000;
    {
      const int fa = Opt( "FogOpacity", 100 );
      g_fogAlpha = ( fa < 20 ) ? 20 : ( fa > 100 ) ? 100 : fa;
    }
    g_showSwitches = Opt( "ShowSwitches", 2 );
    g_showPlaces   = Opt( "ShowPlaces", 2 );
    g_showFurniture = Opt( "ShowFurniture", 0 ) ? 1 : 0;
    g_showCraft     = Opt( "ShowCraft", 1 ) ? 1 : 0;
    g_showAlchemy   = Opt( "ShowAlchemy", 1 ) ? 1 : 0;
    g_showRunes     = Opt( "ShowRunes", 1 ) ? 1 : 0;
    g_showScrolls   = Opt( "ShowScrolls", 1 ) ? 1 : 0;
    g_showDemon     = Opt( "ShowDemon", 1 ) ? 1 : 0;
    g_showKitchen   = Opt( "ShowKitchen", 1 ) ? 1 : 0;
    g_showWorkshop  = Opt( "ShowWorkshop", 1 ) ? 1 : 0;
    g_showLadders   = Opt( "ShowLadders", 1 ) ? 1 : 0;
    g_showFire      = Opt( "ShowFire", 0 ) ? 1 : 0;
    g_showClutter   = Opt( "ShowClutter", 0 ) ? 1 : 0;
    g_placeLearn    = Opt( "PlaceLearn", 1 ) ? 1 : 0;
    g_showExits    = Opt( "ShowExits", 2 );
    if( g_showExits < 0 || g_showExits > 2 ) g_showExits = 2;
    g_searchOthers = Opt( "SearchOthers", 0 );
    if( g_searchOthers < 0 || g_searchOthers > 1 ) g_searchOthers = 0;
    g_showTraders  = Opt( "ShowTraders", 2 );
    if( g_showTraders < 0 || g_showTraders > 2 ) g_showTraders = 2;
    g_showCorpses  = Opt( "ShowCorpses", 1 );
    if( g_showCorpses < 0 || g_showCorpses > 2 ) g_showCorpses = 1;
    g_hoverInv     = Opt( "HoverInv", 0 );
    if( g_hoverInv < 0 || g_hoverInv > 2 ) g_hoverInv = 1;
    g_searchInv    = Opt( "SearchInv", 0 );
    if( g_searchInv < 0 || g_searchInv > 2 ) g_searchInv = 1;
    g_sightRays         = Opt( "SightRays", 1 );
    g_sightRange        = Opt( "SightRange", 0 );
    g_sightNear         = Opt( "SightNear", 600 );
    g_sightRaysPerFrame = Opt( "SightRaysPerFrame", 12 );
    ReloadKeys();
    static bool alignRead = false;
    if( !alignRead ) {
      alignRead = true;
      g_align   = Opt( "Align", -1 );
      if( g_align < -1 ) g_align = -1; else if( g_align > 7 ) g_align = 7;
    }
    {
      const int ai = Opt( "AlignIdx", -1 );
      if( ai == 0 )            g_align = -1;
      else if( ai >= 1 && ai <= 4 ) g_align = ai - 1;
      const int fi = Opt( "FullAlignIdx", -1 );
      if( fi == 0 )                 g_fullAlign = -1;
      else if( fi >= 1 && fi <= 4 ) g_fullAlign = fi - 1;
    }
    g_useFullMap   = Opt( "FullMap",      1 );
    g_wheelZoom    = ( Opt( "WheelMini", Opt( "WheelZoom", 1 ) >= 2 ? 1 : 0 ) != 0 ) ? 2 : 0;
    g_wheelFull    = Opt( "WheelFull", WheelFullDefault() );
    if( g_wheelFull < 0 || g_wheelFull > 2 ) g_wheelFull = 2;
    g_wheelMarkIcon = ( g_wheelFull == 1 ) ? 1 : 0;
    if( Opt( "WheelMini", -1 ) < 0 )
      zoptions->WriteInt( sec, "WheelMini", ( g_wheelZoom >= 2 ) ? 1 : 0, 0 );
    if( Opt( "WheelFull", -1 ) < 0 )
      zoptions->WriteInt( sec, "WheelFull", g_wheelFull, 0 );
    g_fullMargin   = Opt( "FullMargin", 40 );
    g_fullFit      = Opt( "FullFit", 1 );
    g_miniStyle    = Opt( "MiniStyle", 0 );
    g_fullStyle    = Opt( "FullStyle", 0 );
    g_fullSizeIdx  = Opt( "FullSize", 4 );
    if( g_miniStyle < 0 || g_miniStyle > 3 )     g_miniStyle = 0;
    if( g_fullStyle < 0 || g_fullStyle > 3 )     g_fullStyle = 0;
    if( g_fullSizeIdx < 0 || g_fullSizeIdx > 4 ) g_fullSizeIdx = 4;
    g_fullScreen   = Opt( "FullScreen", 0 ) ? 1 : 0;
    g_fullStart    = Opt( "FullStart", 75 );
    g_fullEdgePan  = Opt( "FullEdgePan", 40 );
    g_fullEdgeZone = Opt( "FullEdgeZone", 24 );
    g_fullEdgeScroll = Opt( "FullEdgeScroll", 0 ) ? 1 : 0;
    g_mapDragOn      = Opt( "FullDragMap", 1 ) ? 1 : 0;
    if( g_fullEdgePan < 0 ) g_fullEdgePan = 0; else if( g_fullEdgePan > 300 ) g_fullEdgePan = 300;
    if( g_fullEdgeZone < 2 ) g_fullEdgeZone = 2; else if( g_fullEdgeZone > 200 ) g_fullEdgeZone = 200;
    g_fullLockCam  = Opt( "FullLockCamera", 1 );
    g_fullBlockKeys = Opt( "FullBlockKeys", 1 );
    g_fullPause     = Opt( "FullPause", 1 );
    g_markGuide     = Opt( "MarkGuide", 0 );
    if( g_markGuide < 0 || g_markGuide > 2 ) g_markGuide = 0;
    g_fullCenter   = Opt( "FullCenter", 1 );
    if( g_fullCenter != 0 ) g_fullCenter = 1;
    if( g_fullStart < 2 ) g_fullStart = 2; else if( g_fullStart > 100 ) g_fullStart = 100;
    g_lang = Opt( "Language", SysDefaultLang() );
    if( g_lang < 0 || g_lang >= LANG_COUNT ) g_lang = LANG_PL;
    {
      const zSTRING f = zoptions->ReadString( sec, "Font", "FONT_OLD_10_WHITE.TGA" );
      if( !f.IsEmpty() )
        StrCopy( g_fontName, f.ToChar(), NAME_MAX );
      zSTRING p = zoptions->ReadString( sec, "PlayerTex", "" );
      StrCopy( g_playerTex, p.ToChar(), NAME_MAX );
    }
    static bool mapsLoaded = false;
    bool mapsJustLoaded = false;
    if( !mapsLoaded ) {
      LoadMaps();
      g_visible  = ( g_startHidden == 0 );
      mapsLoaded = true;
      mapsJustLoaded = true;
    }
    if( g_viewTouched && !mapsJustLoaded ) {
      g_zoom = keepZoom;
      g_size = keepSize;
      g_rotate = keepRot;
      g_mapCells = keepCells;
      g_sizeY    = keepSizeY;
    }
    {
      const int sizeLvlNow = Opt( "SizeLevel", -1 );
      const int rotateNow  = Opt( "Rotate", 0 );
      if( menuSeen ) {
        if( sizeLvlNow != g_menuSizeLvl && sizeLvlNow >= 0 && sizeLvlNow < kSizeLevels ) {
          g_size  = SizeLevelPx( sizeLvlNow );
          g_sizeY = 0;
          if( g_viewTouched )
            SaveMaps();
        }
        if( rotateNow != menuRotate ) g_rotate = rotateNow;
      }
      menuSeen    = true;
      g_menuSizeLvl = sizeLvlNow;
      menuRotate  = rotateNow;
    }
    ClampConfig();
    {
      const int lvl = SizeLevelOf( g_size );
      if( lvl != g_menuSizeLvl ) {
        zoptions->WriteInt( sec, "SizeLevel", lvl, 0 );
        g_menuSizeLvl = lvl;
      }
    }
    g_configLoaded = true;
    MM_LOG( zSTRING( "config: size " ) + zSTRING( g_size ) + " px, zoom " + zSTRING( g_zoom ) );
  }
  inline void ScreenSizePx( int& w, int& h ) {
    w = 640;
    h = 480;
    if( screen && screen->psizex > 0 && screen->psizey > 0 ) {
      w = screen->psizex;
      h = screen->psizey;
    }
    else if( zrenderer && zrenderer->vid_xdim > 0 ) {
      w = zrenderer->vid_xdim;
      h = zrenderer->vid_ydim;
    }
  }
  inline void VideoSizePx( int& w, int& h ) {
    if( zrenderer && zrenderer->vid_xdim > 0 && zrenderer->vid_ydim > 0 ) {
      w = zrenderer->vid_xdim;
      h = zrenderer->vid_ydim;
      return;
    }
    ScreenSizePx( w, h );
  }
  inline void RefreshScale() {
    g_vsx = 1.0f;
    g_vsy = 1.0f;
    if( !screen ) return;
    int sw, sh;
    ScreenSizePx( sw, sh );
    if( sw > 0 ) g_vsx = (float)screen->anx( sw ) / (float)sw;
    if( sh > 0 ) g_vsy = (float)screen->any( sh ) / (float)sh;
  }
  inline int VX( float px ) { return (int)( px * g_vsx + 0.5f ); }
  inline int VY( float py ) { return (int)( py * g_vsy + 0.5f ); }
  inline float UiScale() {
    int sw, sh;
    ScreenSizePx( sw, sh );
    const int s = ( sw < sh ) ? sw : sh;
    float k = (float)s / 1080.0f;
    if( k < 0.4f ) k = 0.4f; else if( k > 3.0f ) k = 3.0f;
    return k;
  }
  inline int AlignNow();
  inline bool FullArea( const TMapInfo& m,
                        float& x1, float& z1, float& x2, float& z2 );
  inline int FullSizePct() {
    static const int kPct[5] = { 60, 70, 80, 90, 100 };
    int i = g_fullSizeIdx;
    if( i < 0 ) i = 0; else if( i > 4 ) i = 4;
    return kPct[i];
  }
  inline void FullScreenSet( int on ) {
    g_fullScreen = on ? 1 : 0;
    if( zoptions )
      zoptions->WriteInt( zSTRING( "MINIMAP" ), "FullScreen", g_fullScreen, 0 );
    MM_LOG( g_fullScreen ? "full map: full screen" : "full map: window" );
  }
  static float g_ilPanX = 0.0f, g_ilPanY = 0.0f, g_ilPanW = 0.0f, g_ilPanH = 0.0f;
  inline bool IlReserve() {
    return g_itemList && g_full && ( g_keyItemList <= 0 || g_ilShown );
  }
  inline void IlPanelSize( int sw, int frame, int& pan, int& gap ) {
    pan = 0;
    gap = 0;
    if( !IlReserve() ) return;
    pan = (int)( (float)sw * 0.19f );
    const int minW = (int)( 190.0f * UiScale() );
    if( pan < minW ) pan = minW; else if( pan > sw * 32 / 100 ) pan = sw * 32 / 100;
    gap = 10 + frame;
  }
  inline void FullRect( int& x, int& y, int& w, int& h ) {
    int sw, sh;
    ScreenSizePx( sw, sh );
    int pan = 0, gap = 0;
    g_ilPanW = 0.0f;
    if( g_fullScreen ) {
      const int ms = g_border ? StFullMinMargin( g_fullStyle, UiScale() ) : 0;
      IlPanelSize( sw, ms, pan, gap );
      x = ms + ( pan > 0 ? pan + gap : 0 );
      y = ms;
      w = sw - 2 * ms - ( pan > 0 ? pan + gap : 0 );
      h = sh - 2 * ms;
      if( w < 64 ) w = 64;
      if( h < 64 ) h = 64;
      if( pan > 0 ) {
        g_ilPanX = (float)( x - gap - pan );
        g_ilPanY = (float)y;
        g_ilPanW = (float)pan;
        g_ilPanH = (float)h;
      }
      return;
    }
    const float s = UiScale();
    int m = (int)( (float)g_fullMargin * s + 0.5f );
    if( m < 0 ) m = 0; else if( m > sh / 4 ) m = sh / 4;
    int mst = 0;
    {
      mst = g_border ? StFullMinMargin( g_fullStyle, s ) : 0;
      if( m < mst && mst <= sh / 4 ) m = mst;
    }
    IlPanelSize( sw, mst, pan, gap );
    int availW = sw - 2 * m - ( pan > 0 ? pan + gap : 0 );
    int availH = sh - 2 * m;
    if( availW < 64 ) availW = 64;
    if( availH < 64 ) availH = 64;
    {
      const int pct = FullSizePct();
      if( pct < 100 ) {
        availW = availW * pct / 100;
        availH = availH * pct / 100;
        if( availW < 64 ) availW = 64;
        if( availH < 64 ) availH = 64;
      }
    }
    int side = ( availH < availW ) ? availH : availW;
    if( side < 64 )         side = 64;
    w = side;
    h = side;
    if( g_fullFit && g_mapIdx >= 0 && g_maps[g_mapIdx].hasCoords ) {
      const TMapInfo& mi = g_maps[g_mapIdx];
      float dx = mi.x2 - mi.x1;
      float dz = mi.y1 - mi.y2;
      if( dx < 0.0f ) dx = -dx;
      if( dz < 0.0f ) dz = -dz;
      {
        float ax1, az1, ax2, az2;
        if( FullArea( mi, ax1, az1, ax2, az2 ) ) {
          const float adx = ax2 - ax1;
          const float adz = az2 - az1;
          if( adx > 1.0f && adz > 1.0f ) {
            dx = adx;
            dz = adz;
          }
        }
      }
      if( AlignNow() & 1 ) {
        std::swap( dx, dz );
      }
      if( dx > 1.0f && dz > 1.0f ) {
        const float kw = (float)availW / dx;
        const float kh = (float)availH / dz;
        const float k  = ( kw < kh ) ? kw : kh;
        w = (int)( dx * k );
        h = (int)( dz * k );
        if( w < 64 ) w = 64;
        if( h < 64 ) h = 64;
      }
    }
    const int extra = ( pan > 0 ) ? pan + gap : 0;
    x = ( sw - w - extra ) / 2 + extra;
    y = ( sh - h ) / 2;
    if( pan > 0 ) {
      g_ilPanX = (float)( x - gap - pan );
      g_ilPanY = (float)( ( sh - availH ) / 2 );
      g_ilPanW = (float)pan;
      g_ilPanH = (float)availH;
      if( g_ilPanH < (float)h ) { g_ilPanY = (float)y; g_ilPanH = (float)h; }
    }
  }
  inline void WindowRect( int& x, int& y, int& w, int& h ) {
    if( g_full ) {
      FullRect( x, y, w, h );
      return;
    }
    int sw, sh;
    ScreenSizePx( sw, sh );
    const float s = UiScale();
    w = (int)( (float)g_size * s + 0.5f );
    h = ( g_sizeY > 0 ) ? (int)( (float)g_sizeY * s + 0.5f ) : w;
    if( w > sw - 8 ) w = sw - 8;
    if( h > sh - 8 ) h = sh - 8;
    if( w < 16 ) w = 16;
    if( h < 16 ) h = 16;
    const int m  = (int)( (float)g_margin * s + 0.5f );
    switch( g_corner ) {
      case CORNER_TOPLEFT:     x = m;          y = m;          break;
      case CORNER_BOTTOMRIGHT: x = sw - m - w; y = sh - m - h; break;
      case CORNER_BOTTOMLEFT:  x = m;          y = sh - m - h; break;
      case CORNER_CUSTOM:      x = g_posX;     y = g_posY;     break;
      default:                 x = sw - m - w; y = m;          break;
    }
    if( x < 0 ) x = 0;
    if( y < 0 ) y = 0;
    if( x + w > sw ) x = sw - w;
    if( y + h > sh ) y = sh - h;
  }
  inline zCOLOR ColBack()   { const unsigned char v = (unsigned char)g_backLevel;
                              return zCOLOR( v, v, (unsigned char)( v + 4 > 255 ? 255 : v + 4 ) ); }
  inline zCOLOR ColGold()   { return zCOLOR( 214, 178, 106, 255 ); }
  inline zCOLOR ColDim()    { return zCOLOR( 118, 113, 106 ); }
  inline zCOLOR ColWay()    { return zCOLOR( 150, 128, 96, 190 ); }
  static const unsigned char kColDef[COLK_COUNT][3] = {
    {255,236,170}, {230, 60, 50}, { 80,210, 90}, {225,225,220}, {255,160,110}, {190,160,230},
    { 70,150,245}, { 60,215,220}, {250,140, 30}, {180,110, 60}, {150,160,175}, {245,100,190},
    {190,235, 40}, {160, 90,240}, {245,200, 40}, {185, 20, 55}, { 35, 75,235}, {150, 40,170},
    { 85, 85, 90}, {240,  0,240}, {255,130,255}, {150,215,255}, {  0,165,125}, {255, 85,  0},
    {120,255,200}, {200,190,160}, { 30,235, 30}, {225,225,105}, {135, 75,  0}, {255,250,  0},
    {  0,205,255}, {  0,120,255}, {150,150,  0}, {170,  0,  0}
  };
  inline zCOLOR ColPick( int k ) {
    if( k < 0 || k >= COLK_COUNT ) k = 0;
    const int m = g_colMenu[k];
    if( m > 0 && m < 12 ) return zCOLOR( kColPal[m][0], kColPal[m][1], kColPal[m][2], 255 );
    return g_colOn[k] ? g_colOv[k] : zCOLOR( kColDef[k][0], kColDef[k][1], kColDef[k][2], 255 );
  }
  inline zCOLOR ColPlayer()        { return ColPick( COLK_PLAYER ); }         inline zCOLOR ColFoe()           { return ColPick( COLK_ENEMY ); }
  inline zCOLOR ColFriend()        { return ColPick( COLK_FRIEND ); }         inline zCOLOR ColNeutral()       { return ColPick( COLK_NEUTRAL ); }
  inline zCOLOR ColItem()          { return ColPick( COLK_ITEM ); }           inline zCOLOR ColItem1H()        { return ColPick( COLK_ITEM_1H ); }
  inline zCOLOR ColItem2H()        { return ColPick( COLK_ITEM_2H ); }        inline zCOLOR ColItemRanged()    { return ColPick( COLK_ITEM_RANGED ); }
  inline zCOLOR ColItemArmor()     { return ColPick( COLK_ITEM_ARMOR ); }      inline zCOLOR ColItemPotion()    { return ColPick( COLK_ITEM_POTION ); }
  inline zCOLOR ColItemPlant()     { return ColPick( COLK_ITEM_PLANT ); }      inline zCOLOR ColItemMagic()     { return ColPick( COLK_ITEM_MAGIC ); }
  inline zCOLOR ColChest()         { return ColPick( COLK_CHEST_OPEN ); }      inline zCOLOR ColChestKey()      { return ColPick( COLK_CHEST_KEY ); }
  inline zCOLOR ColChestPick()     { return ColPick( COLK_CHEST_LOCKPICK ); }  inline zCOLOR ColChestBoth()     { return ColPick( COLK_CHEST_BOTH ); }
  inline zCOLOR ColChestSealed()   { return ColPick( COLK_CHEST_SEALED ); }    inline zCOLOR ColSwitch()        { return ColPick( COLK_SWITCH ); }
  inline zCOLOR ColSwitchTrig()    { return ColPick( COLK_SWITCH_TARGET ); }   inline zCOLOR ColPlaceShrine()   { return ColPick( COLK_SHRINE ); }
  inline zCOLOR ColPlaceBoard()    { return ColPick( COLK_BOARD ); }          inline zCOLOR ColPlaceCraft()    { return ColPick( COLK_FORGE ); }
  inline zCOLOR ColPlaceOre()      { return ColPick( COLK_ORE ); }            inline zCOLOR ColPlaceOther()    { return ColPick( COLK_OTHER_PLACE ); }
  inline zCOLOR ColPlaceAlchemy()  { return ColPick( COLK_ALCHEMY ); }        inline zCOLOR ColPlaceRune()     { return ColPick( COLK_RUNE ); }
  inline zCOLOR ColPlaceScroll()   { return ColPick( COLK_SCROLL ); }         inline zCOLOR ColPlaceDemon()    { return ColPick( COLK_DEMON ); }
  inline zCOLOR ColPlaceKitchen()  { return ColPick( COLK_KITCHEN ); }        inline zCOLOR ColPlaceWorkshop() { return ColPick( COLK_WORKSHOP ); }
  inline zCOLOR ColPlaceTreasure() { return ColPick( COLK_TREASURE ); }       inline zCOLOR ColExit()          { return ColPick( COLK_EXIT ); }
  inline zCOLOR ColTrader()        { return ColPick( COLK_TRADER ); }         inline zCOLOR ColCorpse()        { return ColPick( COLK_CORPSE ); }
  struct TScreenState {
    zCFont* font;
    zCOLOR  color;
    bool    valid;
    TScreenState() : font( 0 ), color(), valid( false ) {
      if( !screen ) return;
      font  = screen->font;
      color = screen->fontColor;
      valid = true;
    }
    ~TScreenState() {
      if( !valid || !screen ) return;
      screen->font      = font;
      screen->fontColor = color;
    }
  };
  inline bool ClipSegment( float& x0, float& y0, float& x1, float& y1 ) {
    float t0 = 0.0f, t1 = 1.0f;
    const float dx = x1 - x0;
    const float dy = y1 - y0;
    for( int side = 0; side < 4; side++ ) {
      float p = 0.0f, q = 0.0f;
      switch( side ) {
        case 0: p = -dx; q = x0 - g_clipX0; break;
        case 1: p =  dx; q = g_clipX1 - x0; break;
        case 2: p = -dy; q = y0 - g_clipY0; break;
        default: p = dy; q = g_clipY1 - y0; break;
      }
      if( p == 0.0f ) {
        if( q < 0.0f ) return false;
        continue;
      }
      const float r = q / p;
      if( p < 0.0f ) {
        if( r > t1 ) return false;
        if( r > t0 ) t0 = r;
      }
      else {
        if( r < t0 ) return false;
        if( r < t1 ) t1 = r;
      }
    }
    const float ax = x0 + t0 * dx;
    const float ay = y0 + t0 * dy;
    const float bx = x0 + t1 * dx;
    const float by = y0 + t1 * dy;
    x0 = ax; y0 = ay; x1 = bx; y1 = by;
    return true;
  }
  inline bool ClipDisc( float& x0, float& y0, float& x1, float& y1 );
  inline void PLine( float x0, float y0, float x1, float y1, const zCOLOR& c ) {
    if( !screen ) return;
    if( !ClipSegment( x0, y0, x1, y1 ) ) return;
    if( g_shape && !ClipDisc( x0, y0, x1, y1 ) ) return;
    screen->Line( VX( x0 ), VY( y0 ), VX( x1 ), VY( y1 ), c );
  }
  inline void RawLine( float x0, float y0, float x1, float y1, const zCOLOR& c ) {
    if( !screen ) return;
    screen->Line( VX( x0 ), VY( y0 ), VX( x1 ), VY( y1 ), c );
  }
  inline void FillPx( int x, int y, int w, int h, const zCOLOR& c ) {
    if( !screen || w <= 0 || h <= 0 ) return;
    int rows = h;
    if( rows > 2048 ) rows = 2048;
    const int vx0 = VX( (float)x );
    const int vx1 = VX( (float)( x + w ) );
    for( int k = 0; k <= rows; k++ ) {
      const int vy = VY( (float)y + (float)( h * k ) / (float)rows );
      screen->Line( vx0, vy, vx1, vy, c );
    }
  }
  inline int TextWidth( const char* text ) {
    if( !screen ) return 0;
    zSTRING t( text );
    return screen->FontSize( t );
  }
  inline void EnsureBlitter() {
    if( g_blit ) return;
    g_blit = new zCViewDraw();
    if( !g_blit ) {
      MM_LOG( "could not create the map view" );
      return;
    }
    if( g_blit->ViewParent )
      MM_LOG( "warning: map view has a parent" );
  }
}
