#pragma once
namespace MiniMap {
  static int  g_ilCat        = SCAT_ALL;
  enum { IL_MAX = 512, IL_LEN = 48 };
  static char g_ilName[IL_MAX][IL_LEN];
  static char g_ilNorm[IL_MAX][IL_LEN];
  static int  g_ilCnt[IL_MAX];
  static int  g_ilN          = 0;
  static int  g_ilTotal      = 0;
  static int  g_ilSel        = 0;
  static int  g_ilTop        = 0;
  static int  g_ilRows       = 0;
  static unsigned g_ilSrcGen = 0;
  static int  g_ilSrcCat     = -1;
  static DWORD g_ilSrcNpc    = 0;
  static float g_ilX0 = 0.0f, g_ilY0 = 0.0f, g_ilX1 = 0.0f, g_ilY1 = 0.0f;
  static float g_ilRowY0 = 0.0f, g_ilRowH = 0.0f;
  inline bool IlCatOk( int c ) {
    return c == SCAT_ALL || ( c > SCAT_ALL && c < SCAT_PLACES ) || SearchCatIsNpc( c );
  }
  inline void IlCatStep( int dir ) {
    int c = g_ilCat;
    for( int g = 0; g < SCAT_COUNT; g++ ) {
      c = ( c + dir + SCAT_COUNT ) % SCAT_COUNT;
      if( IlCatOk( c ) ) break;
    }
    g_ilCat = c;
    g_ilSel = 0;
    g_ilTop = 0;
  }
  inline const char* IlItemName( oCItem* it ) {
    const char* s = it->name.ToChar();
    if( s && *s ) return s;
    s = it->description.ToChar();
    return ( s && *s ) ? s : 0;
  }
  inline void IlAdd( const char* h, int am ) {
    while( *h == ' ' )
      h++;
    if( !*h ) return;
    g_ilTotal += am;
    char norm[IL_LEN];
    SearchNormalize( h, norm, IL_LEN );
    int k = 0;
    while( k < g_ilN && strcmp( g_ilNorm[k], norm ) != 0 )
      k++;
    if( k < g_ilN ) {
      g_ilCnt[k] += am;
      return;
    }
    if( g_ilN >= IL_MAX ) return;
    StrCopy( g_ilName[g_ilN], h, IL_LEN );
    StrCopy( g_ilNorm[g_ilN], norm, IL_LEN );
    g_ilCnt[g_ilN] = am;
    g_ilN++;
  }
  inline void IlBuild() {
    char selNorm[IL_LEN];
    selNorm[0] = 0;
    if( g_ilSrcCat == g_ilCat && g_ilSel >= 0 && g_ilSel < g_ilN )
      StrCopy( selNorm, g_ilNorm[g_ilSel], IL_LEN );
    g_ilN     = 0;
    g_ilTotal = 0;
    if( SearchCatIsNpc( g_ilCat ) ) {
      for( int i = 0; i < g_npcCount; i++ ) {
        oCNpc* const npc = g_npcCache ? g_npcCache[i] : 0;
        if( !npc || npc == player || npc->attribute[NPC_ATR_HITPOINTS] <= 0 || NpcFlagAt( i, NPCF_GONE ) ) continue;
        if( !SearchCatHitNpc( npc, g_ilCat ) ) continue;
        const char* const h = npc->name[0].ToChar();
        if( h && *h )
          IlAdd( h, 1 );
      }
    }
    for( int i = 0; i < g_itemCount && !SearchCatIsNpc( g_ilCat ); i++ ) {
      oCItem* const it = g_itemCache ? g_itemCache[i] : 0;
      if( !it || it->homeWorld == 0 ) continue;
      if( g_ilCat != SCAT_ALL && SearchCatOf( it ) != g_ilCat ) continue;
      const char* h = IlItemName( it );
      if( !h ) continue;
      IlAdd( h, ( it->amount > 0 ) ? it->amount : 1 );
    }
    SortNamedCounts( g_ilN, IL_LEN, &g_ilName[0][0], &g_ilNorm[0][0], g_ilCnt );
    if( selNorm[0] )
      for( int i = 0; i < g_ilN; i++ )
        if( strcmp( g_ilNorm[i], selNorm ) == 0 ) {
          g_ilSel = i;
          break;
        }
    if( g_ilSel >= g_ilN ) g_ilSel = g_ilN - 1;
    if( g_ilSel < 0 )      g_ilSel = 0;
    g_ilSrcGen = g_itemGen;
    g_ilSrcCat = g_ilCat;
    g_ilSrcNpc = g_npcNext;
  }
  inline bool IlStale() {
    return g_ilSrcGen != g_itemGen || g_ilSrcCat != g_ilCat
        || ( SearchCatIsNpc( g_ilCat ) && g_ilSrcNpc != g_npcNext );
  }
  inline void IlApply( int row ) {
    if( row < 0 || row >= g_ilN ) return;
    g_ilSel = row;
    StrCopy( g_searchBuf, g_ilName[row], 41 );
    g_searchLen = (int)strlen( g_searchBuf );
    g_searchCat = g_ilCat;
    SearchTextChanged();
    g_search = SearchHasFilter() ? 2 : 0;
    MM_LOG( zSTRING( "item list: show '" ) + zSTRING( g_searchBuf ) + "', found " + zSTRING( g_searchFound ) );
  }
  inline bool IlActive() {
    return g_itemList && g_full && g_search != 1 && g_edit < 0
        && ( g_keyItemList <= 0 || g_ilShown );
  }
  inline void IlScrollTo( int sel ) {
    if( g_ilRows <= 0 ) return;
    if( sel < g_ilTop )                 g_ilTop = sel;
    if( sel >= g_ilTop + g_ilRows )     g_ilTop = sel - g_ilRows + 1;
    if( g_ilTop > g_ilN - g_ilRows )    g_ilTop = g_ilN - g_ilRows;
    if( g_ilTop < 0 )                   g_ilTop = 0;
  }
  inline int IlRepeatStep( int slot ) {
    if( slot < 0 || slot >= KEY_SLOTS || !g_keyHeldSince[slot] ) return 0;
    const DWORD held = GetTickCount() - g_keyHeldSince[slot];
    if( held < 450 )  return 0;
    if( held < 1000 ) return 1;
    if( held < 2000 ) return 3;
    return 10;
  }
  inline void ReadItemList() {
    const bool a  = PreKeyEdge( 40, KEY_A, true );
    const bool d  = PreKeyEdge( 41, KEY_D, true );
    const bool w  = PreKeyEdge( 42, KEY_W, true );
    const bool s  = PreKeyEdge( 43, KEY_S, true );
    if( !IlActive() || zCMenu::GetActive() || ConsoleOpen() ) return;
    if( a != d )
      IlCatStep( d ? 1 : -1 );
    if( IlStale() )
      IlBuild();
    if( w != s && g_ilN > 0 ) {
      const int slot = s ? 43 : 42;
      const int step = IlRepeatStep( slot );
      const bool fresh = ( step == 0 );
      int n = g_ilSel + ( s ? 1 : -1 ) * ( fresh ? 1 : step );
      if( n < 0 ) n = ( fresh && g_ilSel == 0 ) ? g_ilN - 1 : 0;
      if( n >= g_ilN ) n = ( fresh && g_ilSel == g_ilN - 1 ) ? 0 : g_ilN - 1;
      g_ilSel = n;
      IlScrollTo( g_ilSel );
      IlApply( g_ilSel );
    }
  }
  inline bool ItemListMouse( bool l, bool wasL ) {
    if( !IlActive() || g_ilRows <= 0 ) return false;
    if( !( g_curX >= g_ilX0 && g_curX < g_ilX1 && g_curY >= g_ilY0 && g_curY < g_ilY1 ) ) return false;
    if( l && !wasL && g_curY >= g_ilRowY0 && g_ilRowH > 0.0f ) {
      const int row = g_ilTop + (int)( ( g_curY - g_ilRowY0 ) / g_ilRowH );
      if( row >= 0 && row < g_ilN && row < g_ilTop + g_ilRows )
        IlApply( row );
    }
    return true;
  }
  inline bool ItemListWheel( int steps ) {
    if( !IlActive() || g_ilRows <= 0 || steps == 0 ) return false;
    if( !( g_curX >= g_ilX0 && g_curX < g_ilX1 && g_curY >= g_ilY0 && g_curY < g_ilY1 ) ) return false;
    g_ilTop -= steps * 3;
    if( g_ilTop > g_ilN - g_ilRows ) g_ilTop = g_ilN - g_ilRows;
    if( g_ilTop < 0 )                g_ilTop = 0;
    return true;
  }
  inline void DrawItemList() {
    g_ilRows = 0;
    g_ilX1 = g_ilX0;
    if( !IlReserve() || !screen || !ogame ) return;
    {
      oCWorld* const world = ogame->GetGameWorld();
      if( !world ) return;
      if( g_itemCache && world != g_itemWorld )
        ItemCacheFree( false );
      const DWORD now = GetTickCount();
      if( !g_itemWorld || (long)( now - g_itemNext ) >= 0 ) {
        g_itemNext = now + ITEM_CACHE_MS;
        ItemCacheRefresh( world );
        if( !g_itemWorld ) g_itemWorld = world;
      }
      if( SearchCatIsNpc( g_ilCat ) ) {
        if( g_npcCache && world != g_npcWorld )
          NpcCacheFree( false );
        if( !g_npcCache || (long)( now - g_npcNext ) >= 0 ) {
          g_npcNext = now + NPC_CACHE_MS;
          NpcCacheRefresh( world );
        }
      }
    }
    if( IlStale() )
      IlBuild();
    int x, y, w, h;
    FullRect( x, y, w, h );
    if( g_ilPanW <= 0.0f ) return;
    const float lh  = LineHPx();
    const float pad = 4.0f;
    const float rowH = lh + 1.0f;
    const float pw = g_ilPanW;
    const float px = g_ilPanX;
    const float py = g_ilPanY;
    const float ph = g_ilPanH;
    if( ph < lh * 6.0f ) return;
    Pane( px, py, pw, ph );
    char line[96];
    snprintf( line, sizeof( line ), "< %s >", SearchCatName( g_ilCat ) );
    float tw = TextWidthPx( line );
    MarkText( px + ( pw - tw ) * 0.5f, py + pad, line, ColGold() );
    snprintf( line, sizeof( line ), "%s%d, %s%d", T( TX_IL_KINDS ), g_ilN, T( TX_IL_PIECES ), g_ilTotal );
    MarkText( px + pad * 2.0f, py + pad + lh, line, ColDim() );
    const float listY = py + pad + lh * 2.2f;
    const float footH = lh + pad;
    int rows = (int)( ( py + ph - footH - listY ) / rowH );
    if( rows < 1 ) return;
    g_ilRows = rows;
    IlScrollTo( g_ilSel );
    const float maxTw = pw - pad * 4.0f;
    for( int r = 0; r < rows; r++ ) {
      const int i = g_ilTop + r;
      if( i >= g_ilN ) break;
      char nm[IL_LEN + 4];
      StrCopy( nm, g_ilName[i], IL_LEN );
      char cnt[16];
      cnt[0] = 0;
      if( g_ilCnt[i] > 1 ) snprintf( cnt, sizeof( cnt ), " x%d", g_ilCnt[i] );
      const float cw = cnt[0] ? TextWidthPx( cnt ) : 0.0f;
      int len = (int)strlen( nm );
      while( len > 3 && TextWidthPx( nm ) + cw > maxTw ) {
        nm[--len] = 0;
        nm[len - 1] = '.'; nm[len - 2] = '.';
      }
      snprintf( line, sizeof( line ), "%s%s", nm, cnt );
      const float ry = listY + (float)r * rowH;
      const bool sel = ( i == g_ilSel );
      if( sel )
        TexRect( px + 2.0f, ry - 1.0f, pw - 4.0f, rowH, ColGold() );
      MarkText( px + pad * 2.0f, ry, line, sel ? ColMarkPane() : ColMarkLight() );
    }
    if( g_ilN == 0 )
      MarkText( px + pad * 2.0f, listY, T( TX_IL_EMPTY ), ColDim() );
    if( g_ilN > rows ) {
      const float sh  = (float)rows * rowH;
      const float bh  = sh * (float)rows / (float)g_ilN;
      const float by  = listY + sh * (float)g_ilTop / (float)g_ilN;
      TexRect( px + pw - 5.0f, listY, 3.0f, sh, ColDim() );
      TexRect( px + pw - 5.0f, by, 3.0f, ( bh < 6.0f ? 6.0f : bh ), ColGold() );
    }
    const char* const tip = T( TX_IL_TIP );
    tw = TextWidthPx( tip );
    MarkText( px + ( pw - tw ) * 0.5f, py + ph - footH, tip, ColDim() );
    g_ilX0 = px; g_ilX1 = px + pw; g_ilY0 = py; g_ilY1 = py + ph;
    g_ilRowY0 = listY; g_ilRowH = rowH;
  }
}
