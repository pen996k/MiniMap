#pragma once
namespace MiniMap {
  inline void DrawUserMarks() {
    DrawModPoints();
    if( !g_marks || !g_world[0] || g_ownCount <= 0 ) return;
    const float sm    = MarkPxMini();
    const float r     = sm * 0.3f;
    const float s     = g_full ? MarkPxFull() : sm;
    const int   under = g_full ? OwnMarkAtCursor() : -1;
    for( int i = 0; i < g_ownCount; i++ ) {
      const TUserMark& u = g_ownMarks[i];
      if( !MarkHere( u ) ) continue;
      float mx, my;
      WorldToPx( u.x, u.z, mx, my );
      if( !InDisc( mx, my ) ) continue;
      if( g_full ) {
        if( mx < g_clipX0 - s || mx > g_clipX1 + s ) continue;
        if( my < g_clipY0 - s || my > g_clipY1 + s ) continue;
        DrawMarkIcon( mx, my, s, u.icon );
        if( i == g_guide && g_markGuide )
          SearchRing( mx, my, s * 0.9f );
        if( g_markLabels == 2 && u.note[0] && i != under )
          MarkText( mx + s * 0.6f + 2.0f, my - LineHPx() * 0.5f,
                    u.note, ColMarkText() );
      }
      else {
        if( DrawMarkAtlas( u.icon, mx, my, s ) ) continue;
        DrawUserMarkShape( u.icon, mx, my, r + 1.0f, zCOLOR( 18, 16, 15, 255 ) );
        DrawUserMarkShape( u.icon, mx, my, r, ColMarkIcon( u.icon ) );
      }
    }
    if( g_full && g_markLabels >= 1 && g_edit < 0 && g_curReady
        && under >= 0 && g_ownMarks[under].note[0] ) {
      float nx = 0.0f, ny = 0.0f;
      WorldToPx( g_ownMarks[under].x, g_ownMarks[under].z, nx, ny );
      Bubble( nx + s * 0.6f, ny + s * 0.4f, g_ownMarks[under].note );
    }
    if( g_full && g_edit < 0 && g_curReady && under < 0 ) {
      float sc = s * 0.7f;
      const float scMin = 10.0f * UiScale();
      if( sc < scMin ) sc = scMin;
      DrawMarkIcon( g_curX + CursorPx() * 0.75f,
                    g_curY + CursorPx() * 1.05f,
                    sc, g_markIcon );
    }
  }
  inline bool GuideEdge( float tx, float ty, float margin,
                         float& ex, float& ey, float& fx, float& fy ) {
    const float dx = tx - g_cx, dy = ty - g_cy;
    const float len = sqrtf( dx * dx + dy * dy );
    if( len < 0.001f ) return false;
    fx = dx / len;
    fy = dy / len;
    float lim;
    if( g_shape )
      lim = DiscR() - margin;
    else {
      const float hx = ( g_clipX1 - g_clipX0 ) * 0.5f - margin;
      const float hy = ( g_clipY1 - g_clipY0 ) * 0.5f - margin;
      const float kx = ( fx != 0.0f ) ? hx / fabsf( fx ) : 1.0e30f;
      const float ky = ( fy != 0.0f ) ? hy / fabsf( fy ) : 1.0e30f;
      lim = ( kx < ky ) ? kx : ky;
    }
    if( lim < 1.0f ) lim = 1.0f;
    if( len <= lim ) return false;
    ex = g_cx + fx * lim;
    ey = g_cy + fy * lim;
    return true;
  }
  inline void GuideDistText( float cm, char* out, int max ) {
    if( !( cm >= 0.0f ) ) cm = 0.0f;
    if( cm > 99999900.0f ) cm = 99999900.0f;
    int m = (int)( cm / 100.0f + 0.5f );
    if( m < 0 ) m = 0; else if( m > 999999 ) m = 999999;
    if( out && max > 0 ) snprintf( out, (size_t)max, "%d m", m );
  }
  inline void GuideLabel( const char* note, const char* dist, char* out, int max ) {
    if( !out || max <= 0 ) return;
    if( note && note[0] ) snprintf( out, (size_t)max, "%.20s - %s", note, dist ? dist : "" );
    else strncpy_s( out, (size_t)max, dist ? dist : "", _TRUNCATE );
  }
  inline void GuideText( const TUserMark& u, float px, float pz, char* out, int max ) {
    const float dx = u.x - px, dz = u.z - pz;
    char dist[24];
    GuideDistText( sqrtf( dx * dx + dz * dz ), dist, sizeof( dist ) );
    GuideLabel( u.note, dist, out, max );
  }
  inline void DrawGuide() {
    if( !g_markGuide || !g_marks || g_full || !player ) return;
    if( g_guide < 0 || g_guide >= g_ownCount || !MarkHere( g_ownMarks[g_guide] ) ) return;
    const zVEC3 p = player->GetPositionWorld();
    if( GuideReached( p[0], p[2] ) ) {
      MM_LOG( "guide: target reached" );
      g_guide = -1;
      SaveOwnMarks();
      return;
    }
    const TUserMark& u = g_ownMarks[g_guide];
    float tx, ty;
    WorldToPx( u.x, u.z, tx, ty );
    const float s = MarkPxMini();
    const float margin = s * 0.75f + 2.0f;
    float ex, ey, fx, fy;
    const zCOLOR gold( 255, 214, 64, 255 );
    if( !GuideEdge( tx, ty, margin, ex, ey, fx, fy ) ) {
      SearchRing( tx, ty, s * 0.9f );
      return;
    }
    const float al = s * 1.25f;
    if( !DrawIconRot( ICON_PLAYER, ex, ey, al, fx, fy ) ) {
      const float sx = -fy, sy = fx;
      const float tipX = ex + fx * al * 0.5f, tipY = ey + fy * al * 0.5f;
      FillTriTex( tipX, tipY, ex - fx * al * 0.35f + sx * al * 0.4f, ey - fy * al * 0.35f + sy * al * 0.4f,
                  ex - fx * al * 0.35f - sx * al * 0.4f, ey - fy * al * 0.35f - sy * al * 0.4f, gold );
    }
    const float ix = ex - fx * al * 0.95f, iy = ey - fy * al * 0.95f;
    if( !DrawMarkAtlas( u.icon, ix, iy, s * 0.8f ) )
      DrawUserMarkShape( u.icon, ix, iy, s * 0.25f, ColMarkIcon( u.icon ) );
    if( g_markGuide >= 2 ) {
      char txt[80];
      GuideText( u, p[0], p[2], txt, sizeof( txt ) );
      const float tw = TextWidthPx( txt );
      float lx = ix - tw * 0.5f - fx * s * 0.6f;
      float ly = iy - LineHPx() * 0.5f - fy * s * 0.9f;
      if( lx < g_clipX0 + 2.0f ) lx = g_clipX0 + 2.0f;
      if( lx + tw > g_clipX1 - 2.0f ) lx = g_clipX1 - 2.0f - tw;
      if( ly < g_clipY0 + 1.0f ) ly = g_clipY0 + 1.0f;
      if( ly + LineHPx() > g_clipY1 - 1.0f ) ly = g_clipY1 - 1.0f - LineHPx();
      MarkText( lx + 1.0f, ly + 1.0f, txt, zCOLOR( 16, 14, 13, 255 ) );
      MarkText( lx, ly, txt, gold );
    }
  }
  static bool   g_hovNpc   = false;
  static float  g_hovD2    = 0.0f;
  inline void HoverConsider( zCVob* vob, float mx, float my, float s, bool isNpc ) {
    if( !g_hoverInfo || !g_full || !g_curReady || g_edit >= 0 || !vob ) return;
    if( ( isNpc && g_hoverInfo == 3 ) || ( !isNpc && g_hoverInfo == 2 ) ) return;
    if( g_curX < g_clipX0 || g_curX > g_clipX1 || g_curY < g_clipY0 || g_curY > g_clipY1 ) return;
    float r = s * 0.65f;
    const float rmin = 8.0f * UiScale();
    if( r < rmin ) r = rmin;
    const float dx = mx - g_curX, dy = my - g_curY;
    const float d2 = dx * dx + dy * dy;
    if( d2 > r * r ) return;
    if( g_hovVob ) {
      const float bias = ( isNpc && !g_hovNpc ) ? 0.5f : ( !isNpc && g_hovNpc ) ? 2.0f : 1.0f;
      if( d2 * bias >= g_hovD2 ) return;
    }
    g_hovVob = vob;
    g_hovNpc = isNpc;
    g_hovD2  = d2;
  }
  inline bool GuildName( int guild, zSTRING& out ) {
    if( !parser || guild <= 0 ) return false;
    zCPar_Symbol* const sym = parser->GetSymbol( zSTRING( "TXT_GUILDS" ) );
    if( !sym || sym->type != zPAR_TYPE_STRING || guild >= (int)sym->ele ) return false;
    zSTRING s;
    sym->GetValue( s, guild );
    if( s.Length() <= 0 ) return false;
    out = s;
    return true;
  }
  inline zCOLOR HovReadable( const zCOLOR& c ) {
    const int lum = ( (int)c.r * 299 + (int)c.g * 587 + (int)c.b * 114 ) / 1000;
    if( lum >= 140 ) return zCOLOR( c.r, c.g, c.b, 255 );
    const int k = ( ( 140 - lum ) * 256 ) / ( 255 - lum );
    return zCOLOR( (unsigned char)( c.r + ( ( 255 - c.r ) * k >> 8 ) ),
                   (unsigned char)( c.g + ( ( 255 - c.g ) * k >> 8 ) ),
                   (unsigned char)( c.b + ( ( 255 - c.b ) * k >> 8 ) ), 255 );
  }
  inline bool MobNameIsKey( const zSTRING& nm, const zSTRING& focus ) {
    return NameIsKey( const_cast<zSTRING&>( nm ).ToChar(), const_cast<zSTRING&>( focus ).ToChar() );
  }
  inline zSTRING CapFirst( const zSTRING& s ) {
    if( s.Length() <= 0 ) return s;
    const char* p = s.ToChar();
    const unsigned char c0 = (unsigned char)p[0];
    const unsigned char cu = CapFirstChar( c0 );
    if( cu == c0 ) return s;
    return zSTRING( (char)cu ) + ( p + 1 );
  }
  static const int HOV_LIST_MAX = 256;
  struct THovRow {
    char          text[INVN_NAME + 16];
    unsigned char r, g, b;
  };
  static THovRow g_hovList[HOV_LIST_MAX];
  static int     g_hovListN    = 0;
  static int     g_hovListMore = 0;
  static int     g_hovComboLine = -1;
  static int     g_hovComboPre  = 0;
  inline zCOLOR ColComboL() { return zCOLOR( 235,  60,  50, 255 ); }
  inline zCOLOR ColComboR() { return zCOLOR(  70, 215,  80, 255 ); }
  inline void HovRowText( char* out, int cap, const char* name, int amount ) {
    if( !out || cap <= 0 ) return;
    if( amount > 1 ) snprintf( out, (size_t)cap, "%s x%u", name ? name : "", (unsigned)amount );
    else strncpy_s( out, (size_t)cap, name ? name : "", _TRUNCATE );
  }
  inline void HoverBubble( float x, float y, const zSTRING* lines, int n,
                           const zCOLOR& title, int tint ) {
    const int ln    = g_hovListN;
    const int more0 = g_hovListMore;
    const int comboLine = g_hovComboLine;
    const int comboPre  = g_hovComboPre;
    g_hovListN    = 0;
    g_hovListMore = 0;
    g_hovComboLine = -1;
    g_hovComboPre  = 0;
    if( n <= 0 ) return;
    float pad = 4.0f * UiScale();
    if( pad < 4.0f ) pad = 4.0f;
    const float lh  = LineHPx();
    if( lh > 0.0f ) {
      const int fit = (int)( ( g_clipY1 - g_clipY0 - pad * 2.0f ) / lh );
      if( fit >= 1 && n > fit ) n = fit;
    }
    float w = 0.0f;
    for( int i = 0; i < n; i++ ) {
      const float t = TextWidthPx( lines[i].ToChar() );
      if( t > w ) w = t;
    }
    const float ind = lh * 0.5f;
    int   rpc = 0, shown = 0, more = 0;
    float colW = 0.0f;
    if( ln > 0 && lh > 0.0f ) {
      float rw = TextWidthPx( T( TX_HOV_MORE ) ) + TextWidthPx( " 999" );
      for( int i = 0; i < ln; i++ ) {
        const float t = TextWidthPx( g_hovList[i].text );
        if( t > rw ) rw = t;
      }
      colW = rw + lh;
      int fitRows = (int)( ( g_clipY1 - g_clipY0 - pad * 2.0f ) / lh ) - n;
      if( fitRows < 1 ) fitRows = 1;
      int maxCols = (int)( ( g_clipX1 - g_clipX0 - pad * 2.0f - ind + lh ) / colW );
      if( maxCols < 1 ) maxCols = 1;
      int need = ln + ( more0 > 0 ? 1 : 0 );
      int cols = ( need + fitRows - 1 ) / fitRows;
      if( cols > maxCols ) cols = maxCols;
      shown = ln;
      more  = more0;
      if( need > cols * fitRows ) {
        shown = cols * fitRows - 1;
        more  = more0 + ( ln - shown );
        need  = cols * fitRows;
      }
      rpc = ( need + cols - 1 ) / cols;
      const float lw = ind + (float)cols * colW - lh;
      if( lw > w ) w = lw;
    }
    const float bw = w + pad * 2.0f;
    const float bh = lh * (float)( n + rpc ) + pad * 2.0f;
    if( x + bw > g_clipX1 ) x = g_clipX1 - bw;
    if( y + bh > g_clipY1 ) y = g_clipY1 - bh;
    if( x < g_clipX0 ) x = g_clipX0;
    if( y < g_clipY0 ) y = g_clipY0;
    Pane( x, y, bw, bh );
    if( tint < 1 ) tint = 1;
    const zCOLOR tc = HovReadable( title );
    for( int i = 0; i < n; i++ ) {
      const zCOLOR lc = ( i < tint ) ? tc : ColMarkText();
      const float  ly = y + pad + lh * (float)i;
      if( i == comboLine && comboPre > 0 && comboPre < lines[i].Length() ) {
        const char* const s = lines[i].ToChar();
        char buf[128];
        MiniMapCfg::CopyN( buf, sizeof( buf ), s, comboPre );
        int k = (int)strlen( buf );
        MarkText( x + pad, ly, buf, lc );
        for( ; s[k] && k < (int)sizeof( buf ) - 2; k++ ) {
          const float cx = x + pad + TextWidthPx( buf );
          char one[2] = { s[k], 0 };
          MarkText( cx, ly, one, ( s[k] == 'L' ) ? ColComboL() : ColComboR() );
          buf[k] = s[k];
          buf[k + 1] = 0;
        }
        continue;
      }
      MarkText( x + pad, ly, lines[i].ToChar(), lc );
    }
    for( int i = 0; i < shown && rpc > 0; i++ ) {
      const THovRow& r = g_hovList[i];
      MarkText( x + pad + ind + (float)( i / rpc ) * colW, y + pad + lh * (float)( n + i % rpc ),
                r.text, zCOLOR( r.r, r.g, r.b, 255 ) );
    }
    if( more > 0 && rpc > 0 ) {
      const zSTRING ms = zSTRING( T( TX_HOV_MORE ) ) + " " + zSTRING( more );
      MarkText( x + pad + ind + (float)( shown / rpc ) * colW, y + pad + lh * (float)( n + shown % rpc ),
                ms.ToChar(), ColMarkText() );
    }
  }
  inline const char* HovItemCat( int mf ) {
    if( mf & ITM_CAT_NF )     return T( TX_CAT_MELEE );
    if( mf & ITM_CAT_FF )     return T( TX_CAT_RANGED );
    if( mf & ITM_CAT_MUN )    return T( TX_CAT_AMMO );
    if( mf & ITM_CAT_ARMOR )  return T( TX_CAT_ARMOR );
    if( mf & ITM_CAT_FOOD )   return T( TX_CAT_FOOD );
    if( mf & ITM_CAT_DOCS )   return T( TX_CAT_DOCS );
    if( mf & ITM_CAT_POTION ) return T( TX_CAT_POTION );
    if( mf & ITM_CAT_LIGHT )  return T( TX_CAT_LIGHT );
    if( mf & ITM_CAT_RUNE )   return T( TX_CAT_RUNE );
    if( mf & ITM_CAT_MAGIC )  return T( TX_CAT_MAGIC );
    return 0;
  }
  inline const char* HovAtrName( int a ) {
    switch( a ) {
      case NPC_ATR_HITPOINTS: case NPC_ATR_HITPOINTSMAX: return T( TX_HOV_HP );
      case NPC_ATR_MANA:      case NPC_ATR_MANAMAX:      return T( TX_HOV_MANA );
      case NPC_ATR_STRENGTH:  return T( TX_HOV_STR );
      case NPC_ATR_DEXTERITY: return T( TX_HOV_DEX );
    }
    return 0;
  }
  inline bool HovProtLine( const int* prot, zSTRING& out ) {
    static const int idx[4] = { oEDamageIndex_Edge, oEDamageIndex_Point,
                                oEDamageIndex_Magic, oEDamageIndex_Fire };
    const int tx[4] = { TX_HOV_WEAPON, TX_HOV_P_ARROW, TX_HOV_P_MAGIC, TX_HOV_P_FIRE };
    zSTRING l;
    for( int i = 0; i < 4; i++ ) {
      const int v = prot[idx[i]];
      if( v == 0 ) continue;
      if( l.Length() > 0 ) l = l + ", ";
      l = l + zSTRING( T( tx[i] ) ) + " " + ( ( v < 0 ) ? zSTRING( T( TX_HOV_IMMUNE ) ) : zSTRING( v ) );
    }
    if( l.Length() <= 0 ) return false;
    out = zSTRING( T( TX_HOV_PROT ) ) + ": " + l;
    return true;
  }
  inline int    ItemIconOf( oCItem* item );
  inline zCOLOR ItemIconColor( int icon );
  inline void HovListAdd( const char* name, int amount, int icon ) {
    if( g_hovListN >= HOV_LIST_MAX ) return;
    THovRow& r = g_hovList[g_hovListN++];
    HovRowText( r.text, (int)sizeof( r.text ), name, amount );
    zCOLOR c = ColMarkText();
    if( g_itemColors ) {
      const int ic = ( icon < 0 ) ? ICON_ITEM_OTHER : icon;
      c = HovReadable( ItemIconColor( ic ) );
    }
    r.r = c.r;
    r.g = c.g;
    r.b = c.b;
  }
  enum { HTX_NONE = 0, HTX_STR, HTX_DEX, HTX_MANA, HTX_HP, HTX_DMG,
         HTX_PE, HTX_PP, HTX_PF, HTX_PM };
  struct THtxSym { const char* sym; unsigned char kind; };
  static const THtxSym kHtxSyms[] = {
    { "NAME_STR_NEEDED",       HTX_STR  }, { "NAME_DEX_NEEDED",       HTX_DEX  },
    { "NAME_MANA_NEEDED",      HTX_MANA }, { "NAME_MANAKOSTEN",       HTX_MANA },
    { "NAME_MANAKOSTENMAX",    HTX_MANA }, { "NAME_MINMANAKOSTEN",    HTX_MANA },
    { "NAME_HEALTH_NEEDED",    HTX_HP   }, { "NAME_DAMAGE",           HTX_DMG  },
    { "NAME_DAMAGETYPE_EDGE",  HTX_DMG  }, { "NAME_DAMAGETYPE_POINT", HTX_DMG  },
    { "NAME_DAMAGETYPE_BLUNT", HTX_DMG  }, { "NAME_DAMAGETYPE_MAGIC", HTX_DMG  },
    { "NAME_DAMAGETYPE_FIRE",  HTX_DMG  }, { "NAME_PROT_EDGE",        HTX_PE   },
    { "NAME_PROT_POINT",       HTX_PP   }, { "NAME_PROT_FIRE",        HTX_PF   },
    { "NAME_PROT_MAGIC",       HTX_PM   },
  };
  enum { HTX_N = sizeof( kHtxSyms ) / sizeof( kHtxSyms[0] ), HTX_LEN = 64 };
  static char  g_htxText[HTX_N][HTX_LEN];
  static void* g_htxParser = 0;
  inline void HovNormEnd( const char* src, char* dst, int max ) {
    HovNormalize( src, dst, max );
    int n = (int)strlen( dst );
    while( n > 0 && ( dst[n - 1] == ' ' || dst[n - 1] == '.' || dst[n - 1] == ':' || dst[n - 1] == '!' ) )
      dst[--n] = 0;
  }
  inline bool HovSameText( const char* a, const char* b ) {
    if( !a || !b || !a[0] || !b[0] ) return false;
    char na[128], nb[128];
    HovNormEnd( a, na, 128 );
    HovNormEnd( b, nb, 128 );
    return na[0] && strcmp( na, nb ) == 0;
  }
  inline zCPar_Symbol* HtxSymbol( const char* name ) {
    return parser->GetSymbol( zSTRING( name ) );
  }
  inline void HtxLoadCore() {
    __try {
      for( int i = 0; i < HTX_N; i++ ) {
        g_htxText[i][0] = 0;
        zCPar_Symbol* const s = HtxSymbol( kHtxSyms[i].sym );
        if( s && s->type == zPAR_TYPE_STRING && s->ele == 1 && s->stringdata )
          HovNormEnd( s->stringdata->ToChar(), g_htxText[i], HTX_LEN );
      }
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      for( int i = 0; i < HTX_N; i++ )
        g_htxText[i][0] = 0;
    }
  }
  inline void HtxEnsure() {
    if( !parser || g_htxParser == (void*)parser ) return;
    g_htxParser = (void*)parser;
    HtxLoadCore();
    int n = 0;
    for( int i = 0; i < HTX_N; i++ )
      if( g_htxText[i][0] )
        n++;
    MM_LOG( zSTRING( "item tooltip: " ) + zSTRING( n ) + " of " + zSTRING( (int)HTX_N ) + " script stat labels found (duplicates hidden)" );
  }
  inline int HtxKindOf( const zSTRING& text ) {
    if( text.Length() <= 0 ) return HTX_NONE;
    char n[HTX_LEN];
    HovNormEnd( text.ToChar(), n, HTX_LEN );
    if( !n[0] ) return HTX_NONE;
    for( int i = 0; i < HTX_N; i++ )
      if( g_htxText[i][0] && strcmp( g_htxText[i], n ) == 0 ) return kHtxSyms[i].kind;
    return HTX_NONE;
  }
  inline int HtxKindOfAtr( int a ) {
    switch( a ) {
      case NPC_ATR_HITPOINTS: case NPC_ATR_HITPOINTSMAX: return HTX_HP;
      case NPC_ATR_MANA:      case NPC_ATR_MANAMAX:      return HTX_MANA;
      case NPC_ATR_STRENGTH:  return HTX_STR;
      case NPC_ATR_DEXTERITY: return HTX_DEX;
    }
    return HTX_NONE;
  }
  inline void HoverDraw() {
    zCVob* const vob = g_hovVob;
    const bool   npcK = g_hovNpc;
    g_hovVob = 0;
    g_hovListN    = 0;
    g_hovListMore = 0;
    g_hovComboLine = -1;
    if( !vob || !g_hoverInfo || !g_full || !g_curReady || g_edit >= 0 || !screen ) return;
    if( g_marks && OwnMarkAtCursor() >= 0 ) return;
    static const int HOV_MAX = 24;
    zSTRING lines[HOV_MAX];
    int     n = 0;
    zCOLOR  title = ColMarkLight();
    int     tint  = 1;
    const int det = g_hoverDetail;
    if( npcK ) {
      oCNpc* const npc = (oCNpc*)vob;
      const int grp = NpcGroup( npc );
      title = ColNpcGroup( grp );
      if( npc->attribute[NPC_ATR_HITPOINTS] <= 0 )
        title = ColCorpse();
      else if( grp != 0 && g_showTraders && TraderIs( npc ) )
        title = ColTrader();
      lines[n++] = ( npc->name[0].Length() > 0 ) ? CapFirst( npc->name[0] ) : zSTRING( "?" );
      if( const char* tn = NameTxtInst( npc->instanz ) ) lines[0] = zSTRING( tn );
      if( det >= 1 ) {
      const char* att = ( grp == 0 ) ? T( TX_HOV_HOSTILE )
                      : ( grp == 2 ) ? T( TX_HOV_FRIENDLY ) : T( TX_HOV_NEUTRAL );
      if( npc->attribute[NPC_ATR_HITPOINTS] > 0 )
        lines[n++] = zSTRING( att );
      if( npc->attribute[NPC_ATR_HITPOINTS] <= 0 )
        lines[n++] = zSTRING( T( TX_HOV_CORPSE ) );
      else if( TraderIs( npc ) )
        lines[n++] = zSTRING( T( TX_HOV_TRADER ) );
      tint = n;
      zSTRING l2 = zSTRING( T( TX_HOV_LEVEL ) ) + " " + zSTRING( npc->level );
      zSTRING gn;
      if( GuildName( npc->guild, gn ) ) l2 = l2 + "   " + zSTRING( T( TX_HOV_GUILD ) ) + ": " + CapFirst( gn );
      lines[n++] = l2;
      lines[n++] = zSTRING( T( TX_HOV_HP ) ) + " " + zSTRING( npc->attribute[NPC_ATR_HITPOINTS] ) + "/" + zSTRING( npc->attribute[NPC_ATR_HITPOINTSMAX] );
      }
      if( det >= 2 ) {
        zSTRING l4;
        if( npc->attribute[NPC_ATR_MANAMAX] > 0 )
          l4 = zSTRING( T( TX_HOV_MANA ) ) + " " + zSTRING( npc->attribute[NPC_ATR_MANA] ) + "/" + zSTRING( npc->attribute[NPC_ATR_MANAMAX] ) + "   ";
        l4 = l4 + zSTRING( T( TX_HOV_STR ) ) + " " + zSTRING( npc->attribute[NPC_ATR_STRENGTH] ) + "   " + zSTRING( T( TX_HOV_DEX ) ) + " " + zSTRING( npc->attribute[NPC_ATR_DEXTERITY] );
        lines[n++] = l4;
        oCItem* const wm = npc->GetEquippedMeleeWeapon();
        oCItem* const wr = npc->GetEquippedRangedWeapon();
        zSTRING lw;
        if( wm && wm->name.Length() > 0 ) lw = wm->name;
        if( wr && wr->name.Length() > 0 ) lw = ( lw.Length() > 0 ) ? lw + ", " + wr->name : wr->name;
        if( lw.Length() > 0 && n < HOV_MAX )
          lines[n++] = zSTRING( T( TX_HOV_WEAPON ) ) + ": " + CapFirst( lw );
        oCItem* const ar = npc->GetEquippedArmor();
        if( ar && ar->name.Length() > 0 && n < HOV_MAX )
          lines[n++] = zSTRING( T( TX_HOV_ARMOR ) ) + ": " + CapFirst( ar->name );
        zSTRING lp;
        if( HovProtLine( npc->protection, lp ) && n < HOV_MAX )
          lines[n++] = lp;
      }
      const bool dead   = npc->attribute[NPC_ATR_HITPOINTS] <= 0;
      const bool trader = TraderIs( npc );
      if( det >= 1 && g_hoverInv && ( dead || trader || g_hoverInv >= 2 ) ) {
        const int mode = dead ? INV_LOOT : trader ? INV_GOODS : INV_ALL;
        const int lim  = ( !dead && trader ) ? HOV_LIST_MAX : ( det >= 2 ) ? 12 : 8;
        static TInvRow rows[HOV_LIST_MAX];
        int other = 0;
        const int nr = InvCollect( npc, mode, rows, lim, other );
        if( n < HOV_MAX )
          lines[n++] = zSTRING( T( dead ? TX_HOV_LOOT : trader ? TX_HOV_GOODS : TX_HOV_INV ) ) + ":";
        if( nr == 0 && n < HOV_MAX )
          lines[n++] = zSTRING( "  " ) + zSTRING( T( TX_HOV_NONE ) );
        for( int k = 0; k < nr; k++ )
          HovListAdd( rows[k].name, rows[k].amount, rows[k].icon );
        g_hovListMore = other;
      }
    }
    else if( VobIsChest( vob ) ) {
      oCMobContainer* const ch = (oCMobContainer*)vob;
      const int kind = ChestKind( ch );
      title = AtlasColor( kind );
      zSTRING nm = ch->GetName();
      lines[n++] = ( nm.Length() > 0 ) ? CapFirst( nm ) : zSTRING( T( TX_HOV_CHEST ) );
      if( det >= 1 ) {
        if( kind != ICON_CHEST ) {
          zSTRING ll = zSTRING( T( kind == ICON_CHEST_KEY  ? TX_HOV_LOCK_KEY
                                 : kind == ICON_CHEST_PICK ? TX_HOV_LOCK_PICK
                                 : kind == ICON_CHEST_BOTH ? TX_HOV_LOCK_BOTH
                                 :                           TX_HOV_LOCKED ) );
          lines[n++] = ll;
          const int plen = ch->pickLockStr.Length();
          if( g_chestCombo > 0 && plen > 0 && n < HOV_MAX - 2 ) {
            if( g_chestCombo == 1 )
              lines[n++] = zSTRING( T( TX_HOV_MOVES ) ) + zSTRING( plen );
            else {
              char cb[96];
              int  k = 0;
              const char* const ps = ch->pickLockStr.ToChar();
              for( int i = 0; ps && ps[i] && k < (int)sizeof( cb ) - 3; i++ ) {
                char c = ps[i];
                if( c >= 'a' && c <= 'z' ) c = (char)( c - 32 );
                if( c == 'R' && g_lang == LANG_PL ) c = 'P';
                cb[k++] = c;
              }
              cb[k] = 0;
              { const zSTRING pre = zSTRING( T( TX_HOV_COMBO ) ); g_hovComboLine = n; g_hovComboPre = pre.Length(); lines[n++] = pre + zSTRING( cb ); }
            }
          }
          const int klen = ch->keyInstance.Length();
          if( g_chestCombo > 0 && klen > 0 && n < HOV_MAX - 2 ) {
            char kval[INVN_KEY];
            const char* const ks = ch->keyInstance.ToChar();
            int q = 0;
            for( ; ks && ks[q] && q < INVN_KEY - 1; q++ )
              kval[q] = UpChar( ks[q] );
            kval[q] = 0;
            const TInvName* const en = kval[0] ? InvNameGet( kval ) : 0;
            zSTRING kl = zSTRING( T( TX_HOV_KEYNAME ) );
            if( en && en->state == 1 )
              kl = kl + zSTRING( en->name ) + " (" + zSTRING( kval ) + ")";
            else
              kl = kl + zSTRING( kval );
            lines[n++] = kl;
          }
        }
        tint = n;
        static const int CMAX = 64;
        zSTRING cn[CMAX];
        int     ca[CMAX];
        int     nc = 0, other = 0, guard = 0;
        for( zCListSort<oCItem>* e = ch->containList.next; e && guard < 4096; e = e->next, guard++ ) {
          oCItem* const it = e->data;
          if( !it ) continue;
          zSTRING in = ( it->name.Length() > 0 ) ? CapFirst( it->name )
                     : ( it->description.Length() > 0 ) ? CapFirst( it->description ) : zSTRING( "?" );
          const int am = ( it->amount > 0 ) ? it->amount : 1;
          int k = 0;
          while( k < nc && !( cn[k] == in ) ) k++;
          if( k < nc ) { ca[k] += am; continue; }
          if( nc < CMAX ) { cn[nc] = in; ca[nc] = am; nc++; }
          else other++;
        }
        if( nc == 0 )
          lines[n++] = zSTRING( T( TX_HOV_EMPTY ) );
        else {
          const int lim = ( det >= 2 ) ? 12 : 6;
          int k = 0;
          for( ; k < nc && k < lim && n < HOV_MAX - 1; k++ )
            lines[n++] = ( ca[k] > 1 ) ? cn[k] + " x" + zSTRING( ca[k] ) : cn[k];
          const int rest = ( nc - k ) + other;
          if( rest > 0 && n < HOV_MAX )
            lines[n++] = zSTRING( T( TX_HOV_MORE ) ) + " " + zSTRING( rest );
        }
      }
    }
    else if( SwitchKindOf( vob ) >= 0 ) {
      const int kind = SwitchKindOf( vob );
      oCMobInter* const mi = (oCMobInter*)vob;
      title = AtlasColor( kind );
      const char* const kt = T( kind == ICON_WHEEL ? TX_HOV_SW_WHEEL
                              : kind == ICON_LEVER ? TX_HOV_SW_LEVER : TX_HOV_SW_TRIGGER );
      zSTRING nm = mi->GetName();
      if( MobNameIsKey( nm, mi->name ) ) nm = zSTRING( "" );
      lines[n++] = ( nm.Length() > 0 ) ? CapFirst( nm ) : zSTRING( kt );
      if( nm.Length() > 0 && !HovKindInName( nm.ToChar(), kt ) )
        lines[n++] = zSTRING( kt );
      if( det >= 1 && mi->state_num > 0 )
        lines[n++] = zSTRING( T( mi->state != 0 ? TX_HOV_SW_ON : TX_HOV_SW_OFF ) );
      tint = n;
    }
    else if( const TExit* ex = ExitOf( vob ) ) {
      title = ColExit();
      lines[n++] = zSTRING( T( TX_HOV_EXIT ) );
      if( ex->dest[0] )
        lines[n++] = zSTRING( T( TX_HOV_EXIT_TO ) ) + " " + zSTRING( ex->dest );
      tint = n;
    }
    else if( const int wl = WtpHoverLines( vob, lines + n, HOV_MAX - n ) ) {
      title = ColTeleport();
      n += wl;
      tint = n;
    }
    else if( PlaceKindOf( vob ) >= 0 ) {
      const int kind = PlaceKindOf( vob );
      title = AtlasColor( kind );
      const char* const kt = PlaceKindName( kind );
      zSTRING nm = ( (oCMOB*)vob )->GetName();
      if( MobNameIsKey( nm, ( (oCMOB*)vob )->name ) ) nm = zSTRING( "" );
      if( const char* tn = NameTxtMob( ( (oCMOB*)vob )->name.ToChar() ) ) nm = zSTRING( tn );
      lines[n++] = ( nm.Length() > 0 ) ? CapFirst( nm ) : zSTRING( kt );
      if( nm.Length() > 0 && !HovKindInName( nm.ToChar(), kt ) )
        lines[n++] = zSTRING( kt );
      tint = n;
    }
    else {
      oCItem* const item = (oCItem*)vob;
      const int icon = ItemIconOf( item );
      title = ( icon == ICON_ITEM_OTHER ) ? ColItem() : ItemIconColor( icon );
      const bool hasName = item->name.Length() > 0;
      const bool hasDesc = item->description.Length() > 0;
      lines[n++] = hasName ? CapFirst( item->name ) : hasDesc ? CapFirst( item->description ) : zSTRING( "?" );
      if( const char* tn = NameTxtInst( item->instanz ) ) lines[0] = zSTRING( tn );
      if( det >= 2 ) {
        const char* cat = HovItemCat( item->mainflag );
        if( cat && !HovKindInName( lines[0].ToChar(), cat ) ) lines[n++] = zSTRING( cat );
      }
      if( det >= 1 ) {
      if( hasName && hasDesc && !( item->description == item->name )
          && !HovSameText( item->description.ToChar(), item->name.ToChar() ) )
        lines[n++] = CapFirst( item->description );
      }
      tint = n;
      if( det >= 1 ) {
      zSTRING l3;
      if( item->value > 0 ) l3 = zSTRING( T( TX_HOV_VALUE ) ) + ": " + zSTRING( item->value );
      if( item->amount > 1 ) l3 = l3 + zSTRING( ( l3.Length() > 0 ) ? "   x" : "x" ) + zSTRING( item->amount );
      if( l3.Length() > 0 )
        lines[n++] = l3;
      }
      if( det >= 2 ) {
        HtxEnsure();
        int  scrKind[ITM_TEXT_MAX];
        bool scrDmg = false, scrPe = false, scrPp = false, scrPf = false, scrPm = false;
        for( int i = 0; i < ITM_TEXT_MAX; i++ ) {
          scrKind[i] = HtxKindOf( item->text[i] );
          if( scrKind[i] == HTX_DMG && item->count[i] > 0 ) scrDmg = true;
          if( scrKind[i] == HTX_PE ) scrPe = true;
          if( scrKind[i] == HTX_PP ) scrPp = true;
          if( scrKind[i] == HTX_PF ) scrPf = true;
          if( scrKind[i] == HTX_PM ) scrPm = true;
        }
        int dmg = item->damageTotal;
        if( dmg <= 0 ) {
          dmg = 0;
          for( int i = 0; i < oEDamageIndex_MAX; i++ )
            if( item->damage[i] > 0 ) dmg += item->damage[i];
        }
        zSTRING ld;
        if( dmg > 0 && !scrDmg ) ld = zSTRING( T( TX_HOV_DAMAGE ) ) + " " + zSTRING( dmg );
        if( ( item->mainflag & ITM_CAT_NF ) && item->range > 0 )
          ld = ld + zSTRING( ( ld.Length() > 0 ) ? "   " : "" ) + zSTRING( T( TX_HOV_RANGE ) ) + " " + zSTRING( item->range );
        if( ld.Length() > 0 && n < HOV_MAX )
          lines[n++] = ld;
        zSTRING lp;
        int prot[oEDamageIndex_MAX];
        memcpy( prot, item->protection, (size_t)( oEDamageIndex_MAX ) * sizeof( prot[0] ) );
        if( scrPe ) prot[oEDamageIndex_Edge]  = 0;
        if( scrPp ) prot[oEDamageIndex_Point] = 0;
        if( scrPf ) prot[oEDamageIndex_Fire]  = 0;
        if( scrPm ) prot[oEDamageIndex_Magic] = 0;
        if( HovProtLine( prot, lp ) && n < HOV_MAX )
          lines[n++] = lp;
        zSTRING lr;
        for( int i = 0; i < ITM_COND_MAX; i++ ) {
          const char* an = HovAtrName( item->cond_atr[i] );
          if( !an || item->cond_value[i] <= 0 ) continue;
          const int hk = HtxKindOfAtr( item->cond_atr[i] );
          bool inScript = false;
          for( int k = 0; k < ITM_TEXT_MAX && !inScript; k++ )
            inScript = scrKind[k] == hk && item->count[k] == item->cond_value[i];
          if( inScript ) continue;
          lr = lr + zSTRING( ( lr.Length() > 0 ) ? ", " : "" ) + zSTRING( an ) + " " + zSTRING( item->cond_value[i] );
        }
        if( lr.Length() > 0 && n < HOV_MAX )
          lines[n++] = zSTRING( T( TX_HOV_NEEDS ) ) + ": " + lr;
        for( int i = 0; i < ITM_TEXT_MAX && n < HOV_MAX; i++ ) {
          if( item->text[i].Length() <= 0 ) continue;
          if( item->value > 0 && item->count[i] == item->value ) {
            const char* tx = item->text[i].ToChar();
            if( i == ITM_TEXT_MAX - 1
                || StrHasAnyUp( tx, "wart|value|wert|price|cena|gold|zlot" ) )
              continue;
          }
          zSTRING lt = CapFirst( item->text[i] );
          if( item->count[i] != 0 ) lt = lt + " " + zSTRING( item->count[i] );
          bool dup = false;
          for( int k = 0; k < n && !dup; k++ )
            dup = HovSameText( lines[k].ToChar(), lt.ToChar() );
          if( dup ) continue;
          lines[n++] = lt;
        }
      }
    }
    const float c = CursorPx();
    HoverBubble( g_curX + c * 0.9f, g_curY + c * 0.3f, lines, n, title, tint );
  }
  static const DWORD ITEM_CACHE_MS = 700;
  static oCItem** g_itemCache     = 0;
  static int      g_itemCount     = 0;
  static DWORD    g_itemNext      = 0;
  static oCWorld* g_itemWorld     = 0;
  static unsigned g_itemGen       = 1;
  static int      g_itemLastShown = 0;
  static int   g_search        = 0;
  static char  g_searchBuf[48] = "";
  static int   g_searchLen     = 0;
  static char  g_searchNorm[48] = "";
  static bool* g_itemMatch     = 0;
  static int   g_itemMatchCap  = 0;
  static int   g_itemMatchN    = 0;
  static bool* g_chestMatch    = 0;
  static int   g_chestMatchCap = 0;
  static int   g_chestMatchN   = 0;
  static int   g_searchFound   = 0;
  static int   g_searchInChest = 0;
  static int   g_searchNpcFound = 0;
  static int   g_ringsNow      = 0;
  static int   g_ringsLast     = 0;
  enum {
    SCAT_ALL = 0, SCAT_MELEE, SCAT_RANGED, SCAT_ARMOR, SCAT_POTION, SCAT_FOOD,
    SCAT_MAGIC, SCAT_DOCS, SCAT_GOLD, SCAT_OTHER,
    SCAT_PLACES,
    SCAT_NPC_ALL, SCAT_HUMAN, SCAT_MONSTER, SCAT_FOE, SCAT_FRIEND,
    SCAT_TRADER,
    SCAT_COUNT
  };
  static int g_searchCat = SCAT_ALL;
  inline bool SearchCatIsNpc( int c ) {
    return c >= SCAT_NPC_ALL && c < SCAT_COUNT;
  }
  inline const char* SearchCatName( int c ) {
    switch( c ) {
      case SCAT_MELEE:  return T( TX_SCAT_MELEE );
      case SCAT_RANGED: return T( TX_SCAT_RANGED );
      case SCAT_ARMOR:  return T( TX_SCAT_ARMOR );
      case SCAT_POTION: return T( TX_SCAT_POTION );
      case SCAT_FOOD:   return T( TX_SCAT_FOOD );
      case SCAT_MAGIC:  return T( TX_SCAT_MAGIC );
      case SCAT_DOCS:   return T( TX_SCAT_DOCS );
      case SCAT_GOLD:   return T( TX_SCAT_GOLD );
      case SCAT_OTHER:  return T( TX_SCAT_OTHER );
      case SCAT_NPC_ALL: return T( TX_SCAT_NPC_ALL );
      case SCAT_HUMAN:   return T( TX_SCAT_HUMAN );
      case SCAT_MONSTER: return T( TX_SCAT_MONSTER );
      case SCAT_FOE:     return T( TX_SCAT_FOE );
      case SCAT_FRIEND:  return T( TX_SCAT_FRIEND );
      case SCAT_TRADER:  return T( TX_SCAT_TRADER );
      case SCAT_PLACES:  return T( TX_SCAT_PLACES );
    }
    return T( TX_SCAT_ALL );
  }
  inline bool SearchIsGold( oCItem* it ) {
    const int mf = it->mainflag;
    if( mf & ~ITM_CAT_NONE ) return false;
    zSTRING n = it->GetInstanceName();
    n.Upper();
    const char* s = n.ToChar();
    if( !s ) return false;
    if( ENGINE <= Engine_G1A && strncmp( s, "ITMINUGGET", 10 ) == 0 ) {
      const char c = s[10];
      return !( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) );
    }
    if( strncmp( s, "ITMI_GOLD", 9 ) != 0 ) return false;
    const char c = s[9];
    return !( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) );
  }
  inline int SearchCatOf( oCItem* it ) {
    switch( ItemKindTxt( it ) ) {
      case IK_MELEE_1H: case IK_MELEE_2H: return SCAT_MELEE;
      case IK_RANGED: return SCAT_RANGED;
      case IK_ARMOR:  return SCAT_ARMOR;
      case IK_POTION: return SCAT_POTION;
      case IK_FOOD:   return SCAT_FOOD;
      case IK_MAGIC:  return SCAT_MAGIC;
      case IK_DOCS:   return SCAT_DOCS;
      case IK_GOLD:   return SCAT_GOLD;
      case IK_OTHER:  return SCAT_OTHER;
      default: break;
    }
    const int mf = it->mainflag, fl = it->flags;
    if( mf & ITM_CAT_NF )                 return SCAT_MELEE;
    if( mf & ( ITM_CAT_FF | ITM_CAT_MUN ) ) return SCAT_RANGED;
    if( mf & ITM_CAT_ARMOR )              return SCAT_ARMOR;
    if( mf & ITM_CAT_POTION )             return SCAT_POTION;
    if( mf & ITM_CAT_FOOD )               return SCAT_FOOD;
    if( ( mf & ( ITM_CAT_RUNE | ITM_CAT_MAGIC ) ) || ( fl & ( ITM_FLAG_AMULET | ITM_FLAG_RING ) ) ) return SCAT_MAGIC;
    if( mf & ITM_CAT_DOCS )               return SCAT_DOCS;
    if( SearchIsGold( it ) )              return SCAT_GOLD;
    return SCAT_OTHER;
  }
  inline bool SearchCatHit( oCItem* it ) {
    if( g_searchCat == SCAT_ALL ) return true;
    if( SearchCatIsNpc( g_searchCat ) ) return false;
    return it && SearchCatOf( it ) == g_searchCat;
  }
  inline bool SearchCatHitNpc( oCNpc* npc, int cat = g_searchCat ) {
    switch( cat ) {
      case SCAT_ALL:
      case SCAT_NPC_ALL: return true;
      case SCAT_HUMAN:   return !npc->IsMonster() && !npc->IsOrc();
      case SCAT_MONSTER: return npc->IsMonster() != 0 || npc->IsOrc() != 0;
      case SCAT_FOE:     return NpcGroup( npc ) == 0;
      case SCAT_FRIEND:  return NpcGroup( npc ) == 2;
      case SCAT_TRADER:  return TraderIs( npc );
    }
    return false;
  }
  inline bool SearchHasFilter() {
    return g_searchNorm[0] != 0 || g_searchCat != SCAT_ALL;
  }
  inline bool SearchNpcFilter() {
    if( SearchCatIsNpc( g_searchCat ) ) return true;
    if( g_searchInv && g_searchCat != SCAT_ALL && g_searchCat != SCAT_PLACES ) return true;
    return g_searchCat == SCAT_ALL && g_searchNorm[0] != 0;
  }
  inline void SearchMatchItems();
  inline void SearchMatchChests();
  inline void SearchSuggestBuild();
  inline void SearchMatchPlaces();
  inline void SearchMatchExits();
  inline void SearchSuggestExits();
  inline int  SearchExitFound();
  inline bool SearchMobHit( zCVob* v, int kind );
  inline void SearchSuggestPlaces();
  static int g_searchPlaceFound = 0;
  enum { SUGG_SCAN = 64, SUGG_SHOW = 8, SUGG_LEN = 48 };
  static char  g_suggName[SUGG_SCAN][SUGG_LEN];
  static char  g_suggNorm[SUGG_SCAN][SUGG_LEN];
  static int   g_suggCnt[SUGG_SCAN];
  static int   g_suggN      = 0;
  static int   g_suggSel    = -1;
  static bool  g_suggBatch  = false;
  static float g_suggX0 = 0.0f, g_suggY0 = 0.0f, g_suggX1 = 0.0f, g_suggRowH = 0.0f;
  static int   g_suggFirst = 0, g_suggShown = 0;
  inline void SearchRing( float px, float py, float s );
  inline void SearchDimMark( float px, float py, float s );
  inline bool SearchActive() {
    return g_search != 0 && SearchHasFilter();
  }
  inline bool SearchFilterOn() {
    return SearchActive() && ( g_full || g_search == 2 );
  }
  inline bool SearchTyping() {
    return g_search == 1 && g_edit < 0;
  }
}
