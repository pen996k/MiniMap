#pragma once
namespace MiniMap {
#include "MiniMap_WorldTp.h"
  enum { WTP_MAX = 192 };
  struct TWtp {
    zCVob* vob;
    zCVob* mob;
    float  x, z;
    char   dest[48];
  };
  static TWtp     g_wtp[WTP_MAX];
  static int      g_wtpCount = 0;
  static oCWorld* g_wtpWorld = 0;
  inline void WtpFree() {
    g_wtpCount = 0;
    g_wtpWorld = 0;
  }
  inline bool WtpIsStr( void*, int i ) {
    zCPar_Symbol* const s = parser ? parser->GetSymbol( i ) : 0;
    return s && s->type == zPAR_TYPE_STRING && ( s->flags & zPAR_FLAG_CONST ) && s->ele == 1 && s->stringdata;
  }
  inline const char* WtpStr( int i ) {
    zCPar_Symbol* const s = parser ? parser->GetSymbol( i ) : 0;
    return ( s && s->type == zPAR_TYPE_STRING && s->stringdata ) ? s->stringdata->ToChar() : 0;
  }
  inline bool WtpPosOf( oCWorld* world, const char* name, float& x, float& z ) {
    if( !world || !name || !name[0] ) return false;
    zSTRING nm( name );
    nm.Upper();
    if( world->wayNet ) {
      zCWaypoint* const w = world->wayNet->GetWaypoint( nm );
      if( w ) { x = w->pos[0]; z = w->pos[2]; return true; }
    }
    zCVob* const v = world->SearchVobByName( nm );
    if( v && v->homeWorld == (zCWorld*)world ) {
      const zVEC3 p = v->GetPositionWorld();
      x = p[0]; z = p[2];
      return true;
    }
    return false;
  }
  inline bool WtpWpPos( void* ctx, int strIndex, float& x, float& z ) {
    return WtpPosOf( (oCWorld*)ctx, WtpStr( strIndex ), x, z );
  }
  inline WorldTp::TWtSyms WtpSyms() {
    WorldTp::TWtSyms y;
    y.tele    = TpIndex( "AI_TELEPORT" );
    y.dist    = TpIndex( "NPC_GETDISTTOWP" );
    y.nearest = TpIndex( "NPC_GETNEARESTWP" );
    y.strcmp_ = TpIndex( "HLP_STRCMP" );
    y.hero    = TpIndex( "HERO" );
    y.self    = TpIndex( "SELF" );
    return y;
  }
  enum { WTP_TRIG = 512 };
  struct TWtpTrig { zCVob* vob; float cx, cz; };
  static TWtpTrig g_wtpTrig[WTP_TRIG];
  static int      g_wtpTrigN = 0;
  inline bool WtpTriggersCore( oCWorld* world ) {
    __try {
      g_wtpTrigN = 0;
      zCClassDef* const def = oCTriggerScript::classDef;
      if( !def ) return true;
      zCTree<zCVob>* const root = &world->globalVobTree;
      zCTree<zCVob>* node = root->firstChild;
      int guard = 0;
      while( node && guard < TREE_NODE_LIMIT && g_wtpTrigN < WTP_TRIG ) {
        guard++;
        zCVob* const v = node->data;
        if( v && zCObject::CheckInheritance( def, v->_GetClassDef() ) ) {
          oCTriggerScript* const tr = (oCTriggerScript*)v;
          if( tr->filterFlags.respondToPC && !tr->filterFlags.respondToNPC && tr->numCanBeActivated < 0
              && tr->scriptFunc.Length() > 0 ) {
            TWtpTrig& t = g_wtpTrig[g_wtpTrigN++];
            t.vob = v;
            const zTBBox3D& bb = v->bbox3D;
            t.cx = ( bb.mins[0] + bb.maxs[0] ) * 0.5f;
            t.cz = ( bb.mins[2] + bb.maxs[2] ) * 0.5f;
          }
        }
        node = VobTreeNext( node, root );
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      g_wtpTrigN = 0;
      return false;
    }
  }
  inline int WtpDestsCore( int adr, const WorldTp::TWtSyms& y, WorldTp::TWtDest* d ) {
    __try {
      return WorldTp::WtDests( parser->stack.stack, parser->stack.stacksize, adr, 0, WtpIsStr, y, d );
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return 0;
    }
  }
  inline zCVob* WtpPlatform( oCWorld* world, float cx, float cz ) {
    zCVob* best = 0;
    float bd = 600.0f * 600.0f;
    for( zCListSort<zCVob>* node = world->voblist; node; node = node->next ) {
      zCVob* const v = node->data;
      if( !v || v->homeWorld != (zCWorld*)world ) continue;
      zCClassDef* const def = v->_GetClassDef();
      if( !SwitchIs( oCMobInter::classDef, def ) || SwitchIs( oCMobContainer::classDef, def )
          || SwitchIs( oCMobDoor::classDef, def ) || SwitchIs( oCMobLadder::classDef, def )
          || SwitchIs( oCMobFire::classDef, def ) || SwitchIs( oCMobBed::classDef, def ) )
        continue;
      if( SwitchKindOf( v ) >= 0 ) continue;
      const zVEC3 p = v->GetPositionWorld();
      const float dx = p[0] - cx, dz = p[2] - cz;
      const float d2 = dx * dx + dz * dz;
      if( d2 < bd ) { bd = d2; best = v; }
    }
    return best;
  }
  inline void WtpZones( oCWorld* world, const WorldTp::TWtSyms& y ) {
    if( !WtpTriggersCore( world ) ) {
      MM_LOG( "world teleports: exception while collecting triggers" );
      return;
    }
    for( int i = 0; i < g_wtpTrigN && g_wtpCount < WTP_MAX; i++ ) {
      oCTriggerScript* const tr = (oCTriggerScript*)g_wtpTrig[i].vob;
      zCPar_Symbol* const f = parser->GetSymbol( tr->scriptFunc );
      if( !f || f->type != zPAR_TYPE_FUNC || ( f->flags & zPAR_FLAG_EXTERNAL ) ) continue;
      WorldTp::TWtDest d[WorldTp::WT_MAX_DEST];
      const int n = WtpDestsCore( f->single_intdata, y, d );
      if( n <= 0 ) continue;
      const int k = WorldTp::WtChoose( d, n, g_wtpTrig[i].cx, g_wtpTrig[i].cz, (void*)world, WtpWpPos );
      if( k < 0 ) continue;
      zCVob* const mob = WtpPlatform( world, g_wtpTrig[i].cx, g_wtpTrig[i].cz );
      TWtp& t = g_wtp[g_wtpCount++];
      t.vob = g_wtpTrig[i].vob;
      t.mob = mob;
      t.x = g_wtpTrig[i].cx;
      t.z = g_wtpTrig[i].cz;
      if( mob ) { const zVEC3 p = mob->GetPositionWorld(); t.x = p[0]; t.z = p[2]; }
      const char* const dn = WtpStr( d[k].dest );
      StrCopy( t.dest, dn ? dn : "", 48 );
    }
  }
  inline void WtpEnsure( oCWorld* world ) {
    if( world == g_wtpWorld ) return;
    WtpFree();
    g_wtpWorld = world;
    if( !world || !parser ) return;
    const WorldTp::TWtSyms y = WtpSyms();
    if( y.tele >= 0 )
      WtpZones( world, y );
    MM_LOG( zSTRING( "world teleports: zones " ) + zSTRING( g_wtpCount ) + " (player triggers " + zSTRING( g_wtpTrigN ) + ")" );
  }
  inline int WtpIndexOf( zCVob* v ) {
    if( !v || !g_showTeleports ) return -1;
    for( int i = 0; i < g_wtpCount; i++ )
      if( g_wtp[i].vob == v || ( g_wtp[i].mob && g_wtp[i].mob == v ) ) return i;
    return -1;
  }
  inline bool WtpIsPlatform( zCVob* v ) {
    if( !v || !g_showTeleports || !ogame ) return false;
    oCWorld* const world = ogame->GetGameWorld();
    if( !world || v->homeWorld != (zCWorld*)world ) return false;
    WtpEnsure( world );
    for( int i = 0; i < g_wtpCount; i++ )
      if( g_wtp[i].mob == v ) return true;
    return false;
  }
  inline void DrawWorldTeleports() {
    if( !g_showTeleports || !ogame || !player ) return;
    oCWorld* const world = ogame->GetGameWorld();
    if( !world ) return;
    WtpEnsure( world );
    if( g_wtpCount <= 0 ) return;
    const float size = MarkSizePxK() * 0.95f;
    for( int i = 0; i < g_wtpCount; i++ ) {
      const TWtp& t = g_wtp[i];
      float mx, my;
      WorldToPx( t.x, t.z, mx, my );
      if( mx < g_clipX0 || mx > g_clipX1 || my < g_clipY0 || my > g_clipY1 || !MarkerVisible( mx, my ) ) continue;
      if( !DrawIcon( ICON_TELEPORT, mx, my, size ) )
        TpMark( mx, my, size );
      HoverConsider( t.vob, mx, my, size, false );
    }
  }
  inline int WtpHoverLines( zCVob* vob, zSTRING* lines, int max ) {
    const int i = WtpIndexOf( vob );
    if( i < 0 || max < 2 ) return 0;
    const TWtp& t = g_wtp[i];
    int n = 0;
    lines[n++] = zSTRING( T( TX_HOV_WTP ) );
    if( t.dest[0] )
      lines[n++] = zSTRING( T( TX_HOV_EXIT_TO ) ) + " " + zSTRING( t.dest );
    return n;
  }
}
