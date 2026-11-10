#pragma once
namespace MiniMapClash {
  using MiniMapCfg::Low;
  using MiniMapCfg::BaseName;
  inline bool StartsWithI( const char* s, const char* pre ) { return MiniMapCfg::StartsNoCase( s, pre ); }
  inline bool EqI( const char* a, const char* b ) { return MiniMapCfg::EqNoCase( a, b ); }
  inline bool IsSystemModule( const char* path, const char* winDir ) {
    if( !path || !winDir || !winDir[0] || !StartsWithI( path, winDir ) ) return false;
    int n = (int)strlen( winDir );
    const char last = winDir[n - 1];
    if( last == '\\' || last == '/' ) return true;
    return path[n] == '\\' || path[n] == '/';
  }
  struct TKnown { const char* name; const char* what; };
  inline const char* KnownKind( const char* base ) {
    static const TKnown k[] = {
      { "zmousefix.dll",   "Union mouse wrapper (SystemPack.ini [MOUSE])" },
      { "dinput.dll",      "DirectInput replacement in game folder (GRawInput?)" },
      { "dinput8.dll",     "DirectInput 8 replacement in game folder" },
      { "ddraw.dll",       "DirectDraw replacement (DX11 renderer / dgVoodoo / wrapper)" },
      { "d3d11.dll",       "Direct3D 11 in game folder" },
      { "d3d9.dll",        "Direct3D 9 in game folder (ReShade / wrapper)" },
      { "dxgi.dll",        "DXGI in game folder (ReShade?)" },
      { "gd3d11.dll",      "GD3D11 renderer (DX11)" },
      { "dgvoodoo.dll",    "dgVoodoo" },
      { "reshade.dll",     "ReShade" },
      { "gameoverlayrenderer.dll", "Steam overlay" },
      { "discordhook.dll", "Discord overlay" },
      { "rtsshooks.dll",   "RivaTuner / MSI Afterburner overlay" },
      { "nvspcap.dll",     "NVIDIA ShadowPlay / overlay" },
      { "lghub_agent.dll", "Logitech G HUB" },
      { "union.dll",       "Union" },
      { "shw32.dll",       "Union / SystemPack" },
      { "vdfs32g.dll",     "VDFS (SystemPack)" },
      { "systempack.dll",  "SystemPack" },
      { "zspy.dll",        "zSpy" },
      { "minimap.dll",     "MiniMap (this plugin)" },
    };
    for( int i = 0; i < (int)( sizeof( k ) / sizeof( k[0] ) ); i++ )
      if( EqI( base, k[i].name ) ) return k[i].what;
    return 0;
  }
  inline bool CodeHooked( const unsigned char* c ) {
    return c && ( c[0] == 0xE9 || c[0] == 0xE8 || c[0] == 0xEB
        || ( c[0] == 0xFF && ( c[1] == 0x25 || c[1] == 0x15 ) )
        || ( c[0] == 0x68 && c[5] == 0xC3 )
        || ( c[0] == 0xB8 && c[5] == 0xFF && c[6] == 0xE0 ) );
  }
  inline unsigned int HookTarget( const unsigned char* c, unsigned int at ) {
    if( !c ) return 0;
    if( c[0] == 0xE9 || c[0] == 0xE8 ) {
      const int rel = (int)( (unsigned int)c[1] | ( (unsigned int)c[2] << 8 )
                     | ( (unsigned int)c[3] << 16 ) | ( (unsigned int)c[4] << 24 ) );
      return at + 5u + (unsigned int)rel;
    }
    if( c[0] == 0x68 && c[5] == 0xC3 )
      return (unsigned int)c[1] | ( (unsigned int)c[2] << 8 )
           | ( (unsigned int)c[3] << 16 ) | ( (unsigned int)c[4] << 24 );
    if( c[0] == 0xB8 && c[5] == 0xFF && c[6] == 0xE0 )
      return (unsigned int)c[1] | ( (unsigned int)c[2] << 8 )
           | ( (unsigned int)c[3] << 16 ) | ( (unsigned int)c[4] << 24 );
    return 0;
  }
}
