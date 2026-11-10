#pragma once
namespace MiniMap {
  enum { TP_MAX = 96, TP_NAME = 48 };
  struct TTeleport {
    float x, z;
    char  wp[40];
    char  name[TP_NAME];
    int   spell;
    bool  rune;
  };
  static TTeleport g_tp[TP_MAX];
  static int       g_tpCount   = 0;
  static DWORD     g_tpNext    = 0;
  static oCWorld*  g_tpWorld   = 0;
  static const DWORD TP_CACHE_MS = 3000;
  static const int ITEM_MULTI_FLAG = ITM_FLAG_MULTI;
  enum { TP_SPELLS = 256 };
  static char  g_tpSpellWp[TP_SPELLS][40];
  static bool  g_tpSpellKnown[TP_SPELLS];
  static void* g_tpParserSeen = 0;
  static char  g_tpSpellWpAlt[TP_SPELLS][2][40];
  enum { TP_GATES = 128, TP_GATE_NONZERO = 0x7FFFFFFF };
  struct TTpGate {
    int  var;
    int  val;
    char wp[40];
    char name[TP_NAME];
    void* posWorld;
    int   posOk;
    float px, pz;
  };
  static TTpGate g_tpGate[TP_GATES];
  static int     g_tpGateN = 0;
  enum { TPK_EQ = zPAR_OP_EQUAL, TPK_GE = zPAR_OP_HIGHER_EQ, TPK_RET = zPAR_TOK_RET,
         TPK_CALL = zPAR_TOK_CALL, TPK_CALLEXT = zPAR_TOK_CALLEXTERN, TPK_PUSHINT = zPAR_TOK_PUSHINT,
         TPK_PUSHVAR = zPAR_TOK_PUSHVAR, TPK_PUSHINST = zPAR_TOK_PUSHINST, TPK_JUMP = zPAR_TOK_JUMP,
         TPK_JUMPF = zPAR_TOK_JUMPF, TPK_SETINST = zPAR_TOK_SETINSTANCE,
         TPK_ARRAY = zPAR_TOK_PUSHVAR + zPAR_TOK_FLAGARRAY };
  inline int TpTokLen( unsigned char t ) { return PlaceLearn::PlTokLen( t ); }
  inline bool TpSymInt( zCPar_Symbol* s, int& out ) {
    if( !s || s->type != zPAR_TYPE_INT || !( s->flags & zPAR_FLAG_CONST ) || s->ele != 1 ) return false;
    out = s->single_intdata;
    return true;
  }
  enum { TP_WRAPS = 8 };
  static int g_tpWrap[TP_WRAPS];
  static int g_tpWrapN = 0;
  inline bool TpIsWrap( int addr ) {
    for( int i = 0; i < g_tpWrapN; i++ )
      if( g_tpWrap[i] == addr ) return true;
    return false;
  }
  inline int TpBuildWrapsCore( int teleIdx ) {
    __try {
      g_tpWrapN = 0;
      unsigned char* const base = parser->stack.stack;
      const int size = parser->stack.stacksize;
      if( !base || size <= 0 || teleIdx < 0 ) return 0;
      const int n = parser->symtab.table.GetNum();
      for( int k = 0; k + 1 < n && g_tpWrapN < TP_WRAPS; k++ ) {
        zCPar_Symbol* const f = parser->symtab.table[k];
        if( !f || f->type != zPAR_TYPE_FUNC || ( f->flags & zPAR_FLAG_EXTERNAL )
            || !( f->flags & zPAR_FLAG_CONST ) || f->ele < 1 )
          continue;
        zCPar_Symbol* const p = parser->GetSymbol( k + 1 );
        if( !p || p->type != zPAR_TYPE_STRING || ( p->flags & zPAR_FLAG_CONST ) ) continue;
        int i = f->single_intdata;
        if( i < 0 || i >= size ) continue;
        bool lastP = false;
        int jumpMax = 0;
        for( int guard = 0; i < size && guard < 2000; guard++ ) {
          const unsigned char tk = base[i];
          if( tk == TPK_RET && jumpMax <= i ) break;
          const int len = TpTokLen( tk );
          if( i + len > size ) break;
          if( len == 5 ) {
            int arg;
            memcpy( &arg, base + i + 1, 4 );
            if( ( tk == TPK_JUMP || tk == TPK_JUMPF ) && arg > jumpMax && arg < size ) jumpMax = arg;
            if( tk == TPK_PUSHVAR )
              lastP = ( arg == k + 1 );
            else if( tk == TPK_CALLEXT && arg == teleIdx && lastP ) {
              g_tpWrap[g_tpWrapN++] = f->single_intdata;
              break;
            }
          }
          i += len;
        }
      }
      return g_tpWrapN;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      g_tpWrapN = 0;
      return -1;
    }
  }
  inline bool TpWpOfFunc( zCPar_Symbol* f, int teleIdx, char* out, int max,
                          char ( *alt )[40] = 0 ) {
    out[0] = 0;
    if( alt ) { alt[0][0] = 0; alt[1][0] = 0; }
    if( !f || f->type != zPAR_TYPE_FUNC || ( f->flags & zPAR_FLAG_EXTERNAL ) ) return false;
    unsigned char* const base = parser->stack.stack;
    const int size = parser->stack.stacksize;
    int i = f->single_intdata;
    if( !base || i < 0 || i >= size ) return false;
    const char* last = 0;
    int found = 0;
    int jumpMax = 0;
    for( int guard = 0; i < size && guard < 4096; guard++ ) {
      const unsigned char t = base[i];
      if( t == TPK_RET && jumpMax <= i ) break;
      const int len = TpTokLen( t );
      if( i + len > size ) break;
      if( len == 5 ) {
        int arg;
        memcpy( &arg, base + i + 1, 4 );
        if( ( t == TPK_JUMP || t == TPK_JUMPF ) && arg > jumpMax && arg < size ) jumpMax = arg;
        if( t == TPK_PUSHVAR ) {
          zCPar_Symbol* const s = parser->GetSymbol( arg );
          last = ( s && s->type == zPAR_TYPE_STRING && s->ele == 1 && s->stringdata )
               ? s->stringdata->ToChar() : 0;
        }
        else if( ( t == TPK_CALLEXT && arg == teleIdx ) || ( t == TPK_CALL && TpIsWrap( arg ) ) ) {
          if( last && *last ) {
            if( found == 0 )
              StrCopy( out, last, max );
            else if( alt && found <= 2 && strcmp( out, last ) != 0 )
              StrCopy( alt[found - 1], last, 40 );
            found++;
          }
          if( !alt || found >= 3 ) return found > 0;
        }
      }
      i += len;
    }
    return found > 0;
  }
  inline int TpIndex( const char* name );
  inline bool TpVarInt( int idx, int& out ) {
    zCPar_Symbol* const s = parser->GetSymbol( idx );
    if( !s || s->type != zPAR_TYPE_INT || ( s->flags & zPAR_FLAG_CONST ) || s->ele != 1 ) return false;
    out = s->single_intdata;
    return true;
  }
  inline int TpBuildGatesCore() {
    __try {
      g_tpGateN = 0;
      const int teleIdx = TpIndex( "AI_TELEPORT" );
      const int choiceIdx = TpIndex( "INFO_ADDCHOICE" );
      if( teleIdx < 0 || choiceIdx < 0 ) return 0;
      unsigned char* const base = parser->stack.stack;
      const int size = parser->stack.stacksize;
      if( !base || size <= 0 ) return 0;
      const int n = parser->symtab.table.GetNum();
      for( int k = 0; k < n; k++ ) {
        zCPar_Symbol* const f = parser->symtab.table[k];
        if( !f || f->type != zPAR_TYPE_FUNC || ( f->flags & zPAR_FLAG_EXTERNAL )
            || !( f->flags & zPAR_FLAG_CONST ) )
          continue;
        int i = f->single_intdata;
        if( i < 0 || i >= size ) continue;
        int hOp[3] = { -1, -1, -1 }, hArg[3] = { 0, 0, 0 };
        int gVar[8], gVal[8], gEnd[8], gN = 0;
        const char* text = 0;
        int lastInt = -1;
        TTpGate found[TP_GATES];
        int fn = 0;
        int jumpMax = 0;
        for( int guard = 0; i < size && guard < 20000; guard++ ) {
          const unsigned char tk = base[i];
          if( tk == TPK_RET && jumpMax <= i ) break;
          const int len = TpTokLen( tk );
          if( i + len > size ) break;
          int arg = 0;
          if( len == 5 )
            memcpy( &arg, base + i + 1, 4 );
          if( ( tk == TPK_JUMP || tk == TPK_JUMPF ) && arg > jumpMax && arg < size ) jumpMax = arg;
          while( gN > 0 && i >= gEnd[gN - 1] )
            gN--;
          if( tk == TPK_JUMPF && gN < 8 ) {
            int var = -1, val = TP_GATE_NONZERO;
            if( hOp[2] == TPK_EQ && ( hOp[1] == TPK_PUSHVAR || hOp[1] == TPK_PUSHINT )
                && ( hOp[0] == TPK_PUSHVAR || hOp[0] == TPK_PUSHINT ) ) {
              int v, c;
              for( int a = 0; a < 2 && var < 0; a++ ) {
                const int o1 = a ? 0 : 1, o2 = a ? 1 : 0;
                if( hOp[o1] != TPK_PUSHVAR || !TpVarInt( hArg[o1], v ) ) continue;
                if( hOp[o2] == TPK_PUSHINT ) { var = hArg[o1]; val = hArg[o2]; }
                else if( TpSymInt( parser->GetSymbol( hArg[o2] ), c ) ) { var = hArg[o1]; val = c; }
              }
            }
            else if( hOp[2] == TPK_PUSHVAR ) {
              int v;
              if( TpVarInt( hArg[2], v ) ) var = hArg[2];
            }
            if( var >= 0 && arg > i ) {
              gVar[gN] = var; gVal[gN] = val; gEnd[gN] = arg; gN++;
            }
          }
          if( tk == TPK_PUSHVAR ) {
            zCPar_Symbol* const s = parser->GetSymbol( arg );
            if( s && s->type == zPAR_TYPE_STRING && s->ele == 1 && s->stringdata ) text = s->stringdata->ToChar();
          }
          else if( tk == TPK_PUSHINT )
            lastInt = arg;
          else if( tk == TPK_CALLEXT && arg == choiceIdx && lastInt >= 0 && gN > 0 && fn < TP_GATES ) {
            zCPar_Symbol* const tf = parser->GetSymbol( lastInt );
            char wp[40];
            if( tf && tf->type == zPAR_TYPE_FUNC && TpWpOfFunc( tf, teleIdx, wp, 40 ) ) {
              TTpGate& g = found[fn++];
              g.var = gVar[gN - 1];
              g.val = gVal[gN - 1];
              StrCopy( g.wp, wp, 40 );
              StrCopy( g.name, ( text && *text ) ? text : wp, TP_NAME );
              int e = (int)strlen( g.name );
              while( e > 1 && ( g.name[e - 1] == '.' || g.name[e - 1] == ' ' ) )
                g.name[--e] = 0;
            }
          }
          hOp[0] = hOp[1]; hArg[0] = hArg[1];
          hOp[1] = hOp[2]; hArg[1] = hArg[2];
          hOp[2] = tk;     hArg[2] = arg;
          i += len;
        }
        if( fn >= 3 )
          for( int q = 0; q < fn && g_tpGateN < TP_GATES; q++ ) {
            g_tpGate[g_tpGateN] = found[q];
            g_tpGate[g_tpGateN].posWorld = 0;
            g_tpGate[g_tpGateN].posOk = 0;
            g_tpGateN++;
          }
      }
      return g_tpGateN;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      g_tpGateN = 0;
      return -1;
    }
  }
  inline zCPar_Symbol* TpSym( const char* name ) {
    return parser->GetSymbol( zSTRING( name ) );
  }
  inline int TpIndex( const char* name ) {
    return parser->GetIndex( zSTRING( name ) );
  }
  inline zCWaypoint* TpWaypoint( oCWorld* world, const char* wp ) {
    return world->wayNet->GetWaypoint( zSTRING( wp ) );
  }
  inline bool TpTargetPos( oCWorld* world, const char* wp, float& x, float& z ) {
    zCWaypoint* const w = TpWaypoint( world, wp );
    if( w ) { x = w->pos[0]; z = w->pos[2]; return true; }
    zSTRING nm( wp );
    nm.Upper();
    zCVob* const v = world->SearchVobByName( nm );
    if( !v || v->homeWorld != (zCWorld*)world ) return false;
    const zVEC3 p = v->GetPositionWorld();
    x = p[0]; z = p[2];
    return true;
  }
  inline int TpBuildSpellsCore() {
    __try {
      const int teleIdx = TpIndex( "AI_TELEPORT" );
      if( teleIdx < 0 ) return 0;
      const int n = parser->symtab.table.GetNum();
      int found = 0;
      for( int k = 0; k < n; k++ ) {
        zCPar_Symbol* const s = parser->symtab.table[k];
        if( !s ) continue;
        const char* nm = s->name.ToChar();
        if( !nm || strncmp( nm, "SPL_", 4 ) != 0 ) continue;
        int v;
        if( !TpSymInt( s, v ) || v <= 0 || v >= TP_SPELLS ) continue;
        char fn[96];
        const char* const pre[2] = { "SPELL_CAST_", "SPELL_LOGIC_" };
        for( int p = 0; p < 2; p++ ) {
          snprintf( fn, sizeof( fn ), "%s%s", pre[p], nm + 4 );
          zCPar_Symbol* const f = TpSym( fn );
          if( f && TpWpOfFunc( f, teleIdx, g_tpSpellWp[v], 40, g_tpSpellWpAlt[v] ) ) {
            found++;
            break;
          }
        }
      }
      return found;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return -1;
    }
  }
  inline void TpBuildSpells() {
    if( !parser || g_tpParserSeen == (void*)parser ) return;
    g_tpParserSeen = (void*)parser;
    for( int i = 0; i < TP_SPELLS; i++ ) {
      g_tpSpellWp[i][0]  = 0;
      g_tpSpellWpAlt[i][0][0] = 0;
      g_tpSpellWpAlt[i][1][0] = 0;
      g_tpSpellKnown[i]  = true;
    }
    const int wraps = TpBuildWrapsCore( TpIndex( "AI_TELEPORT" ) );
    if( wraps > 0 )
      MM_LOG( zSTRING( "teleports: " ) + zSTRING( wraps ) + " script functions call AI_Teleport with a waypoint argument" );
    const int found = TpBuildSpellsCore();
    if( found < 0 )
      MM_LOG( "teleports: scripts not readable" );
    else
      MM_LOG( zSTRING( "teleports: " ) + zSTRING( found ) + " teleport spells found in scripts" );
    const int gates = TpBuildGatesCore();
    if( gates > 0 )
      MM_LOG( zSTRING( "teleports: " ) + zSTRING( gates ) + " destinations in teleport menus (unlocked by script variables)" );
    else if( gates < 0 )
      MM_LOG( "teleports: teleport menus not readable" );
  }
  inline const char* TpItemName( oCItem* it ) {
    const char* nm = it->description.ToChar();
    if( !nm || !*nm ) nm = it->name.ToChar();
    return nm;
  }
  inline bool TpRefreshCore( oCWorld* world ) {
    __try {
      for( int c = 0; c < INV_LISTS; c++ ) {
        int guard = 0;
        for( zCListSort<oCItem>* e = InvFirst( player, c ); e && guard < 4096; e = e->next, guard++ ) {
          oCItem* const it = e->data;
          if( !it || it->spell <= 0 || it->spell >= TP_SPELLS ) continue;
          const char* wp = g_tpSpellWp[it->spell];
          if( !wp[0] ) continue;
          const bool rune = ( it->flags & ITEM_MULTI_FLAG ) == 0;
          int dup = -1;
          for( int k = 0; k < g_tpCount; k++ )
            if( strcmp( g_tp[k].wp, wp ) == 0 ) { dup = k; break; }
          if( dup >= 0 ) {
            if( rune && !g_tp[dup].rune ) {
              g_tp[dup].rune = true;
              const char* nm = TpItemName( it );
              if( nm && *nm ) StrCopy( g_tp[dup].name, nm, TP_NAME );
            }
            continue;
          }
          if( g_tpCount >= TP_MAX ) continue;
          zCWaypoint* w = TpWaypoint( world, wp );
          for( int a = 0; !w && a < 2; a++ )
            if( g_tpSpellWpAlt[it->spell][a][0] ) {
              w = TpWaypoint( world, g_tpSpellWpAlt[it->spell][a] );
              if( w ) wp = g_tpSpellWpAlt[it->spell][a];
            }
          if( !w ) continue;
          TTeleport& t = g_tp[g_tpCount++];
          t.x = w->pos[0];
          t.z = w->pos[2];
          t.spell = it->spell;
          t.rune  = rune;
          StrCopy( t.wp, wp, 40 );
          const char* nm = TpItemName( it );
          StrCopy( t.name, ( nm && *nm ) ? nm : wp, TP_NAME );
        }
      }
      for( int g = 0; g < g_tpGateN && g_tpCount < TP_MAX; g++ ) {
        int v;
        if( !TpVarInt( g_tpGate[g].var, v ) ) continue;
        if( g_tpGate[g].val == TP_GATE_NONZERO ? v == 0 : v != g_tpGate[g].val ) continue;
        bool dup = false;
        for( int k = 0; k < g_tpCount && !dup; k++ )
          dup = strcmp( g_tp[k].wp, g_tpGate[g].wp ) == 0;
        if( dup ) continue;
        float gx = 0.0f, gz = 0.0f;
        const TTpGate& gg = g_tpGate[g];
        if( gg.posWorld != (void*)world || gg.posOk != 1 ) continue;
        gx = gg.px;
        gz = gg.pz;
        TTeleport& t = g_tp[g_tpCount++];
        t.x = gx;
        t.z = gz;
        t.spell = -1;
        t.rune  = false;
        StrCopy( t.wp, g_tpGate[g].wp, 40 );
        StrCopy( t.name, g_tpGate[g].name, TP_NAME );
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline void TpGatePositions( oCWorld* world ) {
    for( int g = 0; g < g_tpGateN; g++ ) {
      TTpGate& gg = g_tpGate[g];
      if( gg.posWorld == (void*)world ) continue;
      gg.posWorld = (void*)world;
      gg.posOk = TpTargetPos( world, gg.wp, gg.px, gg.pz ) ? 1 : 2;
    }
  }
  inline void TpRefresh( oCWorld* world ) {
    g_tpCount = 0;
    g_tpWorld = world;
    if( !player || !world || !world->wayNet ) return;
    TpBuildSpells();
    TpGatePositions( world );
    if( !TpRefreshCore( world ) ) g_tpCount = 0;
  }
  inline zCOLOR ColTeleport() { return zCOLOR( 190, 120, 255, 255 ); }
  static int   g_tpHover = -1;
  inline void TpMark( float px, float py, float s ) {
    const float r = s * 0.42f;
    const zCOLOR dark( 18, 16, 15, 255 );
    const zCOLOR col = ColTeleport();
    for( int k = 0; k <= (int)r; k++ ) {
      const float w = ( r - (float)k ) * 2.0f + 2.0f;
      TexRect( px - w * 0.5f - 1.0f, py - (float)k - 1.0f, w + 2.0f, 1.0f, dark );
      TexRect( px - w * 0.5f - 1.0f, py + (float)k,        w + 2.0f, 1.0f, dark );
    }
    for( int k = 0; k < (int)r; k++ ) {
      const float w = ( r - (float)k ) * 2.0f;
      TexRect( px - w * 0.5f, py - (float)k - 1.0f, w, 1.0f, col );
      TexRect( px - w * 0.5f, py + (float)k,        w, 1.0f, col );
    }
  }
  inline void DrawTeleports() {
    g_tpHover = -1;
    if( !g_showTeleports || !ogame || !player ) return;
    oCWorld* const world = ogame->GetGameWorld();
    if( !world ) return;
    const DWORD now = GetTickCount();
    if( world != g_tpWorld || (long)( now - g_tpNext ) >= 0 ) {
      g_tpNext = now + TP_CACHE_MS;
      TpRefresh( world );
    }
    const float size = MarkSizePxK() * 1.05f;
    for( int i = 0; i < g_tpCount; i++ ) {
      float mx, my;
      WorldToPx( g_tp[i].x, g_tp[i].z, mx, my );
      if( mx < g_clipX0 || mx > g_clipX1 || my < g_clipY0 || my > g_clipY1 || !MarkerVisible( mx, my ) ) continue;
      if( !DrawIcon( ICON_TELEPORT, mx, my, size ) )
        TpMark( mx, my, size );
      if( g_full && g_curReady ) {
        const float dx = g_curX - mx, dy = g_curY - my;
        if( dx * dx + dy * dy <= size * size * 0.36f ) g_tpHover = i;
      }
    }
  }
  inline void DrawTeleportHover() {
    if( g_tpHover < 0 || g_tpHover >= g_tpCount || !g_full ) return;
    char line[96];
    snprintf( line, sizeof( line ), "%s%s", T( TX_TP_TITLE ), g_tp[g_tpHover].name );
    const char* const hint = ( g_tp[g_tpHover].spell < 0 ) ? T( TX_TP_MENU ) : 0;
    const float lh  = LineHPx();
    const float pad = 4.0f;
    float tw  = TextWidthPx( line );
    if( hint ) {
      const float hw = TextWidthPx( hint );
      if( hw > tw ) tw = hw;
    }
    const float bh = ( hint ? lh * 2.0f : lh ) + pad * 2.0f;
    float bx = g_curX + 14.0f, by = g_curY + 10.0f;
    int sw = 0, sh = 0;
    ScreenSizePx( sw, sh );
    if( bx + tw + pad * 2.0f > (float)sw ) bx = (float)sw - tw - pad * 2.0f - 2.0f;
    if( by + bh > (float)sh ) by = (float)sh - bh - 2.0f;
    Pane( bx, by, tw + pad * 2.0f, bh );
    MarkText( bx + pad, by + pad, line, ColTeleport() );
    if( hint )
      MarkText( bx + pad, by + pad + lh, hint, ColDim() );
  }
}
