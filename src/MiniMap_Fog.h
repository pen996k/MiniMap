#pragma once
namespace MiniMap {
  enum { FOG_N = 256, FOG_BYTES = FOG_N * FOG_N / 8 };
  static const DWORD FOG_TICK_MS = 250;
  static const DWORD FOG_SAVE_MS = 30000;
  static const int   FOG_MAGIC   = 0x31474F46;
  static unsigned char g_fogBits[FOG_BYTES];
  static bool          g_fogLoaded  = false;
  static bool          g_fogDirty   = false;
  static bool          g_fogTexDirty = true;
  static char          g_fogKey[96] = "";
  static DWORD         g_fogTick    = 0;
  static DWORD         g_fogSaved   = 0;
  static zCTexture*    g_fogTex     = 0;
  static int           g_fogFmt     = -1;
  static int           g_fogBpp     = 0;
  static unsigned char* g_fogPx     = 0;
  static TMapInfo      g_fogRect;
  inline bool FogBit( int x, int z ) {
    return ( g_fogBits[( z * FOG_N + x ) >> 3] >> ( x & 7 ) & 1 ) != 0;
  }
  inline bool FogFile( char* out, int max ) {
    if( !out || max <= 0 || !g_world[0] || g_mapIdx < 0 ) return false;
    const int n = snprintf( out, (size_t)max, "%s\\fog\\%s_%08X.fog", FILES_DIR, g_world, g_maps[g_mapIdx].tag );
    return n > 0 && n < max;
  }
  inline void FogSave() {
    if( !g_fogLoaded || !g_fogDirty || !g_fogKey[0] ) return;
    char f[160];
    if( !FogFile( f, sizeof( f ) ) || strcmp( f, g_fogKey ) != 0 ) return;
    FilesEnsure();
    CreateDirectoryA( "MiniMap\\fog", 0 );
    HANDLE h = CreateFileA( f, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) {
      FilesDirs();
      CreateDirectoryA( "MiniMap\\fog", 0 );
      h = CreateFileA( f, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
    }
    if( h == INVALID_HANDLE_VALUE ) return;
    int head[2] = { FOG_MAGIC, FOG_N };
    DWORD w = 0;
    WriteFile( h, head, sizeof( head ), &w, 0 );
    WriteFile( h, g_fogBits, FOG_BYTES, &w, 0 );
    CloseHandle( h );
    g_fogDirty = false;
    g_fogSaved = GetTickCount();
  }
  inline void FogLoad() {
    char f[160];
    if( !FogFile( f, sizeof( f ) ) ) return;
    if( g_fogLoaded && strcmp( f, g_fogKey ) == 0 ) return;
    FogSave();
    memset( g_fogBits, 0, FOG_BYTES );
    StrCopy( g_fogKey, f, sizeof( g_fogKey ) );
    g_fogLoaded   = true;
    g_fogDirty    = false;
    g_fogTexDirty = true;
    g_fogRect     = g_maps[g_mapIdx];
    HANDLE h = CreateFileA( f, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) {
      MM_LOG( "fog: new for this world" );
      return;
    }
    int head[2] = { 0, 0 };
    DWORD r = 0;
    if( ReadFile( h, head, sizeof( head ), &r, 0 ) && r == sizeof( head )
        && head[0] == FOG_MAGIC && head[1] == FOG_N ) {
      if( !ReadFile( h, g_fogBits, FOG_BYTES, &r, 0 ) || r != FOG_BYTES )
        memset( g_fogBits, 0, FOG_BYTES );
    }
    CloseHandle( h );
    int open = 0;
    for( int i = 0; i < FOG_BYTES; i++ )
      for( int b = 0; b < 8; b++ )
        open += ( g_fogBits[i] >> b ) & 1;
    MM_LOG( zSTRING( "fog: loaded, discovered " ) + zSTRING( open * 100 / ( FOG_N * FOG_N ) ) + "%" );
  }
  inline void FogDetach() {
    FogSave();
    g_fogLoaded = false;
    g_fogKey[0] = 0;
    if( g_fogTex ) {
      g_fogTex->Release();
      g_fogTex = 0;
    }
    MiniMapCommon::MmFree( g_fogPx );
    g_fogPx = 0;
  }
  inline void FogUpdate() {
    if( !g_fog || !player || g_mapIdx < 0 || !g_maps[g_mapIdx].hasCoords ) return;
    const DWORD now = GetTickCount();
    if( now - g_fogTick < FOG_TICK_MS ) return;
    g_fogTick = now;
    FogLoad();
    if( !g_fogLoaded ) return;
    const TMapInfo& m = g_fogRect;
    const float w = m.x2 - m.x1, d = m.y2 - m.y1;
    if( w > -1.0f && w < 1.0f ) return;
    if( d > -1.0f && d < 1.0f ) return;
    const zVEC3 pp = player->GetPositionWorld();
    const float fx = ( pp[0] - m.x1 ) / w * (float)FOG_N;
    const float fz = ( pp[2] - m.y1 ) / d * (float)FOG_N;
    float cellX = ( w < 0 ? -w : w ) / (float)FOG_N;
    float cellZ = ( d < 0 ? -d : d ) / (float)FOG_N;
    if( cellX < 1.0f ) cellX = 1.0f;
    if( cellZ < 1.0f ) cellZ = 1.0f;
    const float rad = (float)( g_fogRadius > 0 ? g_fogRadius : 1 );
    int rx = (int)( rad / cellX + 0.5f ), rz = (int)( rad / cellZ + 0.5f );
    if( rx < 1 )   rx = 1;
    if( rz < 1 )   rz = 1;
    if( rx > 128 ) rx = 128;
    if( rz > 128 ) rz = 128;
    const float radSq = rad * rad;
    const int cx = (int)fx, cz = (int)fz;
    bool changed = false;
    for( int z = cz - rz; z <= cz + rz; z++ ) {
      if( z < 0 || z >= FOG_N ) continue;
      for( int x = cx - rx; x <= cx + rx; x++ ) {
        if( x < 0 || x >= FOG_N ) continue;
        const float ddx = (float)( x - cx ) * cellX, ddz = (float)( z - cz ) * cellZ;
        if( ( x != cx || z != cz ) && ddx * ddx + ddz * ddz > radSq ) continue;
        unsigned char& b = g_fogBits[( z * FOG_N + x ) >> 3];
        const unsigned char bit = (unsigned char)( 1 << ( x & 7 ) );
        if( !( b & bit ) ) {
          b |= bit;
          changed = true;
        }
      }
    }
    if( changed ) {
      g_fogDirty    = true;
      g_fogTexDirty = true;
    }
    if( g_fogDirty && now - g_fogSaved > FOG_SAVE_MS )
      FogSave();
  }
  inline bool FogBuildTex() {
    if( !zrenderer ) return false;
    if( !g_fogTex ) {
      zTRnd_TextureFormat fmt = zRND_TEX_FORMAT_ARGB_4444;
      int bpp = 0;
      if( !PickTexFormat( true, fmt, bpp ) ) return false;
      g_fogTex = CreateTex2D( FOG_N, FOG_N, fmt, zCOLOR( 0, 0, 0, 255 ), false );
      if( !g_fogTex ) return false;
      g_fogFmt = (int)fmt; g_fogBpp = bpp;
    }
    if( !g_fogPx ) {
      g_fogPx = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( FOG_N * FOG_N * 4 ) );
      if( !g_fogPx ) return false;
    }
    int alphaMax = g_fogAlpha * 255 / 100;
    if( alphaMax < 0 ) alphaMax = 0; else if( alphaMax > 255 ) alphaMax = 255;
    const unsigned long dark = 0x0E0C0Aul;
    for( int z = 0; z < FOG_N; z++ )
      for( int x = 0; x < FOG_N; x++ ) {
        int open = 0, cnt = 0;
        for( int dz = -1; dz <= 1; dz++ )
          for( int dx = -1; dx <= 1; dx++ ) {
            const int xx = x + dx, zz = z + dz;
            if( xx < 0 || zz < 0 || xx >= FOG_N || zz >= FOG_N ) continue;
            cnt++;
            open += FogBit( xx, zz ) ? 1 : 0;
          }
        const int a = alphaMax - alphaMax * open / ( cnt > 0 ? cnt : 1 );
        const unsigned long c = ( (unsigned long)a << 24 ) | dark;
        const int p = z * FOG_N + x;
        if( g_fogBpp == 4 )
          ( (unsigned long*)g_fogPx )[p] = c;
        else
          ( (unsigned short*)g_fogPx )[p] = GenPack16( c, g_fogFmt, 0.0f );
      }
    if( !GenUpload( g_fogTex, g_fogPx, FOG_N, g_fogBpp ) ) return false;
    g_fogTexDirty = false;
    return true;
  }
  static int g_fogAlphaUsed = -1;
  inline void DrawFog( int x, int y, int w, int h ) {
    if( !g_fog || !g_fogLoaded || g_mapIdx < 0 ) return;
    if( g_fogAlphaUsed != g_fogAlpha ) {
      g_fogAlphaUsed = g_fogAlpha;
      g_fogTexDirty  = true;
    }
    if( ( g_fogTexDirty || !g_fogTex ) && !FogBuildTex() ) return;
    g_curTex   = g_fogTex;
    g_curRect  = &g_fogRect;
    g_curBlend = true;
    g_curAlpha = 255;
    DrawMapTexture( x, y, w, h );
    g_curAlpha = -1;
    g_curTex   = 0;
    g_curRect  = 0;
    g_curBlend = false;
  }
}
