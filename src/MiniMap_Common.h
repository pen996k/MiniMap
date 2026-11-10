#pragma once
namespace MiniMapCommon {
  inline constexpr unsigned char PackLit( float v ) {
    const int k = (int)( ( v < 0.0f ? 0.0f : ( v > 1.0f ? 1.0f : v ) ) * 254.0f + 1.5f );
    return (unsigned char)( k < 1 ? 1 : ( k > 255 ? 255 : k ) );
  }
  inline constexpr float UnpackLit( unsigned char b ) {
    return b ? (float)( (int)b - 1 ) * ( 1.0f / 254.0f ) : 0.0f;
  }
  inline bool CanAllocGrid( int n, int bpp ) {
    if( sizeof( void* ) == 4 && n > 8192 ) return false;
    MEMORYSTATUSEX ms = { sizeof( ms ) };
    if( !GlobalMemoryStatusEx( &ms ) ) return false;
    const unsigned __int64 head = ( ms.ullTotalVirtual <= ( 2100ull << 20 ) ) ? ( 384ull << 20 ) : ( 768ull << 20 );
    const unsigned __int64 nn = (unsigned __int64)n * (unsigned __int64)n;
    const int pb = ( bpp > 0 ) ? bpp : 4;
    if( ms.ullAvailVirtual < nn * (unsigned __int64)( 8 + pb ) + head ) return false;
    const SIZE_T b1 = (SIZE_T)( nn * 4 ), b2 = (SIZE_T)( nn * (unsigned __int64)pb ), b3 = (SIZE_T)( nn * 4 );
    void* p1 = VirtualAlloc( 0, b1, MEM_RESERVE, PAGE_NOACCESS );
    void* p2 = p1 ? VirtualAlloc( 0, b2, MEM_RESERVE, PAGE_NOACCESS ) : 0;
    void* p3 = p2 ? VirtualAlloc( 0, b3, MEM_RESERVE, PAGE_NOACCESS ) : 0;
    if( p3 ) VirtualFree( p3, 0, MEM_RELEASE );
    if( p2 ) VirtualFree( p2, 0, MEM_RELEASE );
    if( p1 ) VirtualFree( p1, 0, MEM_RELEASE );
    return p1 && p2 && p3;
  }
  inline unsigned int FnvHashInts( const int* vals, int count, unsigned int h = 2166136261u ) {
    for( int i = 0; i < count; i++ ) { h ^= (unsigned int)vals[i]; h *= 16777619u; }
    return h;
  }
  inline void TrimHistRange( const int* h, int bins, int cut, int& lo, int& hi ) {
    lo = 0; hi = bins - 1;
    for( int s = 0; lo < bins - 1 && s + h[lo] <= cut; lo++ ) s += h[lo];
    for( int s = 0; hi > 0 && s + h[hi] <= cut; hi-- ) s += h[hi];
  }
  inline void SortNamedCounts( int n, int stride, char* names, char* norms, int* counts ) {
    char t[128];
    if( stride <= 0 || stride > (int)sizeof( t ) ) return;
    for( int i = 1; i < n; i++ )
      for( int j = i; j > 0 && strcmp( norms + ( j - 1 ) * stride, norms + j * stride ) > 0; j-- ) {
        memcpy( t, names + j * stride, (size_t)stride );
        memcpy( names + j * stride, names + ( j - 1 ) * stride, (size_t)stride );
        memcpy( names + ( j - 1 ) * stride, t, (size_t)stride );
        memcpy( t, norms + j * stride, (size_t)stride );
        memcpy( norms + j * stride, norms + ( j - 1 ) * stride, (size_t)stride );
        memcpy( norms + ( j - 1 ) * stride, t, (size_t)stride );
        std::swap( counts[j], counts[j - 1] );
      }
  }
  inline constexpr bool IsAsciiAlpha( char c ) {
    return ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' );
  }
  inline const char* SkipG1Prefix( const char* s ) {
    if( s && ( s[0] == 'S' || s[0] == 's' ) && s[1] == ':' )
      for( int i = 2; s[i]; i++ )
        if( s[i] == '_' ) return s + i + 1;
    return s;
  }
  inline bool GenHasWord( const char* s, const char* w, bool atStart = false, bool atEnd = false, int wlen = -1 ) {
    if( !s || !w ) return false;
    if( wlen < 0 ) wlen = (int)strlen( w );
    if( wlen <= 0 ) return false;
    for( int i = 0; s[i]; i++ ) {
      if( atStart && i > 0 && IsAsciiAlpha( s[i - 1] ) ) continue;
      if( _strnicmp( s + i, w, (size_t)wlen ) == 0 ) {
        if( atEnd && IsAsciiAlpha( s[i + wlen] ) ) continue;
        return true;
      }
      if( !s[i + 1] ) break;
    }
    return false;
  }
  inline bool GenHasAnyWord( const char* s, const char* list, bool atStart = false, bool atEnd = false ) {
    if( !s || !*s || !list ) return false;
    for( const char* p = list; *p; ) {
      const char* e = strchr( p, '|' );
      const int n = e ? (int)( e - p ) : (int)strlen( p );
      if( n > 0 && GenHasWord( s, p, atStart, atEnd, n ) ) return true;
      if( !e ) break;
      p = e + 1;
    }
    return false;
  }
  inline constexpr int GEN_PATHS_DEF = 0;
  inline constexpr int ANCHOR_TOPLEFT     = 0;
  inline constexpr int ANCHOR_TOPRIGHT    = 1;
  inline constexpr int ANCHOR_BOTTOMLEFT  = 2;
  inline constexpr int ANCHOR_BOTTOMRIGHT = 3;
  inline constexpr int ANCHOR_CUSTOM      = 4;
  inline constexpr int CORNER_TOPRIGHT    = 0;
  inline constexpr int CORNER_TOPLEFT     = 1;
  inline constexpr int CORNER_BOTTOMRIGHT = 2;
  inline constexpr int CORNER_BOTTOMLEFT  = 3;
  inline constexpr int CORNER_CUSTOM      = 4;
  inline constexpr int NAME_MAX  = 64;
  inline constexpr int MAP_SLOTS = 32;
  inline constexpr int AnchorToCorner( int anchor, int customVal ) {
    switch( anchor ) {
      case ANCHOR_TOPLEFT:     return CORNER_TOPLEFT;
      case ANCHOR_BOTTOMLEFT:  return CORNER_BOTTOMLEFT;
      case ANCHOR_BOTTOMRIGHT: return CORNER_BOTTOMRIGHT;
      case ANCHOR_CUSTOM:      return customVal;
      default:                 return CORNER_TOPRIGHT;
    }
  }
  inline constexpr int MARK_ICONS = 10;
  inline constexpr int kZoomLadderM[] = {
    10, 15, 20, 30, 40, 50, 60, 75, 100, 150, 200, 300, 400, 500, 750,
    1000, 1500, 2000, 3000, 4000, 5000
  };
  inline constexpr int kZoomLadderN = (int)( sizeof( kZoomLadderM ) / sizeof( kZoomLadderM[0] ) );
  inline constexpr int kSizePct[12] = { 10, 12, 14, 16, 18, 20, 22, 25, 28, 32, 36, 40 };
  inline constexpr int kSizeLevels  = 12;
  inline int SizeLevelPx( int lvl ) {
    if( lvl < 0 )            lvl = 0;
    if( lvl >= kSizeLevels ) lvl = kSizeLevels - 1;
    return ( kSizePct[lvl] * 1080 + 50 ) / 100;
  }
  inline constexpr int MD_DERIVED = -1;
  inline constexpr int kCursorSpeedPct[6] = { 100, 150, 200, 300, 400, 500 };
  inline constexpr int CURSOR_LVL_DEF = 2;
  inline int CursorSpeedPct( int lvl ) {
    if( lvl < 0 ) lvl = 0; else if( lvl > 5 ) lvl = 5;
    return kCursorSpeedPct[lvl];
  }
  inline int CursorSpeedLvlOf( int pct ) {
    int best = CURSOR_LVL_DEF, bd = 1 << 30;
    for( int i = 0; i < 6; i++ ) {
      const int d = ( pct > kCursorSpeedPct[i] ) ? pct - kCursorSpeedPct[i] : kCursorSpeedPct[i] - pct;
      if( d < bd ) { bd = d; best = i; }
    }
    return best;
  }
  inline constexpr int kCursorSpeedOldPct[7] = { 50, 75, 100, 125, 150, 200, 300 };
  inline constexpr int kIconSizePct[10] = { 50, 75, 100, 125, 150, 175, 200, 250, 300, 400 };
  inline constexpr int ICON_SIZE_DEF = 3;
  inline int IconSizeIdx( int v ) {
    return ( v < 0 || v > 9 ) ? ICON_SIZE_DEF : v;
  }
  inline float IconSizeK( int idx ) {
    return (float)kIconSizePct[IconSizeIdx( idx )] * 0.01f;
  }
  inline constexpr unsigned char kColPal[12][3] = {
    {   0,   0,   0 },
    { 230,  60,  50 },
    { 250, 140,  30 },
    { 255, 230,  40 },
    {  80, 210,  90 },
    {  60, 215, 220 },
    {  70, 150, 245 },
    { 160,  90, 240 },
    { 245, 100, 190 },
    { 235, 235, 230 },
    { 150, 150, 150 },
    { 180, 110,  60 },
  };
  struct TMenuDef {
    const char* key;
    int         count;
    int         def;
  };
  inline constexpr TMenuDef kMenuDefs[] = {
    { "Enabled",       2, 1 },
    { "Shape",         2, 0 },
    { "SizeLevel",    12, MD_DERIVED },
    { "Corner",        4, MD_DERIVED },
    { "Rotate",        2, 0 },
    { "ShowNpc",       3, 0 },
    { "ShowNpcKind",   4, 0 },
    { "ShowItems",     3, 0 },
    { "ShowScale",     3, 1 },
    { "AlignIdx",      5, MD_DERIVED },
    { "FullAlignIdx",  5, MD_DERIVED },
    { "ItemColors",    2, 1 },
    { "ShowChests",    3, 2 },
    { "ShowSwitches",  3, 2 },
    { "ShowPlaces",    4, 2 },
    { "ShowFurniture", 2, 0 },
    { "ShowCraft",     2, 1 },
    { "ShowAlchemy",   2, 1 },
    { "ShowRunes",     2, 1 },
    { "ShowScrolls",   2, 1 },
    { "ShowDemon",     2, 1 },
    { "ShowKitchen",   2, 1 },
    { "ShowWorkshop",  2, 1 },
    { "ShowLadders",   2, 1 },
    { "ShowFire",      2, 0 },
    { "ShowClutter",   2, 0 },
    { "Language",      4, MD_DERIVED },
    { "Log",           2, 1 },
    { "GenDetailLvl",  3, MD_DERIVED },
    { "GenSmooth",     3, 0 },
    { "GenSculpt",     3, 1 },
    { "GenFillHoles",  2, 1 },
    { "GenOutside",    4, MD_DERIVED },
    { "GenPaths",      2, MD_DERIVED },
    { "GenIndoorLvl",  4, MD_DERIVED },
    { "GenTexColors",  3, 1 },
    { "ShowDetailBar", 2, 0 },
    { "DetailSpeed",   5, 0 },
    { "DetailQuality", 4, 0 },
    { "GenTreeShapes", 4, 3 },
    { "FullMap",       2, 1 },
    { "WheelMini",     2, MD_DERIVED },
    { "WheelFull",     3, MD_DERIVED },
    { "FullCenter",    2, 1 },
    { "Marks",         2, 0 },
    { "HoverInfo",     4, 1 },
    { "HoverDetail",   3, 1 },
    { "SearchOthers",  2, 0 },
    { "FullPause",     2, 1 },
    { "MarkGuide",     3, 0 },
    { "CursorSpeedLvl", 6, MD_DERIVED },
    { "IconSet",       4, MD_DERIVED },
    { "IconSizeNpc",  10, MD_DERIVED },
    { "IconSizeItem", 10, MD_DERIVED },
    { "IconSizeMark", 10, MD_DERIVED },
    { "ColEnemy",     12, 0 },
    { "ColNeutral",   12, 0 },
    { "ColFriend",    12, 0 },
    { "ColPlayer",    12, 0 },
    { "ColTrader",    12, 0 },
    { "ColCorpse",    12, 0 },
    { "ColItem1H",    12, 0 },
    { "ColItem2H",    12, 0 },
    { "ColRanged",    12, 0 },
    { "ColPotion",    12, 0 },
    { "ColPlant",     12, 0 },
    { "ColMagic",     12, 0 },
    { "MiniStyle",     4, 0 },
    { "FullSize",      5, 4 },
    { "FullStyle",     4, 0 },
    { "FullScreen",    2, 0 },
    { "PlayerRing",    2, 1 },
    { "ShowExits",     3, 2 },
    { "ShowTraders",   3, 2 },
    { "ShowCorpses",   3, 1 },
    { "HoverInv",      3, 0 },
    { "SearchInv",     3, 0 },
    { "FullEdgeScroll", 2, 0 },
    { "FullDragMap",   2, 1 },
    { "HideEmpty",     2, 0 },
    { "ChestCombo",    3, 0 },
    { "ItemList",      2, 0 },
    { "ShowTeleports", 2, 1 },
    { "Fog",           2, 0 },
    { "ExportZoom",    3, 0 },
    { "ExportFormat",  3, 2 },
    { "ExportQuality", 4, 2 },
    { "ExpChests",     2, MD_DERIVED },
    { "ExpItems",      2, MD_DERIVED },
    { "ExpPlants",     2, MD_DERIVED },
    { "ExpNpcs",       2, MD_DERIVED },
    { "ExpFoes",       2, MD_DERIVED },
    { "ExpFriends",    2, MD_DERIVED },
    { "ExpSwitches",   2, MD_DERIVED },
    { "ExpPlaces",     2, MD_DERIVED },
    { "ExpExits",      2, MD_DERIVED },
    { "ExpTeleports",  2, MD_DERIVED },
    { "ExpMarks",      2, MD_DERIVED },
    { "ExpPlayer",     2, MD_DERIVED }
  };
  inline constexpr int kMenuDefCount = (int)( sizeof( kMenuDefs ) / sizeof( kMenuDefs[0] ) );
  inline constexpr int ICON_ATLAS = 512;
  inline constexpr int ICON_SLOTS = 43;
  inline constexpr int ICON_CELLS = 6;
  inline constexpr int ICON_PLAYER = ICON_SLOTS - 1;
  inline constexpr int ICON_COUNT = 42;
  enum {
    ICON_ITEM_OTHER = 3, ICON_ITEM_1H = 4, ICON_ITEM_2H = 5, ICON_ITEM_RANGED = 6,
    ICON_ITEM_ARMOR = 7, ICON_ITEM_POTION = 8, ICON_ITEM_PLANT = 9,
    ICON_ITEM_MAGIC = 10, ICON_CHEST = 11, ICON_CHEST_KEY = 12,
    ICON_CHEST_PICK = 13, ICON_CHEST_BOTH = 14, ICON_CHEST_SEALED = 15,
    ICON_LEVER = 16, ICON_WHEEL = 17, ICON_TRIGGER = 18,
    ICON_SHRINE = 19, ICON_BOARD = 20, ICON_CRAFT = 21, ICON_ORE = 22,
    ICON_TREASURE = 23, ICON_BED = 24, ICON_CHAIR = 25, ICON_BENCH = 26,
    ICON_THRONE = 27, ICON_FIRE = 28,
    ICON_EXIT = 29,
    ICON_TRADER = 30, ICON_CORPSE = 31,
    ICON_TELEPORT = 32,
    ICON_ANIMAL = 33,
    ICON_ALCHEMY = 34, ICON_KITCHEN = 35, ICON_WORKSHOP = 36,
    ICON_LADDER = 37,
    ICON_RUNE = 38, ICON_SCROLL = 39, ICON_DEMON = 40,
    ICON_INTERACT = 41,
    PLACE_OTHER = 50, PLACE_REST = 51
  };
  inline int IconShapeOf( int slot ) {
    if( slot == 2 ) return 1;
    return ( slot >= 4 && slot <= 10 ) ? 3 : slot;
  }
  inline float IconSegD2( float u, float v, float ax, float ay, float bx, float by ) {
    const float dx = bx - ax, dy = by - ay;
    float t = ( ( u - ax ) * dx + ( v - ay ) * dy ) / ( dx * dx + dy * dy );
    if( t < 0.0f ) t = 0.0f; else if( t > 1.0f ) t = 1.0f;
    const float ex = u - ( ax + t * dx ), ey = v - ( ay + t * dy );
    return ex * ex + ey * ey;
  }
  inline float IconStarD( float u, float v ) {
    static float px[5], py[5];
    static bool  init = false;
    if( !init ) {
      for( int k = 0; k < 5; k++ ) {
        const float a = -1.5707963f + (float)k * 1.2566371f;
        px[k] = 0.70f * cosf( a );
        py[k] = 0.70f * sinf( a );
      }
      init = true;
    }
    float best = 1.0e9f;
    for( int k = 0; k < 5; k++ ) {
      const int j = ( k + 2 ) % 5;
      const float d = IconSegD2( u, v, px[k], py[k], px[j], py[j] );
      if( d < best ) best = d;
    }
    return sqrtf( best );
  }
  inline bool IconHull( int icon, float u, float v ) {
    switch( icon ) {
      case 0: {
        const float dy = v + 0.2f;
        if( u * u + dy * dy <= 0.68f * 0.68f ) return true;
        return ( u >= -0.40f && u <= 0.40f && v >= 0.05f && v <= 0.82f );
      }
      case 1: {
        const float hy = v + 0.36f;
        if( u * u + hy * hy <= 0.40f * 0.40f ) return true;
        if( v < 0.20f || v > 0.86f ) return false;
        const float eu = u / 0.84f, ev = ( v - 0.86f ) / 0.66f;
        return eu * eu + ev * ev <= 1.0f;
      }
      case 2: {
        const float ex = ( ( u < 0.0f ) ? -u : u ) - 0.55f, ey = v + 0.34f;
        if( ex * ex + ey * ey <= 0.26f * 0.26f ) return true;
        if( v < 0.16f || v > 0.80f ) return false;
        const float eu = u / 0.82f, ev = ( v - 0.80f ) / 0.64f;
        return eu * eu + ev * ev <= 1.0f;
      }
      case 3: {
        const float dy = v - 0.22f;
        if( u * u + dy * dy <= 0.62f * 0.62f ) return true;
        if( u >= -0.20f && u <= 0.20f && v >= -0.50f && v <= -0.15f ) return true;
        const float eu = u / 0.42f, ev = ( v + 0.58f ) / 0.20f;
        return eu * eu + ev * ev <= 1.0f;
      }
      case 12: case 13: case 14: case 15:
      case 11: {
        if( u >= -0.80f && u <= 0.80f && v >= -0.10f && v <= 0.62f ) return true;
        if( v > -0.10f || v < -0.62f ) return false;
        const float eu = u / 0.80f, ev = ( v + 0.10f ) / 0.52f;
        return eu * eu + ev * ev <= 1.0f;
      }
      case 16: {
        if( u >= -0.70f && u <= 0.70f && v >= 0.46f && v <= 0.84f ) return true;
        const float kx = u - 0.42f, ky = v + 0.52f;
        if( kx * kx + ky * ky <= 0.28f * 0.28f ) return true;
        return IconSegD2( u, v, -0.05f, 0.52f, 0.42f, -0.52f ) <= 0.15f * 0.15f;
      }
      case 17: {
        const float r2 = u * u + v * v;
        if( r2 > 0.86f * 0.86f ) return false;
        if( r2 >= 0.58f * 0.58f || r2 <= 0.24f * 0.24f ) return true;
        const float au = ( u < 0.0f ) ? -u : u, av = ( v < 0.0f ) ? -v : v;
        return au <= 0.10f || av <= 0.10f;
      }
      case 18: {
        const float au = ( u < 0.0f ) ? -u : u, av = ( v < 0.0f ) ? -v : v;
        return au + av <= 0.86f;
      }
      case 19: {
        const float au = ( u < 0.0f ) ? -u : u;
        if( au <= 0.70f && v >= 0.56f && v <= 0.86f ) return true;
        const float hy = v + 0.50f;
        if( u * u + hy * hy <= 0.22f * 0.22f ) return true;
        if( v >= -0.24f && v <= 0.56f && au <= 0.20f + ( v + 0.24f ) * 0.25f ) return true;
        return IconSegD2( au, v, 0.14f, -0.14f, 0.58f, -0.66f ) <= 0.11f * 0.11f;
      }
      case 20: {
        if( u >= -0.84f && u <= 0.84f && v >= -0.74f && v <= 0.30f ) return true;
        return u >= -0.13f && u <= 0.13f && v >= 0.30f && v <= 0.86f;
      }
      case 21: {
        if( u >= -0.55f && u <= 0.76f && v >= -0.44f && v <= -0.10f ) return true;
        if( u >= -0.88f && u < -0.55f && v >= -0.44f
            && v <= -0.10f - ( -0.55f - u ) * ( 0.24f / 0.33f ) ) return true;
        if( u >= -0.24f && u <= 0.40f && v >= -0.10f && v <= 0.44f ) return true;
        return u >= -0.58f && u <= 0.72f && v >= 0.44f && v <= 0.76f;
      }
      case 22: {
        const float au = ( u < 0.0f ) ? -u : u;
        if( au <= 0.34f && v <= 0.70f && v >= -0.86f + 1.25f * au ) return true;
        const float as = ( au - 0.56f < 0.0f ) ? 0.56f - au : au - 0.56f;
        if( as <= 0.22f && v <= 0.70f && v >= -0.24f + 1.4f * as ) return true;
        const float ey = ( v - 0.70f ) / 0.17f, ex = u / 0.86f;
        return ex * ex + ey * ey <= 1.0f;
      }
      case 23: {
        if( IconSegD2( u, v, -0.60f, -0.60f, 0.60f, 0.60f ) <= 0.19f * 0.19f ) return true;
        return IconSegD2( u, v, -0.60f, 0.60f, 0.60f, -0.60f ) <= 0.19f * 0.19f;
      }
      case 24: {
        if( u >= -0.82f && u <= 0.82f && v >= -0.02f && v <= 0.30f ) return true;
        if( u >= -0.82f && u <= -0.60f && v >= -0.56f && v <= 0.62f ) return true;
        if( u >= 0.62f && u <= 0.82f && v >= -0.20f && v <= 0.62f ) return true;
        const float ex = ( u + 0.32f ) / 0.24f, ey = ( v + 0.10f ) / 0.15f;
        return ex * ex + ey * ey <= 1.0f;
      }
      case 25: {
        if( u >= -0.52f && u <= -0.32f && v >= -0.84f && v <= 0.82f ) return true;
        if( u >= -0.52f && u <= 0.50f && v >= -0.02f && v <= 0.18f ) return true;
        return u >= 0.30f && u <= 0.50f && v >= 0.18f && v <= 0.82f;
      }
      case 26: {
        const float au = ( u < 0.0f ) ? -u : u;
        if( au <= 0.86f && v >= 0.00f && v <= 0.20f ) return true;
        if( au <= 0.86f && v >= -0.52f && v <= -0.34f ) return true;
        if( au >= 0.62f && au <= 0.80f && v >= -0.52f && v <= 0.00f ) return true;
        return au >= 0.56f && au <= 0.74f && v >= 0.20f && v <= 0.66f;
      }
      case 27: {
        const float au = ( u < 0.0f ) ? -u : u;
        if( au <= 0.46f && v >= -0.66f && v <= 0.10f ) return true;
        if( v < -0.66f ) {
          const float ex = u / 0.46f, ey = ( v + 0.66f ) / 0.22f;
          if( ex * ex + ey * ey <= 1.0f ) return true;
        }
        if( au >= 0.46f && au <= 0.78f && v >= -0.24f && v <= 0.30f ) return true;
        if( au <= 0.78f && v >= 0.10f && v <= 0.34f ) return true;
        return au >= 0.56f && au <= 0.78f && v >= 0.34f && v <= 0.82f;
      }
      case 28: {
        const float au = ( u < 0.0f ) ? -u : u;
        if( au <= 0.80f && v >= 0.60f && v <= 0.80f ) return true;
        const float cy = v - 0.18f;
        if( u * u + cy * cy <= 0.44f * 0.44f ) return true;
        return v >= -0.86f && v < 0.18f && au <= 0.44f * ( v + 0.86f ) / 1.04f;
      }
      case 29: {
        const float au = ( u < 0.0f ) ? -u : u;
        if( au <= 0.78f && v >= -0.10f && v <= 0.84f ) return true;
        const float ey = ( v + 0.10f ) / 0.74f, ex = u / 0.78f;
        return v < -0.10f && ex * ex + ey * ey <= 1.0f;
      }
      case 30: {
        const float au = ( u < 0.0f ) ? -u : u;
        if( au <= 0.08f && v >= -0.76f && v <= 0.80f ) return true;
        if( au <= 0.46f && v >= 0.66f && v <= 0.86f ) return true;
        if( au <= 0.86f && v >= -0.74f && v <= -0.54f ) return true;
        const float bx = au - 0.56f, by = v - 0.10f;
        if( v >= 0.10f && bx * bx + by * by <= 0.30f * 0.30f ) return true;
        return IconSegD2( au, v, 0.56f, -0.54f, 0.86f, 0.10f ) <= 0.07f * 0.07f
            || IconSegD2( au, v, 0.56f, -0.54f, 0.26f, 0.10f ) <= 0.07f * 0.07f;
      }
      case 32: {
        const float r2 = u * u + v * v;
        if( r2 > 0.86f * 0.86f ) return false;
        if( r2 >= 0.70f * 0.70f ) return true;
        return IconStarD( u, v ) <= 0.075f;
      }
      case 33: {
        const float hu = u / 0.40f, hv = ( v - 0.28f ) / 0.56f;
        if( hu * hu + hv * hv <= 1.0f ) return true;
        const float au = ( u < 0.0f ) ? -u : u;
        if( v > -0.10f || au < 0.10f ) return false;
        if( au < 0.30f && v > -0.30f ) return false;
        const float rx = au - 0.30f, ry = v + 0.10f;
        const float r2 = rx * rx + ry * ry;
        return r2 >= 0.34f * 0.34f && r2 <= 0.56f * 0.56f;
      }
      case 34: {
        const float au = ( u < 0.0f ) ? -u : u;
        if( au <= 0.28f && v >= -0.86f && v <= -0.72f ) return true;
        if( au <= 0.15f && v >= -0.72f && v <= -0.30f ) return true;
        return v >= -0.30f && v <= 0.80f && au <= 0.15f + ( v + 0.30f ) * 0.60f;
      }
      case 35: {
        const float au = ( u < 0.0f ) ? -u : u;
        const float ky = v + 0.52f;
        if( u * u + ky * ky <= 0.13f * 0.13f ) return true;
        if( au <= 0.80f && v >= -0.40f && v <= -0.24f ) return true;
        if( au >= 0.60f && au <= 0.88f && v >= -0.12f && v <= 0.06f ) return true;
        if( au <= 0.64f && v >= -0.24f && v <= 0.40f ) return true;
        const float ex = u / 0.64f, ey = ( v - 0.40f ) / 0.40f;
        return v > 0.40f && ex * ex + ey * ey <= 1.0f;
      }
      case 36: {
        if( u >= -0.74f && u <= -0.28f && v >= -0.86f && v <= -0.36f ) return true;
        if( u >= -0.28f && u <= 0.40f && v >= -0.78f && v <= -0.44f ) return true;
        if( u >= 0.40f && u <= 0.76f && v >= -0.72f + ( u - 0.40f ) * 0.30f
            && v <= -0.50f - ( u - 0.40f ) * 0.30f ) return true;
        return u >= -0.12f && u <= 0.12f && v >= -0.44f && v <= 0.86f;
      }
      case 37: {
        const float au = ( u < 0.0f ) ? -u : u;
        if( au >= 0.36f && au <= 0.60f && v >= -0.86f && v <= 0.86f ) return true;
        if( au > 0.60f ) return false;
        return ( v >= -0.62f && v <= -0.44f ) || ( v >= -0.20f && v <= -0.02f )
            || ( v >= 0.22f && v <= 0.40f ) || ( v >= 0.64f && v <= 0.82f );
      }
      case 31: {
        const float au = ( u < 0.0f ) ? -u : u;
        if( au <= 0.17f && v >= -0.86f && v <= 0.86f ) return true;
        return au <= 0.56f && v >= -0.50f && v <= -0.18f;
      }
      case 38: {
        const float au = ( u < 0.0f ) ? -u : u, av = ( v < 0.0f ) ? -v : v;
        if( au > 0.62f || av > 0.84f ) return false;
        const float ex = au - 0.38f, ey = av - 0.60f;
        return ex <= 0.0f || ey <= 0.0f || ex * ex + ey * ey <= 0.24f * 0.24f;
      }
      case 39: {
        const float au = ( u < 0.0f ) ? -u : u;
        if( au <= 0.52f && v >= -0.62f && v <= 0.62f ) return true;
        const float ry = ( v < 0.0f ) ? v + 0.66f : v - 0.66f;
        return au <= 0.74f && ry * ry <= 0.18f * 0.18f;
      }
      case 40: {
        const float au = ( u < 0.0f ) ? -u : u;
        const float hy = ( v - 0.20f ) / 0.62f, hx = u / 0.56f;
        if( hx * hx + hy * hy <= 1.0f ) return true;
        if( v > -0.30f || v < -0.88f ) return false;
        const float t = ( -0.30f - v ) / 0.58f;
        const float c = 0.41f + t * 0.31f, w = 0.12f * ( 1.0f - t ) + 0.01f;
        return au >= c - w && au <= c + w;
      }
      case ICON_PLAYER: {
        const float au = ( u < 0.0f ) ? -u : u;
        if( v < -0.90f || v > 0.675f ) return false;
        if( au > ( v + 0.90f ) * ( 0.56f / 1.575f ) ) return false;
        return v <= 0.236f + au * ( ( 0.675f - 0.236f ) / 0.56f );
      }
      default:
        return u * u + v * v <= 0.62f * 0.62f;
    }
  }
  inline bool IconHole( int icon, float u, float v ) {
    const float au = ( u < 0.0f ) ? -u : u;
    switch( icon ) {
      case 0: {
        const float ex = au - 0.28f, ey = v + 0.12f;
        if( ex * ex + ey * ey <= 0.19f * 0.19f ) return true;
        if( v >= 0.10f && v <= 0.32f && au <= ( v - 0.10f ) * 0.6f ) return true;
        if( v >= 0.56f && v <= 0.84f
            && ( au <= 0.04f || ( au >= 0.17f && au <= 0.24f ) ) )
          return true;
        return false;
      }
      case 1: {
        const float ex = au - 0.15f, ey = v + 0.36f;
        return ex * ex + ey * ey <= 0.085f * 0.085f;
      }
      case 2:
        return au <= 0.05f && v >= 0.30f;
      case 3:
        return v >= -0.40f && v <= -0.30f && au <= 0.30f;
      case 11:
        if( v >= -0.16f && v <= -0.04f && au > 0.16f ) return true;
        return au <= 0.07f && v >= 0.06f && v <= 0.24f;
      case 12: {
        if( v >= -0.16f && v <= -0.04f && au > 0.16f ) return true;
        const float ex = u + 0.36f, ey = v - 0.26f;
        if( ex * ex + ey * ey <= 0.19f * 0.19f ) return true;
        if( v >= 0.19f && v <= 0.33f && u >= -0.20f && u <= 0.55f ) return true;
        return u >= 0.34f && u <= 0.55f && v >= 0.33f && v <= 0.50f;
      }
      case 13:
        if( v >= -0.16f && v <= -0.04f && au > 0.16f ) return true;
        if( v >= 0.32f && v <= 0.46f && u >= -0.60f && u <= 0.36f ) return true;
        return u >= 0.22f && u <= 0.38f && v >= 0.04f && v <= 0.46f;
      case 14: {
        if( v >= -0.16f && v <= -0.04f && au > 0.16f ) return true;
        const float ey = v - 0.18f;
        if( u * u + ey * ey <= 0.16f * 0.16f ) return true;
        return v >= 0.18f && v <= 0.52f && au <= 0.07f + ( v - 0.18f ) * 0.30f;
      }
      case 15:
        if( v >= -0.16f && v <= -0.04f ) return true;
        return v >= -0.04f && au >= 0.34f && au <= 0.50f;
      case 16:
        return v >= 0.60f && v <= 0.70f && au >= 0.20f && au <= 0.56f;
      case 18: {
        const float av = ( v < 0.0f ) ? -v : v;
        return au + av >= 0.36f && au + av <= 0.52f;
      }
      case 19:
        return v >= 0.66f && v <= 0.76f && au <= 0.50f;
      case 20:
        if( au > 0.60f ) return false;
        return ( v >= -0.56f && v <= -0.44f ) || ( v >= -0.30f && v <= -0.18f )
            || ( v >= -0.04f && v <= 0.08f && u <= 0.20f );
      case 21:
        return u >= 0.44f && u <= 0.60f && v >= -0.36f && v <= -0.20f;
      case 22:
        return au <= 0.05f && v >= -0.40f && v <= 0.50f;
      case 24:
        return v >= 0.09f && v <= 0.16f && u >= -0.08f && u <= 0.58f;
      case 27:
        return au <= 0.26f && v >= -0.62f && v <= -0.04f;
      case 28:
        return au <= 0.70f && v >= 0.60f && v <= 0.66f;
      case 29: {
        const bool inner = ( au <= 0.46f && v >= -0.10f && v <= 0.84f )
                        || ( v < -0.10f && ( u / 0.46f ) * ( u / 0.46f ) + ( ( v + 0.10f ) / 0.46f ) * ( ( v + 0.10f ) / 0.46f ) <= 1.0f );
        if( !inner ) return false;
        const bool arrow = ( au <= 0.11f && v >= -0.05f && v <= 0.62f )
                        || ( v >= -0.40f && v < -0.05f && au <= ( v + 0.40f ) * 0.86f );
        return !arrow;
      }
      case 30:
        return u * u + ( v + 0.64f ) * ( v + 0.64f ) <= 0.07f * 0.07f;
      case 31:
        return false;
      case 32:
        return false;
      case 34:
        return v >= 0.22f && v <= 0.32f && au <= 0.15f + ( v + 0.30f ) * 0.60f - 0.16f;
      case 35:
        return v >= 0.08f && v <= 0.18f && au <= 0.50f;
      case 38:
        return ( au <= 0.06f && v >= -0.56f && v <= 0.56f )
            || IconSegD2( u, v, 0.0f, -0.10f, 0.30f, -0.50f ) <= 0.06f * 0.06f
            || IconSegD2( u, v, 0.0f, 0.10f, -0.30f, 0.50f ) <= 0.06f * 0.06f;
      case 39:
        if( au > 0.36f ) return false;
        return ( v >= -0.36f && v <= -0.26f ) || ( v >= -0.05f && v <= 0.05f ) || ( v >= 0.26f && v <= 0.36f && u <= 0.10f );
      case 40: {
        const float ex = au - 0.22f, ey = v - 0.10f;
        return ex * ex + ey * ey <= 0.10f * 0.10f;
      }
      default:
        return false;
    }
  }
  inline constexpr int kIconCellS[ICON_CELLS] = { 48, 32, 24, 16, 12, 8 };
  inline constexpr int kIconCellY[ICON_CELLS] = {  0, 240, 336, 408, 440, 464 };
  inline void IconCell( int cell, int& x, int& y, int& s ) {
    if( cell < 0 ) cell = 0;
    if( cell >= ICON_CELLS ) cell = ICON_CELLS - 1;
    s = kIconCellS[cell];
    y = kIconCellY[cell];
    x = 0;
  }
  inline void IconSlotXY( int cell, int slot, int& x, int& y, int& s ) {
    IconCell( cell, x, y, s );
    const int perRow = ICON_ATLAS / s;
    x += ( slot % perRow ) * s;
    y += ( slot / perRow ) * s;
  }
  inline int IconCellFor( float s ) {
    for( int c = ICON_CELLS - 1; c >= 0; c-- )
      if( (float)kIconCellS[c] >= s - 0.25f ) return c;
    return 0;
  }
  struct TIconKey { const char* key; int slot; };
  inline constexpr TIconKey kIconFileKey[] = {
    { "item", ICON_ITEM_OTHER }, { "item_1h", ICON_ITEM_1H }, { "item_2h", ICON_ITEM_2H },
    { "item_ranged", ICON_ITEM_RANGED }, { "item_armor", ICON_ITEM_ARMOR }, { "item_potion", ICON_ITEM_POTION },
    { "item_plant", ICON_ITEM_PLANT }, { "item_magic", ICON_ITEM_MAGIC },
    { "chest_open", ICON_CHEST }, { "chest_key", ICON_CHEST_KEY }, { "chest_lockpick", ICON_CHEST_PICK },
    { "chest_both", ICON_CHEST_BOTH }, { "chest_sealed", ICON_CHEST_SEALED },
    { "lever", ICON_LEVER }, { "wheel", ICON_WHEEL }, { "switch_target", ICON_TRIGGER },
    { "shrine", ICON_SHRINE }, { "board", ICON_BOARD }, { "forge", ICON_CRAFT }, { "ore", ICON_ORE },
    { "treasure", ICON_TREASURE }, { "bed", ICON_BED }, { "chair", ICON_CHAIR }, { "bench", ICON_BENCH },
    { "throne", ICON_THRONE }, { "fire", ICON_FIRE }, { "exit", ICON_EXIT }, { "trader", ICON_TRADER },
    { "corpse", ICON_CORPSE }, { "teleport", ICON_TELEPORT }, { "alchemy", ICON_ALCHEMY },
    { "kitchen", ICON_KITCHEN }, { "workshop", ICON_WORKSHOP }, { "ladder", ICON_LADDER },
    { "rune", ICON_RUNE }, { "scroll", ICON_SCROLL }, { "demon", ICON_DEMON },
    { "interact", ICON_INTERACT }
  };
  inline constexpr int kIconFileKeyN = (int)( sizeof( kIconFileKey ) / sizeof( kIconFileKey[0] ) );
  inline int IconFileSlot( const char* key ) {
    for( int k = 0; k < kIconFileKeyN; k++ )
      if( MiniMapCfg::EqNoCase( kIconFileKey[k].key, key ) ) return kIconFileKey[k].slot;
    return -1;
  }
  inline constexpr int  MARK_ATLAS  = 256;
  inline void MarkCell( int cell, int icon, int& x, int& y, int& s ) {
    if( cell == 0 )      { s = 48; x = ( icon % 5 ) * 48; y = ( icon / 5 ) * 48; }
    else if( cell == 1 ) { s = 32; x = ( icon % 8 ) * 32; y = 96 + ( icon / 8 ) * 32; }
    else                 { s = 16; x = icon * 16;         y = 160; }
  }
  inline bool MarkStarIn( float u, float v ) {
    static float px[10], py[10];
    static bool  ready = false;
    if( !ready ) {
      for( int i = 0; i < 10; i++ ) {
        const float a  = -1.5707963f + (float)i * 0.62831853f;
        const float rr = ( i & 1 ) ? 0.37f : 0.90f;
        px[i] = cosf( a ) * rr;
        py[i] = sinf( a ) * rr + 0.06f;
      }
      ready = true;
    }
    bool in = false;
    for( int i = 0, j = 9; i < 10; j = i++ ) {
      if( ( py[i] > v ) != ( py[j] > v )
          && u < ( px[j] - px[i] ) * ( v - py[i] ) / ( py[j] - py[i] ) + px[i] )
        in = !in;
    }
    return in;
  }
  inline bool MarkHull( int icon, float u, float v ) {
    const float au = ( u < 0.0f ) ? -u : u;
    const float av = ( v < 0.0f ) ? -v : v;
    switch( icon ) {
      case 1: {
        if( au <= 0.86f && v >= -0.30f && v <= 0.72f ) return true;
        const float eu = u / 0.86f, ev = ( v + 0.30f ) / 0.45f;
        return v < -0.30f && eu * eu + ev * ev <= 1.0f;
      }
      case 2:
        if( v < -0.86f || v > 0.74f ) return false;
        return au <= ( v + 0.86f ) * 0.56f;
      case 3:
        return MarkStarIn( u, v );
      case 4:
        return u * u + v * v <= 0.72f * 0.72f;
      case 5:
        for( int m = 0; m < 2; m++ ) {
          const float x = m ? -u : u;
          if( IconSegD2( x, v, -0.80f, -0.80f, 0.40f, 0.40f ) <= 0.12f * 0.12f ) return true;
          if( IconSegD2( x, v,  0.16f,  0.64f, 0.64f, 0.16f ) <= 0.09f * 0.09f ) return true;
          if( IconSegD2( x, v,  0.40f,  0.40f, 0.72f, 0.72f ) <= 0.08f * 0.08f ) return true;
          const float gx = x - 0.78f, gy = v - 0.78f;
          if( gx * gx + gy * gy <= 0.12f * 0.12f ) return true;
        }
        return false;
      case 6:
        if( v >= -0.88f && v <= -0.12f && au <= ( v + 0.88f ) * 1.18f ) return true;
        return au <= 0.66f && v >= -0.14f && v <= 0.80f;
      case 7: {
        if( v > 0.78f ) return false;
        const float eu = u / 0.92f, ev = ( v - 0.78f ) / 1.55f;
        return eu * eu + ev * ev <= 1.0f;
      }
      case 8: {
        if( au <= 0.07f && v >= -0.35f && v <= 0.86f ) return true;
        {
          const float eu = u / 0.26f, ev = ( v + 0.50f ) / 0.40f;
          if( eu * eu + ev * ev <= 1.0f ) return true;
        }
        const float x = au - 0.36f, y = v - 0.14f;
        const float a = ( x - y ) * 0.7071f, b = ( x + y ) * 0.7071f;
        const float eu = a / 0.44f, ev = b / 0.21f;
        return eu * eu + ev * ev <= 1.0f;
      }
      case 9:
        return u * u + v * v <= 0.80f * 0.80f;
      default:
        if( u * u + v * v <= 0.70f * 0.70f ) return true;
        return ( au <= 0.11f && av <= 0.90f ) || ( av <= 0.11f && au <= 0.90f );
    }
  }
  inline bool MarkHole( int icon, float u, float v ) {
    const float au = ( u < 0.0f ) ? -u : u;
    switch( icon ) {
      case 1:
        return ( v >= -0.30f && v <= -0.17f && au <= 0.86f )
            || ( au <= 0.13f && v >= -0.36f && v <= 0.10f );
      case 2: {
        if( au <= 0.09f && v >= -0.38f && v <= 0.22f ) return true;
        const float dy = v - 0.46f;
        return u * u + dy * dy <= 0.11f * 0.11f;
      }
      case 4:
        return u * u + v * v <= 0.30f * 0.30f;
      case 6:
        return au <= 0.18f && v >= 0.30f && v <= 0.80f;
      case 7: {
        if( v > 0.78f ) return false;
        const float eu = u / 0.36f, ev = ( v - 0.78f ) / 0.74f;
        return eu * eu + ev * ev <= 1.0f;
      }
      case 9: {
        const float dy = v + 0.24f;
        const float r2 = u * u + dy * dy;
        if( r2 >= 0.19f * 0.19f && r2 <= 0.37f * 0.37f
            && ( dy <= 0.0f || ( u >= 0.0f && dy <= 0.16f ) ) )
          return true;
        if( IconSegD2( u, v, 0.20f, -0.08f, 0.0f, 0.12f ) <= 0.09f * 0.09f ) return true;
        if( au <= 0.09f && v >= 0.10f && v <= 0.30f ) return true;
        const float ey = v - 0.56f;
        return u * u + ey * ey <= 0.11f * 0.11f;
      }
      case 3:
      case 5:
      case 8:
        return false;
      default: {
        const float r2 = u * u + v * v;
        return r2 >= 0.22f * 0.22f && r2 <= 0.46f * 0.46f;
      }
    }
  }
  inline unsigned char CapFirstChar( unsigned char c ) {
    if( c >= 'a' && c <= 'z' )             return (unsigned char)( c - 32 );
    if( c >= 0xE0 && c <= 0xFE && c != 0xF7 ) return (unsigned char)( c - 0x20 );
    if( c == 0x9A || ( c >= 0x9C && c <= 0x9F ) ) return (unsigned char)( c - 0x10 );
    if( c == 0xB3 ) return 0xA3;
    if( c == 0xB9 ) return 0xA5;
    if( c == 0xBE ) return 0xBC;
    if( c == 0xBF ) return 0xAF;
    return c;
  }
  inline char InvCapFirst( char c ) {
    return (char)CapFirstChar( (unsigned char)c );
  }
  inline bool NameIsKey( const char* a, const char* b ) {
    if( !a || !b || !a[0] || _stricmp( a, b ) ) return false;
    if( !strcmp( a, b ) ) return true;
    for( const char* p = a; *p; p++ )
      if( *p >= 'a' && *p <= 'z' ) return false;
    return true;
  }
  inline unsigned char HovFold( unsigned char c ) {
    if( c >= 'A' && c <= 'Z' ) return (unsigned char)( c + 32 );
    if( c < 0x80 ) return c;
    switch( c ) {
      case 0xA5: case 0xB9: case 0xC1: case 0xE1: return 'a';
      case 0xC6: case 0xE6: case 0xC8: case 0xE8: return 'c';
      case 0xCF: case 0xEF: return 'd';
      case 0xCA: case 0xEA: case 0xC9: case 0xE9: case 0xCC: case 0xEC: return 'e';
      case 0xCD: case 0xED: return 'i';
      case 0xA3: case 0xB3: return 'l';
      case 0xD1: case 0xF1: case 0xD2: case 0xF2: return 'n';
      case 0xD3: case 0xF3: return 'o';
      case 0xD8: case 0xF8: return 'r';
      case 0x8C: case 0x9C: case 0x8A: case 0x9A: return 's';
      case 0x8D: case 0x9D: return 't';
      case 0xDA: case 0xFA: case 0xD9: case 0xF9: return 'u';
      case 0xDD: case 0xFD: return 'y';
      case 0x8F: case 0x9F: case 0xAF: case 0xBF: case 0x8E: case 0x9E: return 'z';
    }
    return c;
  }
  inline void HovNormalize( const char* src, char* dst, int max ) {
    int n = 0;
    if( src )
      for( ; *src && n < max - 1; src++ )
      {
        const unsigned char c = (unsigned char)*src;
        const char* two = ( c == 0xE4 || c == 0xC4 ) ? "ae" : ( c == 0xF6 || c == 0xD6 ) ? "oe"
                        : ( c == 0xFC || c == 0xDC ) ? "ue" : ( c == 0xDF ) ? "ss" : 0;
        if( two && n < max - 2 ) {
          dst[n++] = two[0];
          dst[n++] = two[1];
        }
        else
          dst[n++] = (char)HovFold( c );
      }
    dst[n] = 0;
  }
  inline bool HovWordChar( unsigned char c ) {
    return ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || ( c >= '0' && c <= '9' ) || c >= 0x80;
  }
  inline bool HovKindInName( const char* name, const char* kind ) {
    if( !name || !kind || !name[0] || !kind[0] ) return false;
    char nm[128], kd[128];
    HovNormalize( name, nm, 128 );
    HovNormalize( kind, kd, 128 );
    const char* p = kd;
    while( *p ) {
      while( *p == ' ' || *p == '/' )
        p++;
      const char* e = p;
      while( *e && *e != '/' )
        e++;
      int len = (int)( e - p );
      while( len > 0 && p[len - 1] == ' ' )
        len--;
      if( len >= 3 ) {
        for( const char* q = nm; *q; q++ ) {
          if( q != nm && HovWordChar( (unsigned char)q[-1] ) ) continue;
          if( strncmp( q, p, (size_t)len ) == 0 && !HovWordChar( (unsigned char)q[len] ) ) return true;
        }
      }
      p = e;
    }
    return false;
  }
  inline unsigned char SearchFold( unsigned char c ) {
    if( c >= 'A' && c <= 'Z' ) return (unsigned char)( c + 32 );
    if( c < 0x80 ) return c;
    switch( c ) {
      case 0xB9: case 0xA5: case 0xE4: case 0xC4: return 'a';
      case 0xE6: case 0xC6: return 'c';
      case 0xEA: case 0xCA: return 'e';
      case 0xB3: case 0xA3: return 'l';
      case 0xF1: case 0xD1: return 'n';
      case 0xF3: case 0xD3: case 0xF6: case 0xD6: return 'o';
      case 0x9C: case 0x8C: case 0xDF: return 's';
      case 0x9F: case 0x8F: case 0xBF: case 0xAF: return 'z';
      case 0xFC: case 0xDC: return 'u';
    }
    return c;
  }
  inline void SearchNormalize( const char* src, char* dst, int max ) {
    int n = 0;
    if( src )
      for( ; *src && n < max - 1; src++ )
        dst[n++] = (char)SearchFold( (unsigned char)*src );
    dst[n] = 0;
  }
  inline bool SearchInChars( const char* h, const char* needle ) {
    if( !h ) return false;
    while( *h == ' ' || ( *h && (unsigned char)*h < 32 ) )
      h++;
    const char* b = needle;
    while( *b && *h && (char)SearchFold( (unsigned char)*h ) == *b ) { h++; b++; }
    return !*b;
  }
}
