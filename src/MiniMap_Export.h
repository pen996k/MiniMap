#pragma once
namespace MiniMap {
  static const int EXP_MAX_SIDE = 32768;
  static const int EXP_ROWS_PER_STEP = 96;
  enum { EXP_IDLE = 0, EXP_RUN = 1 };
  enum { EXP_FMT_BMP = 0, EXP_FMT_PNG = 1, EXP_FMT_JPG = 2 };
  static int            g_expState   = EXP_IDLE;
  static unsigned char* g_expSrc     = 0;
  static int            g_expN       = 0;
  static int            g_expBpp     = 0;
  static int            g_expFmt     = -1;
  static int            g_expOut     = 0;
  static int            g_expOutH    = 0;
  static int            g_expRow     = 0;
  static int            g_expFile_t  = EXP_FMT_BMP;
  static HANDLE         g_expFile    = INVALID_HANDLE_VALUE;
  static unsigned char* g_expLine    = 0;
  static char           g_expName[160];
  static char           g_expShort[96];
  static TMapInfo       g_expRect;
  static DWORD          g_expT0      = 0;
  static DWORD          g_expDoneAt  = 0;
  static int            g_expResult  = 0;
  static bool           g_expEnterWas = true;
  static bool           g_expCancel  = false;
  enum { EXP_ST_ATLAS = 0, EXP_ST_MARK = 1, EXP_ST_DOT = 2 };
  struct TExpStamp { int kind, icon, S; float fx, fy; unsigned char* px; };
  enum { EXP_STAMPS_MAX = 96 };
  static TExpStamp g_expSt[EXP_STAMPS_MAX];
  static int       g_expStN = 0;
  struct TExpMark { int x0, y0; unsigned short st; };
  enum { EXP_MARKS_MAX = 30000 };
  static TExpMark* g_expMarks  = 0;
  static int       g_expMarkN  = 0;
  static int       g_expMarkSMax = 0;
  static TExpMark  g_expHero;
  static bool      g_expHeroOn = false;
  inline void ExpStampsFree() {
    for( int i = 0; i < g_expStN; i++ ) {
      MiniMapCommon::MmFree( g_expSt[i].px );
      g_expSt[i].px = 0;
    }
    g_expStN = 0;
  }
  inline bool ExpHull( int packed, float u, float v ) {
    const int kind = packed >> 16, icon = packed & 0xFFFF;
    if( kind == EXP_ST_MARK ) return MarkHull( icon, u, v );
    if( kind == EXP_ST_DOT ) return u * u + v * v <= 0.55f * 0.55f;
    return IconHull( IconShapeOf( icon ), u, v );
  }
  inline bool ExpHole( int packed, float u, float v ) {
    const int kind = packed >> 16, icon = packed & 0xFFFF;
    if( kind == EXP_ST_MARK ) return MarkHole( icon, u, v );
    return ( kind != EXP_ST_DOT ) && IconHole( IconShapeOf( icon ), u, v );
  }
  inline int ExpStamp( int kind, int icon, int S, float fx = 0.0f, float fy = -1.0f ) {
    if( S < 6 ) S = 6;
    for( int i = 0; i < g_expStN; i++ ) {
      const TExpStamp& t = g_expSt[i];
      if( t.kind == kind && t.icon == icon && t.S == S && t.fx == fx && t.fy == fy ) return i;
    }
    if( g_expStN >= EXP_STAMPS_MAX ) return -1;
    unsigned char* const px = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( S * S * 4 ) );
    if( !px ) return -1;
    memset( px, 0, S * S * 4 );
    const zCOLOR col = ( kind == EXP_ST_MARK ) ? IconColor( icon ) : AtlasColor( icon );
    const float rot[2] = { fx, fy };
    AtlasBakeCell( px, S, 4, (int)zRND_TEX_FORMAT_ARGB_8888, 0, 0, S, ( kind << 16 ) | ( icon & 0xFFFF ), col,
                   kind == EXP_ST_MARK || S >= 16, &ExpHull, &ExpHole, ( fx != 0.0f || fy != -1.0f ) ? rot : 0, ( S >= 32 ) ? 2 : 4 );
    TExpStamp& t = g_expSt[g_expStN];
    t.kind = kind; t.icon = icon; t.S = S; t.fx = fx; t.fy = fy; t.px = px;
    return g_expStN++;
  }
  typedef HRESULT ( WINAPI *TCoInitEx )( LPVOID, DWORD );
  typedef void    ( WINAPI *TCoUninit )();
  typedef HRESULT ( WINAPI *TCoCreate )( REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID* );
  static HMODULE                 g_wicOle    = 0;
  static TCoUninit               g_wicUninit = 0;
  static bool                    g_wicCoInit = false;
  static MiniMapWic::W_IFactory*     g_wicFact   = 0;
  static MiniMapWic::W_IStreamW*             g_wicStream = 0;
  static MiniMapWic::W_IEncoder*      g_wicEnc    = 0;
  static MiniMapWic::W_IFrameEncode*  g_wicFrame  = 0;
  static const GUID kClsidWicFactory = { 0xcacaf262, 0x9370, 0x4615, { 0xa1, 0x3b, 0x9f, 0x55, 0x39, 0xda, 0x4c, 0x0a } };
  static const GUID kWicPng  = { 0x1b7cfaf4, 0x713f, 0x473c, { 0xbb, 0xcd, 0x61, 0x37, 0x42, 0x5f, 0xae, 0xaf } };
  static const GUID kWicJpeg = { 0x19e4a5aa, 0x5662, 0x4fc5, { 0xa0, 0xc0, 0x17, 0x58, 0x02, 0x8e, 0x10, 0x57 } };
  static const GUID kWic24BGR = { 0x6fddc324, 0x4e03, 0x4bfe, { 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x0c } };
  inline void ExpWicClose( bool commit ) {
    if( g_wicFrame ) {
      if( commit ) g_wicFrame->Commit();
      g_wicFrame->Release();
      g_wicFrame = 0;
    }
    if( g_wicEnc ) {
      if( commit ) g_wicEnc->Commit();
      g_wicEnc->Release();
      g_wicEnc = 0;
    }
    if( g_wicStream ) { g_wicStream->Release(); g_wicStream = 0; }
    if( g_wicFact )   { g_wicFact->Release();   g_wicFact   = 0; }
    if( g_wicCoInit && g_wicUninit )
      g_wicUninit();
    g_wicCoInit = false;
  }
  inline bool ExpWicFail( const char* why, HRESULT hr ) {
    char hx[16];
    snprintf( hx, sizeof( hx ), "0x%08X", (unsigned)hr );
    MM_LOG( zSTRING( "export: image codec - " ) + zSTRING( why ) + " (" + zSTRING( hx ) + ")" );
    ExpWicClose( false );
    return false;
  }
  inline bool ExpWicOpen( const char* file, bool jpg, int quality, int w, int h ) {
    if( !g_wicOle ) g_wicOle = LoadLibraryA( "ole32.dll" );
    if( !g_wicOle ) {
      MM_LOG( "export: ole32.dll not available" );
      return false;
    }
    TCoInitEx coInit  = (TCoInitEx)GetProcAddress( g_wicOle, "CoInitializeEx" );
    TCoCreate coCreate = (TCoCreate)GetProcAddress( g_wicOle, "CoCreateInstance" );
    g_wicUninit = (TCoUninit)GetProcAddress( g_wicOle, "CoUninitialize" );
    if( !coInit || !coCreate || !g_wicUninit ) return ExpWicFail( "ole32 functions missing", E_NOTIMPL );
    const HRESULT hi = coInit( 0, COINIT_APARTMENTTHREADED );
    g_wicCoInit = SUCCEEDED( hi );
    HRESULT hr = coCreate( kClsidWicFactory, 0, CLSCTX_INPROC_SERVER,
                           __uuidof( MiniMapWic::W_IFactory ), (LPVOID*)&g_wicFact );
    if( FAILED( hr ) || !g_wicFact ) return ExpWicFail( "no WIC factory", hr );
    hr = g_wicFact->CreateStream( &g_wicStream );
    if( FAILED( hr ) ) return ExpWicFail( "CreateStream", hr );
    wchar_t wfile[200];
    if( !MultiByteToWideChar( CP_ACP, 0, file, -1, wfile, 200 ) ) return ExpWicFail( "file name", E_INVALIDARG );
    hr = g_wicStream->InitializeFromFilename( wfile, GENERIC_WRITE );
    if( FAILED( hr ) ) return ExpWicFail( "cannot create file", hr );
    hr = g_wicFact->CreateEncoder( jpg ? kWicJpeg : kWicPng, 0, &g_wicEnc );
    if( FAILED( hr ) ) return ExpWicFail( "CreateEncoder", hr );
    hr = g_wicEnc->Initialize( g_wicStream, MiniMapWic::WICBitmapEncoderNoCache );
    if( FAILED( hr ) ) return ExpWicFail( "encoder Initialize", hr );
    IPropertyBag2* props = 0;
    hr = g_wicEnc->CreateNewFrame( &g_wicFrame, &props );
    if( FAILED( hr ) ) return ExpWicFail( "CreateNewFrame", hr );
    if( jpg && props ) {
      PROPBAG2 opt;
      memset( &opt, 0, sizeof( opt ) );
      opt.pstrName = (LPOLESTR)L"ImageQuality";
      VARIANT val;
      memset( &val, 0, sizeof( val ) );
      val.vt     = VT_R4;
      val.fltVal = (float)quality / 100.0f;
      props->Write( 1, &opt, &val );
    }
    hr = g_wicFrame->Initialize( props );
    if( props )
      props->Release();
    if( FAILED( hr ) ) return ExpWicFail( "frame Initialize", hr );
    hr = g_wicFrame->SetSize( (UINT)w, (UINT)h );
    if( FAILED( hr ) ) return ExpWicFail( "SetSize", hr );
    MiniMapWic::WICPixelFormatGUID pf = kWic24BGR;
    hr = g_wicFrame->SetPixelFormat( &pf );
    if( FAILED( hr ) || memcmp( &pf, &kWic24BGR, sizeof( GUID ) ) != 0 ) return ExpWicFail( "24 bit BGR not supported", hr );
    return true;
  }
  inline bool ExpWicRow( unsigned char* line, int rowBytes ) {
    if( !g_wicFrame ) return false;
    const HRESULT hr = g_wicFrame->WritePixels( 1, (UINT)rowBytes, (UINT)rowBytes, (BYTE*)line );
    if( FAILED( hr ) ) {
      ExpWicFail( "WritePixels", hr );
      return false;
    }
    return true;
  }
  inline void ExportFree() {
    MiniMapCommon::MmFree( g_expSrc );   g_expSrc   = 0;
    MiniMapCommon::MmFree( g_expLine );  g_expLine  = 0;
    MiniMapCommon::MmFree( g_expMarks ); g_expMarks = 0;
    g_expMarkN = 0;
    g_expHeroOn = false;
    ExpStampsFree();
    if( g_expFile != INVALID_HANDLE_VALUE ) {
      CloseHandle( g_expFile );
      g_expFile = INVALID_HANDLE_VALUE;
    }
    ExpWicClose( false );
  }
  inline void ExpSrcRGB( int ix, int iz, int& r, int& g, int& b ) {
    if( ix < 0 ) ix = 0;
    if( iz < 0 ) iz = 0;
    if( ix >= g_expN ) ix = g_expN - 1;
    if( iz >= g_expN ) iz = g_expN - 1;
    const int q = iz * g_expN + ix;
    int a = 255;
    if( g_expBpp == 4 ) {
      const unsigned long c = ( (const unsigned long*)g_expSrc )[q];
      a = (int)( ( c >> 24 ) & 0xFF );
      r = (int)( ( c >> 16 ) & 0xFF );
      g = (int)( ( c >>  8 ) & 0xFF );
      b = (int)(   c         & 0xFF );
    }
    else {
      const unsigned short c = ( (const unsigned short*)g_expSrc )[q];
      if( g_expFmt == (int)zRND_TEX_FORMAT_ARGB_1555 ) {
        a = ( c & 0x8000 ) ? 255 : 0;
        r = ( ( c >> 10 ) & 0x1F ) << 3;
        g = ( ( c >>  5 ) & 0x1F ) << 3;
        b = (   c         & 0x1F ) << 3;
      }
      else if( g_expFmt == (int)zRND_TEX_FORMAT_RGB_565 ) {
        r = ( ( c >> 11 ) & 0x1F ) << 3;
        g = ( ( c >>  5 ) & 0x3F ) << 2;
        b = (   c         & 0x1F ) << 3;
      }
      else {
        a = ( ( c >> 12 ) & 0x0F ) << 4;
        r = ( ( c >>  8 ) & 0x0F ) << 4;
        g = ( ( c >>  4 ) & 0x0F ) << 4;
        b = (   c         & 0x0F ) << 4;
      }
    }
    if( a < 128 ) {
      const int v = g_backLevel;
      r = v; g = v; b = v;
    }
  }
  inline bool ExpWorldToOutF( float wx, float wz, float& ox, float& oy ) {
    const TMapInfo& m = g_expRect;
    const float u = ( wx - m.x1 ) / ( m.x2 - m.x1 );
    const float v = ( wz - m.y1 ) / ( m.y2 - m.y1 );
    ox = u * (float)g_expOut;
    oy = v * (float)g_expOutH;
    return u >= 0.0f && u <= 1.0f && v >= 0.0f && v <= 1.0f;
  }
  inline void ExpAddMark( float wx, float wz, int st, bool hero = false ) {
    if( st < 0 || !g_expMarks ) return;
    float ox, oy;
    if( !ExpWorldToOutF( wx, wz, ox, oy ) ) return;
    const int S = g_expSt[st].S;
    TExpMark k;
    k.x0 = (int)( ox + 0.5f ) - S / 2;
    k.y0 = (int)( oy + 0.5f ) - S / 2;
    k.st = (unsigned short)st;
    if( S > g_expMarkSMax ) g_expMarkSMax = S;
    if( hero ) {
      g_expHero   = k;
      g_expHeroOn = true;
      return;
    }
    if( g_expMarkN < EXP_MARKS_MAX )
      g_expMarks[g_expMarkN++] = k;
  }
  inline int ExpIconSide() {
    const int side = ( g_expOut > g_expOutH ) ? g_expOut : g_expOutH;
    int s = side / 450;
    if( s < 14 ) s = 14; else if( s > 64 ) s = 64;
    return s;
  }
  inline int ExpDetailCount() {
    return g_expChests + g_expItems + g_expPlants + g_expNpcs + g_expFoes + g_expFriends + g_expSwitches
         + g_expPlaces + g_expExits + g_expTeleports + g_expMarksOn + g_expPlayer;
  }
  inline bool ExpCollectCore( oCWorld* world ) {
    __try {
      const int S  = ExpIconSide();
      const int Si = S * 4 / 5;
      if( g_expChests ) {
        ChestCacheRefresh( world );
        for( int i = 0; i < g_chestCount; i++ ) {
          zCVob* const v = g_chestCache ? g_chestCache[i] : 0;
          if( !v || v->homeWorld != (zCWorld*)world ) continue;
          const zVEC3 p = v->GetPositionWorld();
          ExpAddMark( p[0], p[2], ExpStamp( EXP_ST_ATLAS, ChestKind( (oCMobContainer*)v ), S ) );
        }
      }
      if( g_expItems || g_expPlants ) {
        ItemCacheRefresh( world );
        for( int i = 0; i < g_itemCount; i++ ) {
          oCItem* const it = g_itemCache ? g_itemCache[i] : 0;
          if( !it || it->homeWorld != (zCWorld*)world ) continue;
          const int ic = ItemKindIcon( it );
          if( ic == ICON_ITEM_PLANT ? !g_expPlants : !g_expItems ) continue;
          const zVEC3 p = it->GetPositionWorld();
          ExpAddMark( p[0], p[2], ExpStamp( EXP_ST_ATLAS, g_itemColors ? ic : ICON_ITEM_OTHER, Si ) );
        }
      }
      if( g_expNpcs || g_expFoes || g_expFriends ) {
        NpcCacheRefresh( world );
        for( int i = 0; i < g_npcCount; i++ ) {
          oCNpc* const npc = g_npcCache ? g_npcCache[i] : 0;
          if( !npc || npc == player ) continue;
          zCWorld* const home = npc->homeWorld;
          if( home && home != (zCWorld*)world ) continue;
          if( npc->attribute[NPC_ATR_HITPOINTS] <= 0 || NpcFlagAt( i, NPCF_GONE ) ) continue;
          const int grp = NpcGroup( npc );
          if( grp == 0 ? !g_expFoes : grp == 2 ? !g_expFriends : !g_expNpcs ) continue;
          zVEC3 p = npc->GetPositionWorld();
          if( !home ) {
            if( const NpcSpawnPos* sp = NpcSpawnFind( npc ) ) {
              p[0] = sp->x;
              p[2] = sp->z;
            }
            else if( !NpcWpAt( i, p[0], p[2] ) )
              continue;
          }
          int ic = ( grp != 0 && NpcFlagAt( i, NPCF_TRADER ) ) ? ICON_TRADER : grp;
          if( ic == 1 && !npc->IsHuman() ) ic = ICON_ANIMAL;
          ExpAddMark( p[0], p[2], ExpStamp( EXP_ST_ATLAS, ic, S ) );
        }
      }
      if( g_expSwitches ) {
        SwitchCacheRefresh( world );
        for( int i = 0; i < g_switchCount; i++ ) {
          zCVob* const v = g_switchCache ? g_switchCache[i] : 0;
          if( !v || v->homeWorld != (zCWorld*)world ) continue;
          const int k = SwitchKindOf( v );
          if( k < 0 ) continue;
          const zVEC3 p = v->GetPositionWorld();
          ExpAddMark( p[0], p[2], ExpStamp( EXP_ST_ATLAS, k, S ) );
        }
      }
      if( g_expPlaces ) {
        PlaceCacheRefresh( world );
        for( int i = 0; i < g_placeCount; i++ ) {
          zCVob* const v = g_placeCache ? g_placeCache[i] : 0;
          if( !v || v->homeWorld != (zCWorld*)world ) continue;
          const int k = PlaceKindOf( v );
          if( k < 0 ) continue;
          if( !PlaceKindEnabled( k ) ) continue;
          const zVEC3 p = v->GetPositionWorld();
          const bool dot = ( k >= ICON_COUNT );
          const int  s   = PlaceIsRest( k ) ? S * 2 / 3 : S;
          ExpAddMark( p[0], p[2], ExpStamp( dot ? EXP_ST_DOT : EXP_ST_ATLAS, k, dot ? s * 2 / 3 : s ) );
        }
      }
      if( g_expExits ) {
        ExitEnsure( world );
        for( int i = 0; i < g_exitCount; i++ )
          ExpAddMark( g_exits[i].x, g_exits[i].z, ExpStamp( EXP_ST_ATLAS, ICON_EXIT, S * 6 / 5 ) );
      }
      if( g_expTeleports )
        for( int i = 0; i < g_tpCount; i++ )
          ExpAddMark( g_tp[i].x, g_tp[i].z, ExpStamp( EXP_ST_ATLAS, ICON_TELEPORT, S * 6 / 5 ) );
      if( g_expTeleports && g_showTeleports ) {
        WtpEnsure( world );
        for( int i = 0; i < g_wtpCount; i++ )
          ExpAddMark( g_wtp[i].x, g_wtp[i].z, ExpStamp( EXP_ST_ATLAS, ICON_TELEPORT, S ) );
      }
      if( g_expMarksOn )
        for( int i = 0; i < g_ownCount; i++ )
          if( MarkHere( g_ownMarks[i] ) )
            ExpAddMark( g_ownMarks[i].x, g_ownMarks[i].z, ExpStamp( EXP_ST_MARK, g_ownMarks[i].icon, S * 13 / 10 ) );
      if( g_expPlayer && player ) {
        const zVEC3 p  = player->GetPositionWorld();
        const zVEC3 at = player->GetAtVectorWorld();
        float ax, ay, bx, by;
        ExpWorldToOutF( p[0], p[2], ax, ay );
        ExpWorldToOutF( p[0] + at[0] * 1000.0f, p[2] + at[2] * 1000.0f, bx, by );
        float fx = bx - ax, fy = by - ay;
        const float l = sqrtf( fx * fx + fy * fy );
        if( l > 0.001f ) { fx /= l; fy /= l; }
        else             { fx = 0.0f; fy = -1.0f; }
        fx = floorf( fx * 64.0f + 0.5f ) / 64.0f;
        fy = floorf( fy * 64.0f + 0.5f ) / 64.0f;
        ExpAddMark( p[0], p[2], ExpStamp( EXP_ST_ATLAS, ICON_PLAYER, S * 3 / 2, fx, fy ), true );
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline int ExpMarkCmp( const void* a, const void* b ) {
    return ( (const TExpMark*)a )->y0 - ( (const TExpMark*)b )->y0;
  }
  inline void ExpPaintOne( const TExpMark& k, int y, unsigned char* line ) {
    const TExpStamp& t = g_expSt[k.st];
    const int ty = y - k.y0;
    if( ty < 0 || ty >= t.S ) return;
    const unsigned char* sp = t.px + ty * t.S * 4;
    for( int tx = 0; tx < t.S; tx++, sp += 4 ) {
      const int a = sp[3];
      if( !a ) continue;
      const int x = k.x0 + tx;
      if( x < 0 || x >= g_expOut ) continue;
      unsigned char* const p = line + x * 3;
      p[0] = (unsigned char)( ( sp[0] * a + p[0] * ( 255 - a ) + 127 ) / 255 );
      p[1] = (unsigned char)( ( sp[1] * a + p[1] * ( 255 - a ) + 127 ) / 255 );
      p[2] = (unsigned char)( ( sp[2] * a + p[2] * ( 255 - a ) + 127 ) / 255 );
    }
  }
  inline void ExpPaintMarks( int y, unsigned char* line ) {
    int lo = 0, hi = g_expMarkN;
    const int from = y - g_expMarkSMax;
    while( lo < hi ) {
      const int mid = ( lo + hi ) / 2;
      if( g_expMarks[mid].y0 <= from ) lo = mid + 1;
      else                             hi = mid;
    }
    for( int i = lo; i < g_expMarkN && g_expMarks[i].y0 <= y; i++ )
      ExpPaintOne( g_expMarks[i], y, line );
    if( g_expHeroOn )
      ExpPaintOne( g_expHero, y, line );
  }
  inline bool ExpLoadSource( const TMapInfo& m ) {
    char name[NAME_MAX], file[NAME_MAX + 24];
    GenCacheName( g_world, name, NAME_MAX );
    FilesEnsure();
    FilePath( file, NAME_MAX + 24, FILES_GEN_DIR, name );
    HANDLE h = CreateFileA( file, GENERIC_READ, FILE_SHARE_READ, 0,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) {
      MM_LOG( "export: no world map cache file (map not ready yet)" );
      return false;
    }
    int head[6] = { 0, 0, 0, 0, 0, 0 };
    DWORD rd = 0;
    const bool ok = ReadFile( h, head, sizeof( head ), &rd, 0 ) && rd == sizeof( head )
                 && head[0] == GEN_CACHE_MAGIC && head[1] >= 256 && head[1] <= 16384
                 && ( head[3] == 2 || head[3] == 4 ) && head[4] == GenSignature( m );
    if( !ok ) {
      CloseHandle( h );
      MM_LOG( "export: world map cache out of date - open the map once first" );
      return false;
    }
    const int n = head[1], bpp = head[3];
    unsigned char* px = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( (size_t)n * n * bpp ) );
    if( !px ) {
      CloseHandle( h );
      MM_LOG( "export: out of memory for world map" );
      return false;
    }
    const bool rdOk = PkReadAny( h, px, (unsigned)( n * n * bpp ), 0 );
    CloseHandle( h );
    if( !rdOk ) {
      MiniMapCommon::MmFree( px );
      MM_LOG( "export: cannot read world map cache" );
      return false;
    }
    g_expSrc = px;
    g_expN   = n;
    g_expBpp = bpp;
    g_expFmt = head[2];
    return true;
  }
  inline void ExpPut16( unsigned char* p, unsigned v ) { p[0] = (unsigned char)v; p[1] = (unsigned char)( v >> 8 ); }
  inline void ExpPut32( unsigned char* p, unsigned v ) {
    p[0] = (unsigned char)v; p[1] = (unsigned char)( v >> 8 );
    p[2] = (unsigned char)( v >> 16 ); p[3] = (unsigned char)( v >> 24 );
  }
  inline int ExpQualityPct() {
    static const int k[4] = { 70, 80, 90, 95 };
    const int i = ( g_exportQuality < 0 ) ? 0 : ( g_exportQuality > 3 ) ? 3 : g_exportQuality;
    return k[i];
  }
  inline void ExportStart() {
    if( g_expState != EXP_IDLE || g_genRunning ) return;
    g_expResult = 0;
    if( !ogame || !player || g_mapIdx < 0 || !g_maps[g_mapIdx].hasCoords || !g_world[0] ) {
      g_expResult = -1;
      g_expDoneAt = GetTickCount();
      MM_LOG( "export: no world map in this world" );
      return;
    }
    const TMapInfo& m = g_maps[g_mapIdx];
    if( !ExpLoadSource( m ) ) {
      g_expResult = -1;
      g_expDoneAt = GetTickCount();
      return;
    }
    int zoom = ( g_exportZoom < 0 ) ? 0 : ( g_exportZoom > 2 ) ? 2 : g_exportZoom;
    float ww = m.x2 - m.x1, wd = m.y2 - m.y1;
    if( ww < 0.0f ) ww = -ww;
    if( wd < 0.0f ) wd = -wd;
    const double base = (double)( g_expN << zoom );
    double fw = base, fh = base;
    if( ww > 1.0f && wd > 1.0f ) {
      if( ww >= wd ) fw = base * (double)ww / (double)wd;
      else           fh = base * (double)wd / (double)ww;
    }
    const double big = ( fw > fh ) ? fw : fh;
    if( big > (double)EXP_MAX_SIDE ) {
      const double s = (double)EXP_MAX_SIDE / big;
      fw *= s;
      fh *= s;
    }
    int outW = (int)( fw + 0.5 ), outH = (int)( fh + 0.5 );
    if( outW < 16 ) outW = 16;
    if( outH < 16 ) outH = 16;
    if( outW > EXP_MAX_SIDE ) outW = EXP_MAX_SIDE;
    if( outH > EXP_MAX_SIDE ) outH = EXP_MAX_SIDE;
    g_expOut  = outW;
    g_expOutH = outH;
    g_expRect = m;
    g_expRow  = 0;
    const int rowBytes = ( outW * 3 + 3 ) & ~3;
    g_expLine = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( rowBytes ) );
    if( !g_expLine ) {
      ExportFree();
      g_expResult = -1;
      g_expDoneAt = GetTickCount();
      return;
    }
    memset( g_expLine, 0, rowBytes );
    g_expMarkN    = 0;
    g_expMarkSMax = 0;
    g_expHeroOn   = false;
    const int dets = ExpDetailCount();
    if( dets > 0 ) {
      g_expMarks = MiniMapCommon::MmAlloc<TExpMark>( (size_t)( EXP_MARKS_MAX ) );
      oCWorld* const world = ogame->GetGameWorld();
      if( g_expMarks && world && !ExpCollectCore( world ) ) {
        g_expMarkN  = 0;
        g_expHeroOn = false;
      }
      if( g_expMarkN > 1 )
        qsort( g_expMarks, g_expMarkN, sizeof( TExpMark ), ExpMarkCmp );
    }
    SYSTEMTIME st;
    GetLocalTime( &st );
    g_expFile_t = ( g_exportFormat < 0 || g_exportFormat > 2 ) ? EXP_FMT_JPG : g_exportFormat;
    const char* const ext = ( g_expFile_t == EXP_FMT_PNG ) ? ".png"
                          : ( g_expFile_t == EXP_FMT_JPG ) ? ".jpg" : ".bmp";
    snprintf( g_expShort, sizeof( g_expShort ), "%s_%04u%02u%02u_%02u%02u%02u_%dx%d%s%s",
              g_world, (unsigned)st.wYear, (unsigned)st.wMonth, (unsigned)st.wDay,
              (unsigned)st.wHour, (unsigned)st.wMinute, (unsigned)st.wSecond,
              outW, outH, dets > 0 ? "_detale" : "", ext );
    snprintf( g_expName, sizeof( g_expName ), "%s\\export\\%s", FILES_DIR, g_expShort );
    FilesEnsure();
    CreateDirectoryA( "MiniMap\\export", 0 );
    if( g_expFile_t != EXP_FMT_BMP ) {
      if( !ExpWicOpen( g_expName, g_expFile_t == EXP_FMT_JPG, ExpQualityPct(), outW, outH ) ) {
        DeleteFileA( g_expName );
        ExportFree();
        g_expResult = -1;
        g_expDoneAt = GetTickCount();
        return;
      }
    }
    else {
      g_expFile = CreateFileA( g_expName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
      if( g_expFile == INVALID_HANDLE_VALUE ) {
        MM_LOG( zSTRING( "export: cannot create file " ) + zSTRING( g_expName ) );
        ExportFree();
        g_expResult = -1;
        g_expDoneAt = GetTickCount();
        return;
      }
      unsigned char hd[54];
      memset( hd, 0, sizeof( hd ) );
      const unsigned __int64 img = (unsigned __int64)rowBytes * (unsigned __int64)outH;
      hd[0] = 'B'; hd[1] = 'M';
      ExpPut32( hd + 2, ( img + 54 > 0xFFFFFFFFull ) ? 0u : (unsigned)( img + 54 ) );
      ExpPut32( hd + 10, 54 );
      ExpPut32( hd + 14, 40 );
      ExpPut32( hd + 18, (unsigned)outW );
      ExpPut32( hd + 22, (unsigned)( -outH ) );
      ExpPut16( hd + 26, 1 );
      ExpPut16( hd + 28, 24 );
      ExpPut32( hd + 34, ( img > 0xFFFFFFFFull ) ? 0u : (unsigned)img );
      ExpPut32( hd + 38, 2835 );
      ExpPut32( hd + 42, 2835 );
      DWORD w = 0;
      WriteFile( g_expFile, hd, sizeof( hd ), &w, 0 );
    }
    g_expState  = EXP_RUN;
    g_expCancel = false;
    g_expT0     = GetTickCount();
    MM_LOG( zSTRING( "export: start " ) + zSTRING( g_expShort ) + ", source " + zSTRING( g_expN ) + ", details " + zSTRING( dets ) + ", marks " + zSTRING( g_expMarkN + ( g_expHeroOn ? 1 : 0 ) ) + ", icons " + zSTRING( g_expStN ) + ( g_expFile_t == EXP_FMT_JPG ? zSTRING( ", jpg quality " ) + zSTRING( ExpQualityPct() ) : zSTRING( "" ) ) );
  }
  inline void ExportFinish( bool ok, const char* why ) {
    if( ok && g_expFile_t != EXP_FMT_BMP ) {
      HRESULT hr = g_wicFrame ? g_wicFrame->Commit() : E_FAIL;
      if( SUCCEEDED( hr ) ) hr = g_wicEnc ? g_wicEnc->Commit() : E_FAIL;
      if( g_wicFrame ) { g_wicFrame->Release(); g_wicFrame = 0; }
      if( g_wicEnc )   { g_wicEnc->Release();   g_wicEnc   = 0; }
      if( FAILED( hr ) ) {
        ok  = false;
        why = "image codec could not finish the file (disk full?)";
      }
    }
    ExportFree();
    g_expState  = EXP_IDLE;
    g_expResult = ok ? 1 : -1;
    g_expDoneAt = GetTickCount();
    if( !ok )
      DeleteFileA( g_expName );
    MM_LOG( zSTRING( ok ? "export: saved " : "export: stopped - " ) + zSTRING( ok ? g_expName : why ) + " (" + zSTRING( (int)( GetTickCount() - g_expT0 ) ) + " ms)" );
  }
  inline void ExportStep() {
    if( g_expState != EXP_RUN ) return;
    if( g_expCancel ) {
      g_expCancel = false;
      ExportFinish( false, "cancelled (Esc)" );
      g_expResult = 0;
      return;
    }
    const int out  = g_expOut;
    const int outH = g_expOutH;
    const int rowBytes = ( out * 3 + 3 ) & ~3;
    const float k  = (float)g_expN / (float)out;
    const float kz = (float)g_expN / (float)outH;
    const bool  up = ( k < 0.999f || kz < 0.999f );
    const DWORD t0 = GetTickCount();
    for( int s = 0; s < EXP_ROWS_PER_STEP && g_expRow < outH; s++ ) {
      const int y = g_expRow;
      const float sz = ( (float)y + 0.5f ) * kz - 0.5f;
      int z0 = (int)floorf( sz );
      const float fz = sz - (float)z0;
      for( int x = 0; x < out; x++ ) {
        const float sx = ( (float)x + 0.5f ) * k - 0.5f;
        const int x0 = (int)floorf( sx );
        const float fx = sx - (float)x0;
        int r, g, b;
        if( !up ) {
          ExpSrcRGB( (int)( ( (float)x + 0.5f ) * k ), (int)( ( (float)y + 0.5f ) * kz ), r, g, b );
        }
        else {
          int r00, g00, b00, r10, g10, b10, r01, g01, b01, r11, g11, b11;
          ExpSrcRGB( x0,     z0,     r00, g00, b00 );
          ExpSrcRGB( x0 + 1, z0,     r10, g10, b10 );
          ExpSrcRGB( x0,     z0 + 1, r01, g01, b01 );
          ExpSrcRGB( x0 + 1, z0 + 1, r11, g11, b11 );
          const float a0 = ( 1.0f - fx ) * ( 1.0f - fz ), a1 = fx * ( 1.0f - fz );
          const float a2 = ( 1.0f - fx ) * fz,          a3 = fx * fz;
          r = (int)( r00 * a0 + r10 * a1 + r01 * a2 + r11 * a3 + 0.5f );
          g = (int)( g00 * a0 + g10 * a1 + g01 * a2 + g11 * a3 + 0.5f );
          b = (int)( b00 * a0 + b10 * a1 + b01 * a2 + b11 * a3 + 0.5f );
        }
        unsigned char* p = g_expLine + x * 3;
        p[0] = (unsigned char)b; p[1] = (unsigned char)g; p[2] = (unsigned char)r;
      }
      if( g_expMarkN > 0 || g_expHeroOn )
        ExpPaintMarks( y, g_expLine );
      if( g_expFile_t != EXP_FMT_BMP ) {
        if( !ExpWicRow( g_expLine, rowBytes ) ) {
          ExportFinish( false, "image codec write error (disk full?)" );
          return;
        }
      }
      else {
        DWORD w = 0;
        if( !WriteFile( g_expFile, g_expLine, rowBytes, &w, 0 ) || (int)w != rowBytes ) {
          ExportFinish( false, "write error (disk full?)" );
          return;
        }
      }
      g_expRow++;
      if( GetTickCount() - t0 > 40 ) break;
    }
    if( g_expRow >= outH )
      ExportFinish( true, 0 );
  }
  inline bool ExportRunning() { return g_expState == EXP_RUN; }
  inline void DrawExportProgress() {
    if( !screen ) return;
    const bool run  = g_expState == EXP_RUN;
    const bool done = !run && g_expResult != 0 && GetTickCount() - g_expDoneAt < 6000;
    if( !run && !done ) return;
    char line[200];
    float f = 0.0f;
    if( run ) {
      f = ( g_expOutH > 0 ) ? (float)g_expRow / (float)g_expOutH : 0.0f;
      snprintf( line, sizeof( line ), "%s%d%%", T( TX_EXP_PROG ), (int)( f * 100.0f ) );
    } else if( g_expResult > 0 )
      snprintf( line, sizeof( line ), "%sMiniMap\\export\\%s", T( TX_EXP_SAVED ), g_expShort );
    else
      StrCopy( line, T( TX_EXP_FAIL ), sizeof( line ) );
    DrawTopProgressBanner( line, run ? T( TX_EXP_ESC ) : 0, run, f );
  }
  static bool  g_expPending  = false;
  static DWORD g_expMenuSeen = 0;
  static bool  g_expRowText  = false;
  inline void ExportMenu() {
    zCMenu* menu = zCMenu::GetActive();
    if( !menu || !zinput ) return;
    zCMenuItem* go = zCMenuItem::GetByName( zSTRING( "MM_EXP_GO" ) );
    if( !go ) return;
    const DWORD now = GetTickCount();
    if( g_expMenuSeen == 0 || now - g_expMenuSeen > 300 ) g_expEnterWas = true;
    g_expMenuSeen = now;
    zCMenuItem* act = menu->GetActiveItem();
    const bool enterDown = zinput->KeyPressed( KEY_RETURN ) != 0;
    const bool edge = enterDown && !g_expEnterWas;
    g_expEnterWas = enterDown;
    if( act == go && edge && !g_expPending && g_expState == EXP_IDLE ) {
      g_expPending = true;
      MM_LOG( "export: confirmed in menu - starts after leaving the menu" );
    }
    if( g_expPending ) {
      char row[MENU_PL_MAX];
      StrCopy( row, T( TX_EXP_AFTER_MENU ), MENU_PL_MAX );
      if( g_menuFontPL == 0 )
        MenuFoldPL( row );
      const zSTRING want( row );
      if( !( go->m_parText[0] == want ) )
        go->SetText( want, 0, 1 );
      g_expRowText = true;
    }
    else if( g_expRowText ) {
      g_expRowText      = false;
      g_menuLangApplied = -1;
    }
  }
}
