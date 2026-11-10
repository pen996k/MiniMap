#pragma once
namespace MiniMap {
  inline bool SearchIn( const zSTRING& s, const char* needle ) {
    return SearchInChars( const_cast<zSTRING&>( s ).ToChar(), needle );
  }
  inline bool SearchTextHit( oCItem* it ) {
    if( !g_searchNorm[0] ) return true;
    if( const char* tn = NameTxtInst( it->instanz ) )
      if( SearchInChars( tn, g_searchNorm ) ) return true;
    return SearchIn( it->name, g_searchNorm ) || SearchIn( it->description, g_searchNorm );
  }
  inline bool SearchItemHit( oCItem* it ) {
    if( !it ) return false;
    return SearchTextHit( it ) && SearchCatHit( it );
  }
  inline bool SearchNpcHit( oCNpc* npc ) {
    if( !npc || npc == player ) return false;
    if( g_searchNorm[0] && !SearchIn( npc->name[0], g_searchNorm ) ) {
      const char* tn = NameTxtInst( npc->instanz );
      if( !tn || !SearchInChars( tn, g_searchNorm ) ) return false;
    }
    return SearchCatHitNpc( npc );
  }
  inline bool SearchInvHit( oCNpc* npc, int mode ) {
    const bool anyCat = ( g_searchCat == SCAT_ALL || SearchCatIsNpc( g_searchCat ) );
    for( int c = 0; c < INV_LISTS; c++ ) {
      int guard = 0;
      for( zCListSort<oCItem>* e = InvFirst( npc, c ); e && guard < 4096; e = e->next, guard++ ) {
        oCItem* const it = e->data;
        if( !it || !InvItemCounts( it, mode ) ) continue;
        if( SearchTextHit( it ) && ( anyCat || SearchCatOf( it ) == g_searchCat ) ) return true;
      }
      TPackIt t;
      PackStart( t, InvPacked( npc, c ) );
      while( PackNext( t ) ) {
        const TInvName* const en = InvNameGet( t.key );
        if( !en || en->state != 1 ) continue;
        char nn[INVN_NAME];
        if( g_searchNorm[0] ) {
          SearchNormalize( en->name, nn, INVN_NAME );
          if( strncmp( nn, g_searchNorm, strlen( g_searchNorm ) ) != 0 ) continue;
        }
        if( anyCat || en->cat == g_searchCat ) return true;
      }
    }
    return false;
  }
  inline bool SearchInvWanted( oCNpc* npc, bool trader ) {
    if( g_searchCat == SCAT_TRADER ) return trader;
    if( g_searchInv >= 2 ) return true;
    return g_searchInv == 1 && trader;
  }
  inline void SearchTextChanged() {
    SearchNormalize( g_searchBuf, g_searchNorm, 48 );
    g_suggBatch = true;
    SearchMatchPlaces();
    SearchMatchExits();
    SearchMatchItems();
    SearchMatchChests();
    SearchMatchNpcs();
    SearchMatchCorpses();
    g_suggBatch = false;
    g_suggSel = -1;
    SearchSuggestBuild();
  }
  inline bool SearchSuggestApply( int idx ) {
    if( idx < 0 || idx >= g_suggN ) return false;
    StrCopy( g_searchBuf, g_suggName[idx], 41 );
    g_searchLen = (int)strlen( g_searchBuf );
    return true;
  }
  inline void SearchClear() {
    g_search       = 0;
    g_searchLen    = 0;
    g_searchBuf[0] = 0;
    g_searchCat    = SCAT_ALL;
    SearchTextChanged();
  }
  inline void SearchCommit() {
    if( g_search == 1 ) g_search = SearchHasFilter() ? 2 : 0;
  }
  inline void SearchCatStep( int dir ) {
    g_searchCat = ( g_searchCat + dir + SCAT_COUNT ) % SCAT_COUNT;
    SearchTextChanged();
    MM_LOG( zSTRING( "search: category " ) + zSTRING( g_searchCat ) + ", found " + zSTRING( g_searchFound ) );
  }
  static DWORD g_searchStartAt = 0;
  inline void SearchStart() {
    g_search      = 1;
    g_searchLen = (int)strlen( g_searchBuf );
    g_searchStartAt = GetTickCount();
    g_preHead = g_preCount = 0;
    if( zinput )
      zinput->ClearKeyBuffer();
    MM_LOG( "search: typing" );
  }
  inline void ReadSearch() {
    if( g_search != 1 || !zinput ) return;
    if( !g_full ) {
      SearchCommit();
      return;
    }
    if( ConsoleOpen() ) return;
    EatKeys();
    const DWORD now = GetTickCount();
    if( !g_charsExact && now - g_searchStartAt < 150 ) {
      while( FetchChar() ) {}
      return;
    }
    const DWORD REPEAT_FIRST = 350;
    const DWORD REPEAT_NEXT  = 45;
    const DWORD REPEAT_GAP   = 120;
    if( now - g_lastTime > REPEAT_GAP ) g_lastChar = 0;
    bool changed = false;
    for( int guard = 0; guard < 64; guard++ ) {
      const unsigned char c = FetchChar();
      if( c == 0 ) break;
      if( !g_charsExact ) {
        g_lastTime = now;
        if( (char)c == g_lastChar && now < g_nextTime ) continue;
        g_nextTime = now + ( ( (char)c == g_lastChar ) ? REPEAT_NEXT : REPEAT_FIRST );
        g_lastChar = (char)c;
      }
      if( c == 13 ) {
        if( !changed && SearchSuggestApply( g_suggSel ) ) changed = true;
        if( changed )
          SearchTextChanged();
        SearchCommit();
        MM_LOG( zSTRING( "search: confirmed '" ) + zSTRING( g_searchBuf ) + "', found " + zSTRING( g_searchFound ) );
        return;
      }
      if( c == 27 ) {
        SearchClear();
        return;
      }
      if( c == 8 ) {
        if( g_searchLen > 0 ) {
          g_searchBuf[--g_searchLen] = 0;
          changed = true;
        }
        continue;
      }
      if( c < 32 || g_searchLen >= 40 || !SearchCharOk( c ) ) continue;
      g_searchBuf[g_searchLen++] = (char)c;
      g_searchBuf[g_searchLen]   = 0;
      changed = true;
    }
    if( changed )
      SearchTextChanged();
  }
  inline void DrawSearchBar() {
    if( !g_full || !screen || g_search == 0 ) return;
    int x, y, w, h;
    FullRect( x, y, w, h );
    const float lh  = LineHPx();
    const float pad = 4.0f;
    const float bh  = lh + pad * 2.0f;
    int sw = 0, sh = 0;
    ScreenSizePx( sw, sh );
    float noteY = (float)( y + h ) - bh - 4.0f;
    if( noteY + bh > (float)sh - 6.0f ) noteY = (float)sh - 6.0f - bh;
    if( noteY < (float)y + 4.0f )       noteY = (float)y + 4.0f;
    const float px    = (float)x + 8.0f;
    const float pw    = (float)w - 16.0f;
    const float inner = pw - pad * 2.0f;
    const float gap   = lh * 2.0f;
    char line[160];
    int pfx = snprintf( line, sizeof( line ), "%s< %s >  ", T( TX_SEARCH_PREFIX ), SearchCatName( g_searchCat ) );
    if( pfx < 0 ) pfx = 0; else if( pfx >= (int)sizeof( line ) ) pfx = (int)sizeof( line ) - 1;
    const char* caret = g_search == 1 ? ( ( ( GetTickCount() / 400 ) & 1 ) == 0 ? "_" : " " ) : "";
    float linew = 0.0f;
    for( int skip = 0; ; skip++ ) {
      snprintf( line + pfx, sizeof( line ) - (size_t)pfx, "%s%s%s", skip > 0 ? "..." : "", g_searchBuf + skip, caret );
      linew = TextWidthPx( line );
      if( linew <= inner || skip >= g_searchLen ) break;
    }
    char cnt[128];
    cnt[0] = 0;
    if( SearchHasFilter() ) {
      const bool npcOnly = SearchCatIsNpc( g_searchCat );
      int cl = snprintf( cnt, sizeof( cnt ), "(" );
      if( !npcOnly && cl > 0 && cl < (int)sizeof( cnt ) )
        cl += snprintf( cnt + cl, sizeof( cnt ) - (size_t)cl, "%s%d", T( TX_SEARCH_FOUND ), g_searchFound + g_searchPlaceFound + SearchExitFound() );
      if( ( g_searchNpcFound > 0 || npcOnly ) && cl > 0 && cl < (int)sizeof( cnt ) )
        cl += snprintf( cnt + cl, sizeof( cnt ) - (size_t)cl, "%s%s%d", npcOnly ? "" : ", ", T( TX_SEARCH_NPCS ), g_searchNpcFound );
      if( g_searchInChest + g_corpseFound > 0 && cl > 0 && cl < (int)sizeof( cnt ) )
        cl += snprintf( cnt + cl, sizeof( cnt ) - (size_t)cl, ", %s%d", T( TX_SEARCH_INCHEST ), g_searchInChest + g_corpseFound );
      if( cl > 0 && cl < (int)sizeof( cnt ) ) snprintf( cnt + cl, sizeof( cnt ) - (size_t)cl, ")" );
    }
    const float cntw = cnt[0] ? TextWidthPx( cnt ) : 0.0f;
    const char* tip  = g_search == 1 ? T( TX_SEARCH_TIP ) : 0;
    const float tipw = tip ? TextWidthPx( tip ) : 0.0f;
    int   rows   = 1;
    float rowW   = linew;
    int   cntRow = 0;
    float cntX   = px + pad + linew + lh;
    if( cnt[0] ) {
      if( rowW + lh + cntw > inner ) {
        cntRow = rows++;
        cntX   = px + pad;
        rowW   = cntw;
      }
      else
        rowW += lh + cntw;
    }
    int tipRow = rows - 1;
    if( tip && rowW + gap + tipw > inner ) tipRow = rows++;
    const float ph = (float)rows * lh + (float)( rows + 1 ) * pad;
    float by = noteY - 4.0f - ph;
    if( by < (float)y + 4.0f ) by = (float)y + 4.0f;
    Pane( px, by, pw, ph );
    MarkText( px + pad, by + pad, line, ColMarkLight() );
    if( cnt[0] )
      MarkText( cntX, by + pad + (float)cntRow * ( lh + pad ), cnt, ColMarkLight() );
    if( tip ) {
      float tx = px + pw - pad - tipw;
      if( tx < px + pad ) tx = px + pad;
      MarkText( tx, by + pad + (float)tipRow * ( lh + pad ), tip, ColDim() );
    }
    g_suggShown = 0;
    if( g_search == 1 && g_suggN > 0 ) {
      const float rowH = lh + pad;
      int n = g_suggN < SUGG_SHOW ? g_suggN : SUGG_SHOW;
      const float room = by - 4.0f - ( (float)y + 4.0f ) - pad;
      while( n > 0 && (float)n * rowH > room )
        n--;
      if( n > 0 ) {
        const float lph = (float)n * rowH + pad;
        const float ly  = by - 4.0f - lph;
        char row[SUGG_LEN + 16];
        float lw = 0.0f;
        for( int i = 0; i < n; i++ ) {
          snprintf( row, sizeof( row ), "%s (%d)", g_suggName[i], g_suggCnt[i] < 0 ? 0 : g_suggCnt[i] );
          const float tw = TextWidthPx( row );
          if( tw > lw ) lw = tw;
        }
        float lpw = lw + pad * 4.0f;
        if( lpw > pw ) lpw = pw;
        Pane( px, ly, lpw, lph );
        for( int i = 0; i < n; i++ ) {
          snprintf( row, sizeof( row ), "%s (%d)", g_suggName[i], g_suggCnt[i] < 0 ? 0 : g_suggCnt[i] );
          const float ry = ly + pad + (float)i * rowH;
          if( i == g_suggSel )
            TexRect( px + 2.0f, ry - pad * 0.5f, lpw - 4.0f, rowH, ColGold() );
          MarkText( px + pad * 2.0f, ry, row, i == g_suggSel ? ColMarkPane() : ColMarkLight() );
        }
        g_suggX0 = px; g_suggX1 = px + lpw; g_suggY0 = ly + pad * 0.5f;
        g_suggRowH = rowH; g_suggFirst = 0; g_suggShown = n;
      }
    }
  }
  inline void ItemCacheFree( bool release = true ) {
    if( g_itemCache && release ) {
      for( int i = 0; i < g_itemCount; i++ )
        if( g_itemCache[i] )
          g_itemCache[i]->Release();
    }
    MiniMapCommon::MmFree( g_itemCache );
    g_itemCache     = 0;
    g_itemCount     = 0;
    g_itemNext      = 0;
    g_itemWorld     = 0;
    g_itemGen++;
    g_itemLastShown = 0;
    g_itemMatchN    = 0;
    g_searchFound   = 0;
  }
  inline bool ItemHeld( zCVob* vob, zCClassDef* npcDef ) {
    zCTree<zCVob>* t = vob->globalVobTreeNode;
    for( int depth = 0; t && depth < 8; depth++ ) {
      t = t->parent;
      if( !t || !t->data ) return false;
      if( npcDef && zCObject::CheckInheritance( npcDef, t->data->_GetClassDef() ) ) return true;
    }
    return false;
  }
  inline void ItemCacheRefresh( oCWorld* world ) {
    zCClassDef* const itemDef = oCItem::classDef;
    zCClassDef* const npcDef  = oCNpc::classDef;
    if( !itemDef ) return;
    int n = 0;
    for( zCListSort<zCVob>* node = world->voblist; node; node = node->next ) {
      zCVob* const vob = node->data;
      if( vob && zCObject::CheckInheritance( itemDef, vob->_GetClassDef() ) )
        n++;
    }
    oCItem** arr = 0;
    int      cnt = 0;
    if( n > 0 ) {
      arr = MiniMapCommon::MmAlloc<oCItem*>( (size_t)( n ) );
      if( !arr ) return;
      for( zCListSort<zCVob>* node = world->voblist; node && cnt < n; node = node->next ) {
        zCVob* const vob = node->data;
        if( !vob || !zCObject::CheckInheritance( itemDef, vob->_GetClassDef() ) ) continue;
        if( vob->homeWorld != (zCWorld*)world ) continue;
        if( ItemHeld( vob, npcDef ) ) continue;
        if( HiddenInst( ( (oCItem*)vob )->instanz ) ) continue;
        arr[cnt++] = (oCItem*)vob;
      }
    }
    for( int i = 0; i < cnt; i++ )
      arr[i]->AddRef();
    oCItem** const old  = g_itemCache;
    const int      oldN = g_itemCount;
    const bool same = ( old != 0 ) && ( oldN == cnt ) && ( cnt == 0 || memcmp( old, arr, (size_t)cnt * sizeof( oCItem* ) ) == 0 );
    if( !same )
      g_itemGen++;
    g_itemCache = arr;
    g_itemCount = cnt;
    g_itemWorld = world;
    for( int i = 0; i < oldN; i++ )
      if( old[i] )
        old[i]->Release();
    MiniMapCommon::MmFree( old );
    SearchMatchItems();
    SearchSuggestBuild();
    ItemIconsBuild( g_itemCache, g_itemCount );
    static int loggedN = -1;
    if( loggedN < 0 || ( cnt - loggedN ) * ( cnt - loggedN ) > 400 ) {
      loggedN = cnt;
      MM_LOG( zSTRING( "items: lying in world " ) + zSTRING( cnt ) + " (items in vob list " + zSTRING( n ) + ")" );
    }
  }
  inline int ItemKindIcon( oCItem* item ) {
    if( !item ) return ICON_ITEM_OTHER;
    switch( ItemKindTxt( item ) ) {
      case IK_MELEE_1H: return ICON_ITEM_1H;
      case IK_MELEE_2H: return ICON_ITEM_2H;
      case IK_RANGED:   return ICON_ITEM_RANGED;
      case IK_ARMOR:    return ICON_ITEM_ARMOR;
      case IK_POTION:   return ICON_ITEM_POTION;
      case IK_FOOD:     return ICON_ITEM_PLANT;
      case IK_MAGIC:    return ICON_ITEM_MAGIC;
      case IK_DOCS: case IK_GOLD: case IK_OTHER: return ICON_ITEM_OTHER;
      default: break;
    }
    const int mf = item->mainflag, fl = item->flags;
    if( mf & ITM_CAT_NF ) return ( fl & ( ITM_FLAG_2HD_SWD | ITM_FLAG_2HD_AXE ) ) ? ICON_ITEM_2H : ICON_ITEM_1H;
    if( mf & ( ITM_CAT_FF | ITM_CAT_MUN ) ) return ICON_ITEM_RANGED;
    if( mf & ITM_CAT_ARMOR )  return ICON_ITEM_ARMOR;
    if( mf & ITM_CAT_POTION ) return ICON_ITEM_POTION;
    if( mf & ITM_CAT_FOOD )   return ICON_ITEM_PLANT;
    if( ( mf & ( ITM_CAT_RUNE | ITM_CAT_MAGIC ) ) || ( fl & ( ITM_FLAG_AMULET | ITM_FLAG_RING ) ) ) return ICON_ITEM_MAGIC;
    return ICON_ITEM_OTHER;
  }
  inline int ItemIconOf( oCItem* item ) {
    if( !g_itemColors || !item ) return ICON_ITEM_OTHER;
    return ItemKindIcon( item );
  }
  inline zCOLOR ItemIconColor( int icon ) { return AtlasColor( icon ); }
  inline void ItemMark( int icon, float px, float py, float s, const zCOLOR& c, bool cheap ) {
    if( DrawIcon( icon, px, py, s ) ) return;
    const float r = s * 0.4f;
    const zCOLOR dark( 18, 16, 15, 255 );
    if( cheap ) {
      const float h = r * 0.75f;
      float x0 = px - h - 1.0f, y0 = py - h - 1.0f;
      float x1 = px + h + 1.0f, y1 = py + h + 1.0f;
      if( x0 < g_clipX0 ) x0 = g_clipX0;
      if( y0 < g_clipY0 ) y0 = g_clipY0;
      if( x1 > g_clipX1 ) x1 = g_clipX1;
      if( y1 > g_clipY1 ) y1 = g_clipY1;
      if( x1 <= x0 || y1 <= y0 ) return;
      TexRect( x0, y0, x1 - x0, y1 - y0, dark );
      x0 += 1.0f; y0 += 1.0f; x1 -= 1.0f; y1 -= 1.0f;
      if( x1 > x0 && y1 > y0 )
        TexRect( x0, y0, x1 - x0, y1 - y0, c );
      return;
    }
    for( int pass = 0; pass < 2; pass++ ) {
      const float  rr = ( pass == 0 ) ? r + 1.0f : r;
      const zCOLOR cc = ( pass == 0 ) ? dark : c;
      for( float k = -rr; k <= rr; k += 1.0f ) {
        const float d    = ( k < 0.0f ) ? -k : k;
        const float half = rr - d + 0.5f;
        MarkBar( px - half, py + k, px + half, cc );
      }
    }
  }
  inline void DrawItems() {
    if( !ogame || !player ) return;
    oCWorld* world = ogame->GetGameWorld();
    if( !world ) return;
    if( g_itemCache && world != g_itemWorld )
      ItemCacheFree( false );
    const DWORD now = GetTickCount();
    if( !g_itemWorld || (long)( now - g_itemNext ) >= 0 ) {
      g_itemNext = now + ITEM_CACHE_MS;
      ItemCacheRefresh( world );
      if( !g_itemWorld ) g_itemWorld = world;
    }
    if( !g_itemCache || g_itemCount <= 0 ) return;
    const bool filt = SearchFilterOn() && g_itemMatch && g_itemMatchN == g_itemCount;
    if( !filt && !g_showItems ) return;
    const bool  sight = ( g_showItems == SHOW_SIGHT ) && g_sightRays && !filt;
    const float range = sight ? SightRange( world, g_itemRange ) : (float)g_itemRange;
    const float r2    = range * range;
    const zCOLOR c    = ColItem();
    const float size = ItemSizePx() * 0.8f;
    int limit = ( g_itemLimit == 60 ) ? 200 : g_itemLimit;
    if( g_full ) {
      limit *= 4;
      if( limit < 800 ) limit = 800; else if( limit > 3000 ) limit = 3000;
    }
    if( !g_full && g_showItems == SHOW_WORLD && limit < 400 ) limit = 400;
    if( filt ) limit = 8000;
    if( g_showItems == SHOW_WORLD ) limit = 8000;
    static bool s_cheapItem = false;
    if( g_itemLastShown > 300 )      s_cheapItem = true;
    else if( g_itemLastShown < 240 ) s_cheapItem = false;
    const bool cheap = s_cheapItem;
    int drawn = 0;
    const zVEC3 pp = player->GetPositionWorld();
    g_rayBudget = g_rayBudgetItem;
    static int s_start = 0;
    if( s_start >= g_itemCount ) s_start = 0;
    const int start = sight ? s_start : 0;
    if( sight )
      s_start += ( g_sightRaysPerFrame > 1 ) ? g_sightRaysPerFrame : 1;
    for( int k = 0; k < g_itemCount && drawn < limit; k++ ) {
      const int i = ( start + k ) % g_itemCount;
      oCItem* const item = g_itemCache[i];
      if( !item ) continue;
      if( item->homeWorld != (zCWorld*)world ) continue;
      const zVEC3 p = item->GetPositionWorld();
      const float dx = p[0] - pp[0];
      const float dz = p[2] - pp[2];
      const float d2 = dx * dx + dz * dz;
      const bool hit = filt && g_itemMatch[i];
      if( filt && !hit && ( !g_searchOthers || !g_showItems ) ) continue;
      if( !hit && g_showItems < SHOW_WORLD && d2 > r2 ) continue;
      float mx, my;
      WorldToPx( p[0], p[2], mx, my );
      if( mx < g_clipX0 || mx > g_clipX1 || my < g_clipY0 || my > g_clipY1 ) continue;
      if( !MarkerVisible( mx, my ) ) continue;
      if( sight && !InSight( world, item, d2, true ) ) continue;
      if( filt && !hit ) {
        SearchDimMark( mx, my, size );
        drawn++;
        continue;
      }
      const int icon = ItemIconOf( item );
      const zCOLOR ic = ( icon == ICON_ITEM_OTHER ) ? c : ItemIconColor( icon );
      if( !ItemFileMark( i, g_itemCount, mx, my, size * ITEM_FILE_SCALE, ic ) )
        ItemMark( icon, mx, my, size, ic, cheap );
      if( hit )
        SearchRing( mx, my, size );
      HoverConsider( item, mx, my, size, false );
      drawn++;
    }
    g_itemLastShown = drawn;
    g_rayBudgetItem = g_rayBudget;
  }
  static const DWORD CHEST_CACHE_MS = 5000;
  static zCVob**  g_chestCache = 0;
  static int      g_chestCount = 0;
  static DWORD    g_chestNext  = 0;
  static oCWorld* g_chestWorld = 0;
  inline bool VobIsChest( zCVob* v ) {
    return v && oCMobContainer::classDef
        && zCObject::CheckInheritance( oCMobContainer::classDef, v->_GetClassDef() );
  }
  inline void ChestCacheFree( bool release = true ) {
    if( g_chestCache && release ) {
      for( int i = 0; i < g_chestCount; i++ )
        if( g_chestCache[i] )
          g_chestCache[i]->Release();
    }
    MiniMapCommon::MmFree( g_chestCache );
    g_chestCache = 0;
    g_chestCount = 0;
    g_chestNext  = 0;
    g_chestWorld = 0;
    g_chestMatchN   = 0;
    g_searchInChest = 0;
  }
  inline void ChestCacheRefresh( oCWorld* world ) {
    zCClassDef* const def = oCMobContainer::classDef;
    if( !def ) return;
    int n = 0;
    for( zCListSort<zCVob>* node = world->voblist; node; node = node->next )
      if( node->data && zCObject::CheckInheritance( def, node->data->_GetClassDef() ) )
        n++;
    zCVob** arr = 0;
    int     cnt = 0;
    if( n > 0 ) {
      arr = MiniMapCommon::MmAlloc<zCVob*>( (size_t)( n ) );
      if( !arr ) return;
      for( zCListSort<zCVob>* node = world->voblist; node && cnt < n; node = node->next ) {
        zCVob* const vob = node->data;
        if( !vob || !zCObject::CheckInheritance( def, vob->_GetClassDef() ) ) continue;
        if( vob->homeWorld != (zCWorld*)world ) continue;
        arr[cnt++] = vob;
      }
    }
    for( int i = 0; i < cnt; i++ )
      arr[i]->AddRef();
    zCVob** const old  = g_chestCache;
    const int     oldN = g_chestCount;
    g_chestCache = arr;
    g_chestCount = cnt;
    g_chestWorld = world;
    for( int i = 0; i < oldN; i++ )
      if( old[i] )
        old[i]->Release();
    MiniMapCommon::MmFree( old );
    SearchMatchChests();
  }
  inline bool SearchEnsure( bool*& arr, int& cap, int n ) {
    if( n <= cap && arr ) return true;
    int nc = ( cap > 0 ) ? cap : 256;
    while( nc < n ) nc *= 2;
    bool* const na = MiniMapCommon::MmAlloc<bool>( (size_t)( nc ) );
    if( !na ) return false;
    MiniMapCommon::MmFree( arr );
    arr = na;
    cap = nc;
    return true;
  }
  inline void SearchMatchItems() {
    g_itemMatchN  = 0;
    g_searchFound = 0;
    if( !SearchHasFilter() || !g_itemCache || g_itemCount <= 0 ) return;
    if( !SearchEnsure( g_itemMatch, g_itemMatchCap, g_itemCount ) ) return;
    int found = 0;
    for( int i = 0; i < g_itemCount; i++ ) {
      const bool hit = SearchItemHit( g_itemCache[i] );
      g_itemMatch[i] = hit;
      if( hit )
        found++;
    }
    g_itemMatchN  = g_itemCount;
    g_searchFound = found;
  }
  inline void SearchMatchNpcs() {
    g_npcMatchN      = 0;
    g_searchNpcFound = 0;
    if( !SearchHasFilter() || !SearchNpcFilter() || !player || !g_npcCache || g_npcCount <= 0 ) return;
    if( !SearchEnsure( g_npcMatch, g_npcMatchCap, g_npcCount ) ) return;
    int found = 0;
    for( int i = 0; i < g_npcCount; i++ ) {
      oCNpc* const npc = g_npcCache[i];
      bool hit = false;
      if( npc && npc->attribute[NPC_ATR_HITPOINTS] > 0 && !NpcFlagAt( i, NPCF_GONE ) ) {
        const bool itemCat = !SearchCatIsNpc( g_searchCat ) && g_searchCat != SCAT_ALL;
        hit = !itemCat && SearchNpcHit( npc );
        if( !hit && ( g_searchNorm[0] || itemCat || g_searchCat == SCAT_TRADER ) ) {
          const bool trader = NpcFlagAt( i, NPCF_TRADER );
          const bool catOk  = itemCat || g_searchCat == SCAT_ALL
                           || ( SearchCatIsNpc( g_searchCat ) && SearchCatHitNpc( npc ) );
          if( catOk && g_searchNorm[0] && SearchInvWanted( npc, trader ) )
            hit = SearchInvHit( npc, trader ? INV_GOODS : INV_ALL );
          else if( catOk && itemCat && SearchInvWanted( npc, trader ) )
            hit = SearchInvHit( npc, trader ? INV_GOODS : INV_ALL );
        }
      }
      g_npcMatch[i] = hit;
      if( hit )
        found++;
    }
    g_npcMatchN      = g_npcCount;
    g_searchNpcFound = found;
  }
  inline void SearchMatchChests() {
    g_chestMatchN   = 0;
    g_searchInChest = 0;
    if( !SearchHasFilter() || !g_chestCache || g_chestCount <= 0 ) return;
    if( !SearchEnsure( g_chestMatch, g_chestMatchCap, g_chestCount ) ) return;
    int found = 0;
    for( int i = 0; i < g_chestCount; i++ ) {
      bool hit = false;
      if( g_chestCache[i] ) {
        oCMobContainer* const ch = (oCMobContainer*)g_chestCache[i];
        int guard = 0;
        for( zCListSort<oCItem>* e = ch->containList.next; e && guard < 4096 && !hit;
             e = e->next, guard++ )
          if( e->data && SearchItemHit( e->data ) ) hit = true;
        if( !hit && g_searchNorm[0] ) hit = SearchMobHit( g_chestCache[i], -1 );
      }
      g_chestMatch[i] = hit;
      if( hit )
        found++;
    }
    g_chestMatchN   = g_chestCount;
    g_searchInChest = found;
  }
  inline bool CorpseLootable( oCItem* it ) {
    return !( ( it->mainflag & ITM_CAT_ARMOR ) && ( it->flags & ITM_FLAG_ACTIVE ) );
  }
  inline bool SearchCorpseHit( oCNpc* npc ) {
    if( !npc || npc == player || !SearchHasFilter() || npc->attribute[0] > 0 ) return false;
    int guard = 0;
    for( int c = 0; c < INV_LISTS; c++ )
      for( zCListSort<oCItem>* e = InvFirst( npc, c ); e && guard < 4096; e = e->next, guard++ )
        if( e->data && CorpseLootable( e->data ) && SearchItemHit( e->data ) ) return true;
    if( g_searchNorm[0] || ( g_searchCat != SCAT_ALL && !SearchCatIsNpc( g_searchCat ) ) ) {
      for( int c = 0; c < INV_LISTS; c++ ) {
        TPackIt t;
        PackStart( t, InvPacked( npc, c ) );
        while( PackNext( t ) ) {
          const TInvName* const en = InvNameGet( t.key );
          if( !en || en->state != 1 ) continue;
          if( g_searchNorm[0] ) {
            char nn[INVN_NAME];
            SearchNormalize( en->name, nn, INVN_NAME );
            if( strncmp( nn, g_searchNorm, strlen( g_searchNorm ) ) != 0 ) continue;
          }
          if( g_searchCat == SCAT_ALL || en->cat == g_searchCat ) return true;
        }
      }
    }
    return false;
  }
  inline void SearchMatchCorpses() {
    g_corpseMatchN = 0;
    g_corpseFound  = 0;
    if( !SearchHasFilter() || SearchCatIsNpc( g_searchCat ) || !player
        || !g_npcCache || g_npcCount <= 0 )
      return;
    if( !SearchEnsure( g_corpseMatch, g_corpseMatchCap, g_npcCount ) ) return;
    int found = 0;
    for( int i = 0; i < g_npcCount; i++ ) {
      oCNpc* const npc = g_npcCache[i];
      const bool hit = npc && npc->homeWorld && !NpcFlagAt( i, NPCF_GONE ) && SearchCorpseHit( npc );
      g_corpseMatch[i] = hit;
      if( hit )
        found++;
    }
    g_corpseMatchN = g_npcCount;
    g_corpseFound  = found;
  }
  inline void SearchSuggestAdd( const zSTRING& s, int n ) {
    const char* h = const_cast<zSTRING&>( s ).ToChar();
    if( !h ) return;
    while( *h == ' ' || ( *h && (unsigned char)*h < 32 ) )
      h++;
    if( !*h ) return;
    char norm[SUGG_LEN];
    SearchNormalize( h, norm, SUGG_LEN );
    for( int i = 0; i < g_suggN; i++ )
      if( strcmp( g_suggNorm[i], norm ) == 0 ) {
        g_suggCnt[i] += n;
        return;
      }
    if( g_suggN >= SUGG_SCAN ) return;
    StrCopy( g_suggName[g_suggN], h, SUGG_LEN );
    memcpy( g_suggNorm[g_suggN], norm, SUGG_LEN );
    g_suggCnt[g_suggN] = n;
    g_suggN++;
  }
  inline void SearchSuggestInv( oCNpc* npc, int mode ) {
    for( int c = 0; c < INV_LISTS; c++ ) {
      int guard = 0;
      for( zCListSort<oCItem>* e = InvFirst( npc, c ); e && guard < 4096; e = e->next, guard++ ) {
        oCItem* const it = e->data;
        if( !it || !InvItemCounts( it, mode ) || !SearchTextHit( it ) ) continue;
        SearchSuggestAdd( SearchIn( it->name, g_searchNorm ) ? it->name : it->description,
                          ( it->amount > 0 ) ? it->amount : 1 );
      }
      TPackIt t;
      PackStart( t, InvPacked( npc, c ) );
      while( PackNext( t ) ) {
        const TInvName* const en = InvNameGet( t.key );
        if( !en || en->state != 1 ) continue;
        char nn[INVN_NAME];
        SearchNormalize( en->name, nn, INVN_NAME );
        if( strncmp( nn, g_searchNorm, strlen( g_searchNorm ) ) == 0 )
          SearchSuggestAdd( zSTRING( en->name ), t.amount );
      }
    }
  }
  inline void SearchSuggestBuild() {
    if( g_suggBatch ) return;
    char selNorm[SUGG_LEN];
    selNorm[0] = 0;
    if( g_suggSel >= 0 && g_suggSel < g_suggN )
      memcpy( selNorm, g_suggNorm[g_suggSel], SUGG_LEN );
    g_suggN = 0;
    if( g_searchNorm[0] ) {
      if( g_itemMatch && g_itemMatchN == g_itemCount )
        for( int i = 0; i < g_itemCount; i++ )
          if( g_itemMatch[i] && g_itemCache[i] ) {
            oCItem* const it = g_itemCache[i];
            const int am = ( it->amount > 0 ) ? it->amount : 1;
            SearchSuggestAdd( SearchIn( it->name, g_searchNorm ) ? it->name : it->description, am );
          }
      if( SearchNpcFilter() && g_npcMatch && g_npcMatchN == g_npcCount )
        for( int i = 0; i < g_npcCount; i++ )
          if( g_npcMatch[i] && g_npcCache[i] ) {
            oCNpc* const sn = g_npcCache[i];
            if( SearchIn( sn->name[0], g_searchNorm ) )
              SearchSuggestAdd( sn->name[0], 1 );
            else
              SearchSuggestInv( sn, NpcFlagAt( i, NPCF_TRADER ) ? INV_GOODS : INV_ALL );
          }
      SearchSuggestPlaces();
      SearchSuggestExits();
    }
    SortNamedCounts( g_suggN, SUGG_LEN, &g_suggName[0][0], &g_suggNorm[0][0], g_suggCnt );
    const int shown = g_suggN < SUGG_SHOW ? g_suggN : SUGG_SHOW;
    if( selNorm[0] ) {
      g_suggSel = -1;
      for( int i = 0; i < shown; i++ )
        if( strcmp( g_suggNorm[i], selNorm ) == 0 ) {
          g_suggSel = i;
          break;
        }
    }
    if( g_suggSel >= shown ) g_suggSel = shown - 1;
  }
  inline void SearchClipRect( float x0, float y0, float x1, float y1, const zCOLOR& c ) {
    if( x0 < g_clipX0 ) x0 = g_clipX0;
    if( y0 < g_clipY0 ) y0 = g_clipY0;
    if( x1 > g_clipX1 ) x1 = g_clipX1;
    if( y1 > g_clipY1 ) y1 = g_clipY1;
    if( x1 > x0 && y1 > y0 )
      TexRect( x0, y0, x1 - x0, y1 - y0, c );
  }
  inline void SearchRing( float px, float py, float s ) {
    g_ringsNow++;
    static DWORD s_tick = 0;
    static float s_ph   = 0.0f;
    const DWORD now = GetTickCount();
    if( now != s_tick ) {
      s_tick = now;
      const DWORD t = now % 900;
      s_ph = ( t < 450 ) ? (float)t / 450.0f : (float)( 900 - t ) / 450.0f;
    }
    float r = s * ( 0.75f + 0.30f * s_ph );
    if( r < 5.0f ) r = 5.0f;
    const float w = ( s > 10.0f ) ? 2.0f : 1.5f;
    const zCOLOR dark( 18, 16, 15, 230 );
    const zCOLOR gold( 255, 236, 90, 255 );
    const float x0 = px - r, y0 = py - r, x1 = px + r, y1 = py + r;
    if( g_ringsLast > 300 ) {
      SearchClipRect( x0, y0, x1, y0 + w, gold );
      SearchClipRect( x0, y1 - w, x1, y1, gold );
      return;
    }
    SearchClipRect( x0 - 1.0f, y0 - 1.0f, x1 + 1.0f, y0, dark );
    SearchClipRect( x0 - 1.0f, y1, x1 + 1.0f, y1 + 1.0f, dark );
    SearchClipRect( x0, y0 + w, x0 + w, y1 - w, gold );
    SearchClipRect( x1 - w, y0 + w, x1, y1 - w, gold );
    SearchClipRect( x0, y0, x1, y0 + w, gold );
    SearchClipRect( x0, y1 - w, x1, y1, gold );
  }
  inline void SearchDimMark( float px, float py, float s ) {
    float h = s * 0.22f;
    if( h < 1.5f ) h = 1.5f;
    float x0 = px - h, y0 = py - h, x1 = px + h, y1 = py + h;
    if( x0 < g_clipX0 ) x0 = g_clipX0;
    if( y0 < g_clipY0 ) y0 = g_clipY0;
    if( x1 > g_clipX1 ) x1 = g_clipX1;
    if( y1 > g_clipY1 ) y1 = g_clipY1;
    if( x1 > x0 && y1 > y0 )
      TexRect( x0, y0, x1 - x0, y1 - y0, zCOLOR( 120, 116, 108, 110 ) );
  }
  inline int ChestKind( oCMobContainer* ch ) {
    if( !ch->locked ) return ICON_CHEST;
    const bool key  = ch->keyInstance.Length() > 0;
    const bool pick = ch->pickLockStr.Length() > 0;
    return ( key && pick ) ? ICON_CHEST_BOTH : key ? ICON_CHEST_KEY
         : pick ? ICON_CHEST_PICK : ICON_CHEST_SEALED;
  }
  inline bool ChestEmptyOpen( oCMobContainer* ch ) {
    __try {
      if( !ch || ch->locked ) return false;
      int guard = 0;
      for( zCListSort<oCItem>* e = ch->containList.next; e && guard < 4096; e = e->next, guard++ )
        if( e->data ) return false;
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline void CorpseMark( float px, float py, float s ) {
    if( DrawIcon( ICON_CORPSE, px, py, s ) ) return;
    const zCOLOR dark( 18, 16, 15, 255 );
    const zCOLOR col = ColCorpse();
    const float w = ( s * 0.16f < 1.5f ) ? 1.5f : s * 0.16f;
    const float h = s * 0.45f, a = s * 0.30f, ay = py - s * 0.15f;
    TexRect( px - w - 1.0f, py - h - 1.0f, 2.0f * w + 2.0f, 2.0f * h + 2.0f, dark );
    TexRect( px - a - 1.0f, ay - w - 1.0f, 2.0f * a + 2.0f, 2.0f * w + 2.0f, dark );
    TexRect( px - w, py - h, 2.0f * w, 2.0f * h, col );
    TexRect( px - a, ay - w, 2.0f * a, 2.0f * w, col );
  }
  inline void ChestMark( int kind, float px, float py, float s ) {
    if( DrawIcon( kind, px, py, s ) ) return;
    const zCOLOR dark( 18, 16, 15, 255 );
    const float hw = s * 0.42f, hh = s * 0.30f;
    float x0 = px - hw - 1.0f, y0 = py - hh - 1.0f;
    float x1 = px + hw + 1.0f, y1 = py + hh + 1.0f;
    if( x0 < g_clipX0 ) x0 = g_clipX0;
    if( y0 < g_clipY0 ) y0 = g_clipY0;
    if( x1 > g_clipX1 ) x1 = g_clipX1;
    if( y1 > g_clipY1 ) y1 = g_clipY1;
    if( x1 - x0 < 3.0f || y1 - y0 < 3.0f ) return;
    TexRect( x0, y0, x1 - x0, y1 - y0, dark );
    TexRect( x0 + 1.0f, y0 + 1.0f, x1 - x0 - 2.0f, y1 - y0 - 2.0f, AtlasColor( kind ) );
    TexRect( x0 + 1.0f, py - 0.5f, x1 - x0 - 2.0f, 1.0f, dark );
  }
  inline void DrawChests() {
    if( !ogame || !player ) return;
    oCWorld* world = ogame->GetGameWorld();
    if( !world ) return;
    if( g_chestCache && world != g_chestWorld )
      ChestCacheFree( false );
    const DWORD now = GetTickCount();
    if( !g_chestWorld || (long)( now - g_chestNext ) >= 0 ) {
      g_chestNext = now + CHEST_CACHE_MS;
      ChestCacheRefresh( world );
      if( !g_chestWorld ) g_chestWorld = world;
    }
    if( !g_chestCache || g_chestCount <= 0 ) return;
    const bool filt = SearchFilterOn() && g_chestMatch && g_chestMatchN == g_chestCount;
    if( !filt && !g_showChests ) return;
    const float r2   = (float)g_itemRange * (float)g_itemRange;
    const float size = MarkSizePxK() * 0.85f;
    const zVEC3 pp   = player->GetPositionWorld();
    int drawn = 0;
    for( int i = 0; i < g_chestCount && drawn < 8000; i++ ) {
      zCVob* const chest = g_chestCache[i];
      if( !chest || chest->homeWorld != (zCWorld*)world ) continue;
      const bool hit = filt && g_chestMatch[i];
      if( !hit && !g_showChests ) continue;
      const zVEC3 p = chest->GetPositionWorld();
      const float dx = p[0] - pp[0], dz = p[2] - pp[2];
      if( !hit && g_showChests < SHOW_WORLD && dx * dx + dz * dz > r2 ) continue;
      float mx, my;
      WorldToPx( p[0], p[2], mx, my );
      if( mx < g_clipX0 || mx > g_clipX1 || my < g_clipY0 || my > g_clipY1 ) continue;
      if( !MarkerVisible( mx, my ) ) continue;
      if( g_hideEmpty && !hit && ChestEmptyOpen( (oCMobContainer*)chest ) ) continue;
      ChestMark( ChestKind( (oCMobContainer*)chest ), mx, my, size );
      if( hit )
        SearchRing( mx, my, size );
      HoverConsider( chest, mx, my, size, false );
      drawn++;
    }
  }
  static const DWORD SWITCH_CACHE_MS = 5000;
  static const int   SWITCH_MAX      = 4000;
  static zCVob**        g_switchCache = 0;
  static unsigned char* g_switchKind  = 0;
  static int            g_switchCount = 0;
  static DWORD          g_switchNext  = 0;
  static oCWorld*       g_switchWorld = 0;
  static bool*          g_switchMatch    = 0;
  static int            g_switchMatchCap = 0;
  static int            g_switchMatchN   = 0;
  inline bool SwitchIs( zCClassDef* base, zCClassDef* def ) {
    return base && def && zCObject::CheckInheritance( base, def );
  }
  inline int SwitchKindOf( zCVob* v ) {
    if( !v ) return -1;
    zCClassDef* const def = v->_GetClassDef();
    if( !SwitchIs( oCMobInter::classDef, def ) ) return -1;
    if( SwitchIs( oCMobContainer::classDef, def ) || SwitchIs( oCMobDoor::classDef, def )
        || SwitchIs( oCMobLadder::classDef, def ) || SwitchIs( oCMobFire::classDef, def )
        || SwitchIs( oCMobBed::classDef, def ) )
      return -1;
    if( SwitchIs( oCMobWheel::classDef, def ) ) return ICON_WHEEL;
    if( SwitchIs( oCMobSwitch::classDef, def ) ) return ICON_LEVER;
    oCMobInter* const mi = (oCMobInter*)v;
    const char* const sc = mi->sceme.ToChar();
    if( sc && sc[0] ) {
      if( StrHasUp( sc, "VWHEEL" ) ) return ICON_WHEEL;
      if( StrHasAnyUp( sc, "LEVER|TOUCHPLATE|TURNSWITCH" ) ) return ICON_LEVER;
    }
    if( mi->triggerTarget.Length() > 0 ) return ICON_TRIGGER;
    const char* const nm = mi->name.ToChar();
    if( nm && nm[0] ) {
      if( StrEqUp( nm, "MOBNAME_WHEEL" ) ) return ICON_WHEEL;
      if( StrHasAnyUp( nm, "SWITCH|LEVER" ) ) return ICON_LEVER;
      if( StrHasUp( nm, "KEYBUT" ) ) return ICON_TRIGGER;
    }
    return -1;
  }
  inline void SwitchCacheFree( bool release = true ) {
    if( g_switchCache && release ) {
      for( int i = 0; i < g_switchCount; i++ )
        if( g_switchCache[i] )
          g_switchCache[i]->Release();
    }
    MiniMapCommon::MmFree( g_switchCache );
    MiniMapCommon::MmFree( g_switchKind );
    g_switchCache = 0;
    g_switchKind  = 0;
    g_switchCount = 0;
    g_switchNext  = 0;
    g_switchWorld = 0;
  }
  inline void SwitchCacheRefresh( oCWorld* world ) {
    if( !oCMobInter::classDef ) return;
    int n = 0;
    for( zCListSort<zCVob>* node = world->voblist; node && n < SWITCH_MAX; node = node->next )
      if( node->data && node->data->homeWorld == (zCWorld*)world && SwitchKindOf( node->data ) >= 0 )
        n++;
    zCVob**        arr  = 0;
    unsigned char* kind = 0;
    int            cnt  = 0;
    if( n > 0 ) {
      arr  = MiniMapCommon::MmAlloc<zCVob*>( (size_t)( n ) );
      kind = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n ) );
      if( !arr || !kind ) {
        MiniMapCommon::MmFree( arr );
        MiniMapCommon::MmFree( kind );
        return;
      }
      for( zCListSort<zCVob>* node = world->voblist; node && cnt < n; node = node->next ) {
        zCVob* const vob = node->data;
        if( !vob || vob->homeWorld != (zCWorld*)world ) continue;
        const int k = SwitchKindOf( vob );
        if( k < 0 ) continue;
        arr[cnt]  = vob;
        kind[cnt] = (unsigned char)k;
        cnt++;
      }
    }
    for( int i = 0; i < cnt; i++ )
      arr[i]->AddRef();
    zCVob** const        old  = g_switchCache;
    unsigned char* const oldK = g_switchKind;
    const int            oldN = g_switchCount;
    g_switchCache = arr;
    g_switchKind  = kind;
    g_switchCount = cnt;
    g_switchWorld = world;
    for( int i = 0; i < oldN; i++ )
      if( old[i] )
        old[i]->Release();
    MiniMapCommon::MmFree( old );
    MiniMapCommon::MmFree( oldK );
    SearchMatchPlaces();
  }
  inline void SwitchMark( int kind, float px, float py, float s ) {
    if( DrawIcon( kind, px, py, s ) ) return;
    const float r = s * 0.42f;
    FillTriTex( px, py - r - 1.0f, px + r + 1.0f, py, px, py + r + 1.0f, zCOLOR( 18, 16, 15, 255 ) );
    FillTriTex( px, py - r - 1.0f, px - r - 1.0f, py, px, py + r + 1.0f, zCOLOR( 18, 16, 15, 255 ) );
    FillTriTex( px, py - r, px + r, py, px, py + r, AtlasColor( kind ) );
    FillTriTex( px, py - r, px - r, py, px, py + r, AtlasColor( kind ) );
  }
  inline void DrawSwitches() {
    if( !g_showSwitches || !ogame || !player ) return;
    oCWorld* world = ogame->GetGameWorld();
    if( !world ) return;
    if( g_switchCache && world != g_switchWorld )
      SwitchCacheFree( false );
    const DWORD now = GetTickCount();
    if( !g_switchWorld || (long)( now - g_switchNext ) >= 0 ) {
      g_switchNext = now + SWITCH_CACHE_MS;
      SwitchCacheRefresh( world );
      if( !g_switchWorld ) g_switchWorld = world;
    }
    if( !g_switchCache || g_switchCount <= 0 ) return;
    int limit = ( g_itemLimit == 60 ) ? 200 : g_itemLimit;
    if( g_full || g_showSwitches == SHOW_WORLD ) limit = 8000;
    const float r2   = (float)g_itemRange * (float)g_itemRange;
    const float size = MarkSizePxK() * 0.85f;
    const zVEC3 pp   = player->GetPositionWorld();
    int drawn = 0;
    for( int i = 0; i < g_switchCount && drawn < limit; i++ ) {
      zCVob* const sw = g_switchCache[i];
      if( !sw || sw->homeWorld != (zCWorld*)world ) continue;
      const zVEC3 p = sw->GetPositionWorld();
      const float dx = p[0] - pp[0], dz = p[2] - pp[2];
      if( g_showSwitches < SHOW_WORLD && dx * dx + dz * dz > r2 ) continue;
      float mx, my;
      WorldToPx( p[0], p[2], mx, my );
      if( mx < g_clipX0 || mx > g_clipX1 || my < g_clipY0 || my > g_clipY1 ) continue;
      if( !MarkerVisible( mx, my ) ) continue;
      SwitchMark( g_switchKind[i], mx, my, size );
      if( SearchFilterOn() && g_switchMatch && g_switchMatchN == g_switchCount && g_switchMatch[i] )
        SearchRing( mx, my, size );
      HoverConsider( sw, mx, my, size, false );
      drawn++;
    }
  }
  static const DWORD PLACE_CACHE_MS = 5000;
  static const int   PLACE_MAX      = 8000;
  static zCVob**        g_placeCache = 0;
  static unsigned char* g_placeKind  = 0;
  static int            g_placeCount = 0;
  static DWORD          g_placeNext  = 0;
  static oCWorld*       g_placeWorld = 0;
  static bool*          g_placeMatch    = 0;
  static int            g_placeMatchCap = 0;
  static int            g_placeMatchN   = 0;
  inline const char* PlaceKindName( int kind );
  inline bool PlaceWordHit( const char* s, const char* needle );
  inline void SearchSuggestPlaces() {
    if( !g_searchNorm[0] ) return;
    for( int pass = 0; pass < 2; pass++ ) {
      zCVob** const        arr = pass ? g_switchCache : g_placeCache;
      unsigned char* const kval  = pass ? g_switchKind : g_placeKind;
      bool* const          mt  = pass ? g_switchMatch : g_placeMatch;
      const int            n   = pass ? g_switchCount : g_placeCount;
      const int            mn  = pass ? g_switchMatchN : g_placeMatchN;
      if( !arr || !mt || mn != n ) continue;
      for( int i = 0; i < n; i++ ) {
        if( !mt[i] || !arr[i] ) continue;
        zSTRING nm = ( (oCMOB*)arr[i] )->GetName();
        if( nm.Length() > 0 && PlaceWordHit( nm.ToChar(), g_searchNorm ) )
          SearchSuggestAdd( nm, 1 );
        else {
          const int k = kval[i];
          SearchSuggestAdd( zSTRING( k == ICON_LEVER ? T( TX_HOV_SW_LEVER ) : k == ICON_WHEEL ? T( TX_HOV_SW_WHEEL )
                                     : k == ICON_TRIGGER ? T( TX_HOV_SW_TRIGGER ) : PlaceKindName( k ) ), 1 );
        }
      }
    }
  }
  inline bool PlaceWord( const char* s, const char* w ) {
    return s && s[0] && GenHasWord( s, w, true, true );
  }
  inline bool PlaceIsRest( int kind ) {
    return ( kind >= ICON_BED && kind <= ICON_FIRE ) || kind == PLACE_REST;
  }
  inline int PlaceKindOfText( const char* s ) {
    if( !s || !s[0] ) return -1;
    if( StrHasAnyUp( s, "DOOR|LADDER" ) || PlaceWord( s, "NOTHING" ) ) return -2;
    if( StrHasAnyUp( s, "INNOS|ADANOS|BELIAR|PRAY|IDOL|SHRINE|ALTAR|STATUE" ) ) return ICON_SHRINE;
    if( StrHasAnyUp( s, "NEWSBOARD|BOOKSBOARD|BOOKSTAND|BOOK" ) || PlaceWord( s, "SIGN" ) || StrHasAnyUp( s, "SIGN_|NOTICE|STONE_PLATE|STONEPLATE" ) ) return ICON_BOARD;
    if( StrHasAnyUp( s, "REPAIR|STONEMILL|STOMPER_OM" )
        || PlaceWord( s, "STOMPER" ) )
      return PLACE_OTHER;
    if( PlaceWord( s, "SAW" ) || StrHasAnyUp( s, "BOWMAKING|VERSTAK|MAKEBOWS|MAKEARROWS|TANNING|SKINMAKE|SKINRAPE|SKINLAB|GOLDMAKE|MAKEGOLD|BSGOLD|GOLDSMITH|JEWEL|JEWERLY|MAPBOARD|KARTHOGRAPH|WINEMAKER" ) ) return ICON_WORKSHOP;
    if( StrHasAnyUp( s, "RUNEMAKER|MAKERUNE|RMAKER|TRFMAGIC" ) ) return ICON_RUNE;
    if( StrHasAnyUp( s, "SCROLLWRITING|SCROLL_WRITING|SCROLLBOARD|MAKESCROLL|SCHREIBPULT" ) ) return ICON_SCROLL;
    if( StrHasAnyUp( s, "DEMONCASTER|ENCHANT" ) ) return ICON_DEMON;
    if( PlaceWord( s, "LAB" ) || StrHasAnyUp( s, "LABOR|ALCHEM|POTION|HERBSTOMPER|HERB_" ) ) return ICON_ALCHEMY;
    if( StrHasUp( s, "STOVE" ) || PlaceWord( s, "PAN" ) || StrHasAnyUp( s, "HERDPAN|CAULDRON|KESSEL|COOK|BARBQ|BBQ" ) || PlaceWord( s, "HERW" ) || PlaceWord( s, "HERD" )
        || StrHasUp( s, "BAKE_" ) )
      return ICON_KITCHEN;
    if( StrHasAnyUp( s, "ANVIL|FORGE|BSFIRE|BSCOOL|BSSHARP|GRINDSTONE|SMITH|BELLOW|BSWBENCH|COOLBLADE|SHARPWEAPON" ) ) return ICON_CRAFT;
    if( StrHasUp( s, "HERB" ) ) return ICON_ALCHEMY;
    if( StrHasUp( s, "STOMPER" ) ) return PLACE_OTHER;
    if( StrHasAnyUp( s, "TREASURE|SCHATZ" ) || PlaceWord( s, "DIG" )
        || StrHasAnyUp( s, "DIGGRAVE|BURIED" ) )
      return ICON_TREASURE;
    if( PlaceWord( s, "ORE" ) || StrHasAnyUp( s, "ORE_|IRONMOB|SILVERMOB|GOLDMOB|ADDON_GOLD|SALTORE|OREMMOB|WOODCUT|VINEYARD|PICKORE|MINING|PICKAXE|PLUNDER|NUGGET|HARVEST" )
        || PlaceWord( s, "CHOOP" ) || StrHasAnyUp( s, "WOODCHOOP|TREEFALING|TREE_FALL" ) )
      return ICON_ORE;
    if( StrHasAnyUp( s, "BED|HAMMOCK|SLEEPABIT" ) ) return ICON_BED;
    if( StrHasAnyUp( s, "THRONE|ARMCHAIR" ) ) return ICON_THRONE;
    if( StrHasAnyUp( s, "BENCH|SOFA" ) ) return ICON_BENCH;
    if( StrHasAnyUp( s, "CHAIR|STOOL" ) ) return ICON_CHAIR;
    if( StrHasAnyUp( s, "FIREPLACE|CRYSTALLIGHT" ) ) return ICON_FIRE;
    if( StrHasUp( s, "WATERPIPE" ) ) return PLACE_REST;
    return -1;
  }
  inline int PlaceKindOfWords( zCVob* v, bool* fallback = 0 ) {
    if( fallback ) *fallback = false;
    if( !v || !oCMobInter::classDef ) return -1;
    zCClassDef* const def = v->_GetClassDef();
    if( !SwitchIs( oCMobInter::classDef, def ) ) return -1;
    if( SwitchIs( oCMobLadder::classDef, def ) ) return ICON_LADDER;
    if( SwitchIs( oCMobContainer::classDef, def ) ) return -1;
    const bool door = SwitchIs( oCMobDoor::classDef, def );
    if( SwitchKindOf( v ) >= 0 ) return -1;
    oCMobInter* const mi = (oCMobInter*)v;
    const bool noShape = PlaceWord( mi->onStateFuncName.ToChar(), "WELL" )
                      || PlaceWord( mi->name.ToChar(), "WELL" )
                      || StrHasUp( mi->name.ToChar(), "DEADBODY" );
    int k = PlaceKindOfText( mi->name.ToChar() );
    if( k == -1 && !noShape ) k = PlaceKindOfText( mi->sceme.ToChar() );
    if( k == -1 ) k = PlaceKindOfText( mi->onStateFuncName.ToChar() );
    if( k == -1 && v->visual && !noShape ) {
      zSTRING vn = v->visual->GetVisualName();
      k = PlaceKindOfText( vn.ToChar() );
    }
    if( k == -1 && StrHasUp( mi->useWithItem.ToChar(), "AXE" )
        && StrHasUp( mi->onStateFuncName.ToChar(), "MIN" ) )
      k = ICON_ORE;
    if( k == -1 && ( StrHasUp( mi->useWithItem.ToChar(), "SHOVEL" )
                     || StrHasUp( mi->useWithItem.ToChar(), "SCHAUFEL" ) ) )
      k = ICON_TREASURE;
    if( k == -1 && StrHasUp( mi->useWithItem.ToChar(), "MUTTONRAW" ) ) k = ICON_KITCHEN;
    if( k == -2 ) return -1;
    if( k == -1 && noShape ) return ( mi->name.Length() > 0 ) ? PLACE_OTHER : PLACE_REST;
    if( door ) return ( k == ICON_BED ) ? ICON_BED : -1;
    if( k >= 0 && k != PLACE_REST ) return k;
    if( SwitchIs( oCMobFire::classDef, def ) ) return ICON_FIRE;
    if( SwitchIs( oCMobBed::classDef, def ) ) return ICON_BED;
    if( k == PLACE_REST ) return PLACE_REST;
    if( fallback ) *fallback = true;
    return ( mi->name.Length() > 0 ) ? PLACE_OTHER : PLACE_REST;
  }
  static PlaceLearn::TPlState* g_plState  = 0;
  static PlaceLearn::TPlWork*  g_plWork   = 0;
  static void*                 g_plParser = 0;
  static void*                 g_plInfo   = 0;
  static int                   g_plLogged = -1;
  enum { PL_FNCACHE = 1024 };
  static unsigned int g_plFnHash[PL_FNCACHE];
  static int          g_plFnIdx[PL_FNCACHE];
  static int          g_plFnN = 0;
  inline bool PlSymOf( void*, int index, PlaceLearn::TPlSym& out ) {
    zCPar_Symbol* const s = parser ? parser->GetSymbol( index ) : 0;
    if( !s ) return false;
    out.type       = (unsigned char)s->type;
    out.isConst    = ( s->flags & zPAR_FLAG_CONST ) != 0;
    out.isExternal = ( s->flags & zPAR_FLAG_EXTERNAL ) != 0;
    out.ele        = (int)s->ele;
    const char* nm = s->name.ToChar();
    out.dotted = nm && strchr( nm, '.' ) != 0;
    out.value = ( out.type == zPAR_TYPE_FUNC || out.type == zPAR_TYPE_INSTANCE
                  || ( out.type == zPAR_TYPE_INT && out.isConst && out.ele == 1 ) )
              ? s->single_intdata : 0;
    return true;
  }
  static PlaceLearn::TPlProdWork* g_plProd = 0;
  inline int PlBuildCore( int* idx, int cap, int* cond, int* info, int infoCap, int cr, int rm,
                          const int* ext, int mainf, int* fsym, int fcap ) {
    __try {
      unsigned char* const code = parser->stack.stack;
      const int size = parser->stack.stacksize;
      if( !code || size <= 0 ) return -1;
      const int n = parser->symtab.table.GetNum();
      int m = 0;
      for( int k = 0; k < n && m < cap; k++ ) {
        zCPar_Symbol* const s = parser->symtab.table[k];
        if( !s || s->type != zPAR_TYPE_FUNC || ( s->flags & zPAR_FLAG_EXTERNAL ) ) continue;
        const char* nm = s->name.ToChar();
        if( nm && MiniMapCfg::EndsNoCase( nm, (int)strlen( nm ), "_S1" ) ) idx[m++] = k;
      }
      PlaceLearn::PlBuildFuncs( *g_plState, code, size, 0, PlSymOf, idx, m );
      int ni = 0;
      oCInfoManager* const im = ogame ? ogame->infoman : 0;
      if( im ) {
        int guard = 0;
        for( zCListSort<oCInfo>* e = im->infoList.next; e && ni < infoCap && guard < 200000; e = e->next, guard++ )
          if( e->data && e->data->pd.conditions > 0 ) {
            cond[ni] = e->data->pd.conditions;
            info[ni] = e->data->pd.information;
            ni++;
          }
      }
      PlaceLearn::PlBuildRecipes( *g_plState, code, size, 0, PlSymOf, cond, info, ni, cr, rm );
      if( g_plProd && fsym ) {
        int nf = 0;
        for( int k = 0; k < n && nf < fcap; k++ ) {
          zCPar_Symbol* const s = parser->symtab.table[k];
          if( s && s->type == zPAR_TYPE_FUNC && !( s->flags & zPAR_FLAG_EXTERNAL ) && ( s->flags & zPAR_FLAG_CONST )
              && s->ele >= 2 && s->ele <= 8 )
            fsym[nf++] = k;
        }
        PlaceLearn::PlBuildProduce( *g_plState, *g_plProd, code, size, 0, PlSymOf, ext, mainf,
                                    fsym, nf, cond, info, ni );
      }
      return ni;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return -2;
    }
  }
  inline void PlEnsure() {
    if( !parser ) return;
    void* const im = ogame ? (void*)ogame->infoman : 0;
    if( g_plState && g_plParser == (void*)parser && g_plInfo == im ) return;
    if( !g_plState ) g_plState = MiniMapCommon::MmAlloc<PlaceLearn::TPlState>( 1 );
    if( !g_plWork )  g_plWork  = MiniMapCommon::MmAlloc<PlaceLearn::TPlWork>( 1 );
    if( !g_plProd )  g_plProd  = MiniMapCommon::MmAlloc<PlaceLearn::TPlProdWork>( 1 );
    if( !g_plState || !g_plWork ) return;
    g_plParser = (void*)parser;
    g_plInfo   = im;
    g_plFnN    = 0;
    g_plLogged = -1;
    const int cr = parser->GetIndex( zSTRING( "CREATEINVITEMS" ) );
    const int rm = parser->GetIndex( zSTRING( "NPC_REMOVEINVITEMS" ) );
    const int ext[4] = { cr, parser->GetIndex( zSTRING( "CREATEINVITEM" ) ), rm,
                         parser->GetIndex( zSTRING( "NPC_REMOVEINVITEM" ) ) };
    const int mainf = parser->GetIndex( zSTRING( "C_ITEM.MAINFLAG" ) );
    const int INFO_CAP = 40000;
    const int FSYM_CAP = 60000;
    int*  idx  = MiniMapCommon::MmAlloc<int>( (size_t)( PlaceLearn::PL_FUNCS ) );
    int*  cond = MiniMapCommon::MmAlloc<int>( (size_t)( INFO_CAP ) );
    int*  info = MiniMapCommon::MmAlloc<int>( (size_t)( INFO_CAP ) );
    int*  fsym = MiniMapCommon::MmAlloc<int>( (size_t)( FSYM_CAP ) );
    int dialogs = -3;
    if( idx && cond && info )
      dialogs = PlBuildCore( idx, PlaceLearn::PL_FUNCS, cond, info, INFO_CAP, cr, rm,
                             ext, mainf, fsym, fsym ? FSYM_CAP : 0 );
    MiniMapCommon::MmFree( idx );
    MiniMapCommon::MmFree( cond );
    MiniMapCommon::MmFree( info );
    MiniMapCommon::MmFree( fsym );
    if( dialogs < 0 ) {
      g_plState->ready = false;
      MM_LOG( zSTRING( "places: scripts not readable for learning (" ) + zSTRING( dialogs ) + ")" );
      return;
    }
    int recipeKeys = 0;
    for( int i = 0; i < g_plState->nPairs; i++ )
      if( g_plState->pairRecipes[i] >= 4 ) recipeKeys++;
    int prodFuncs = 0;
    for( int i = 0; i < g_plState->nFuncs; i++ )
      if( g_plState->fprod[i] ) prodFuncs++;
    MM_LOG( zSTRING( "places: learning from scripts - mob functions " ) + zSTRING( g_plState->nFuncs ) + ", menu variables " + zSTRING( g_plState->nMenu ) + ", dialogues " + zSTRING( dialogs ) + " (recipes " + zSTRING( g_plState->recipeDialogs ) + "), workbench menus with recipes " + zSTRING( recipeKeys ) + ", limits hit " + zSTRING( g_plState->truncated ) );
    MM_LOG( zSTRING( "places: production network - item helpers " ) + zSTRING( g_plState->nWraps ) + ", crafted items " + zSTRING( g_plState->nInst ) + ", recipe entries " + zSTRING( g_plState->nRec ) + ", mob functions with a product kind " + zSTRING( prodFuncs ) );
  }
  inline int PlFuncOf( oCMobInter* mi ) {
    if( !g_plState || !g_plState->ready || !parser ) return -1;
    const char* fn = mi->onStateFuncName.ToChar();
    if( !fn || !*fn ) return -1;
    const unsigned int h = PlaceLearn::PlHash( fn );
    for( int i = 0; i < g_plFnN; i++ )
      if( g_plFnHash[i] == h ) return g_plFnIdx[i];
    char buf[96];
    snprintf( buf, sizeof( buf ), "%.90s_S1", fn );
    _strupr_s( buf, sizeof( buf ) );
    const int sym = parser->GetIndex( zSTRING( buf ) );
    const int fi  = sym >= 0 ? PlaceLearn::PlFind( *g_plState, sym ) : -1;
    if( g_plFnN < PL_FNCACHE ) {
      g_plFnHash[g_plFnN] = h;
      g_plFnIdx[g_plFnN]  = fi;
      g_plFnN++;
    }
    return fi;
  }
  inline int PlaceKindLearned( zCVob* v ) {
    if( !g_plState || !g_plState->ready ) return 0;
    oCMobInter* const mi = (oCMobInter*)v;
    PlaceLearn::TPlMob m;
    m.func     = PlFuncOf( mi );
    m.item     = PlaceLearn::PlHash( mi->useWithItem.ToChar() );
    m.named    = mi->name.Length() > 0;
    m.fallback = true;
    m.kind     = 0;
    return PlaceLearn::PlLearned( *g_plState, m );
  }
  enum { PW_MAX = 512, PW_LEN = MiniMapCfg::KEY_LEN };
  static char g_pwWord[PW_MAX][PW_LEN];
  static int  g_pwKind[PW_MAX];
  static int  g_pwN   = 0;
  static int  g_pwGen = -1;
  struct TPlaceWordKey { const char* key; int kind; };
  static const TPlaceWordKey kPlaceWordKind[] = {
    { "shrine", ICON_SHRINE }, { "board", ICON_BOARD }, { "forge", ICON_CRAFT }, { "ore", ICON_ORE },
    { "treasure", ICON_TREASURE }, { "bed", ICON_BED }, { "chair", ICON_CHAIR }, { "bench", ICON_BENCH },
    { "throne", ICON_THRONE }, { "fire", ICON_FIRE }, { "alchemy", ICON_ALCHEMY }, { "kitchen", ICON_KITCHEN },
    { "workshop", ICON_WORKSHOP }, { "ladder", ICON_LADDER }, { "rune", ICON_RUNE }, { "scroll", ICON_SCROLL },
    { "demon", ICON_DEMON }, { "other", PLACE_OTHER }, { "none", -2 }
  };
  inline void PlaceWordsEnsure() {
    if( g_pwGen == g_cfgGen ) return;
    g_pwGen = g_cfgGen;
    g_pwN   = 0;
    const int sec = g_modFiles ? MiniMapCfg::SecFind( g_cfg, "place_words" ) : -1;
    int bad = 0;
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      int kind = -1;
      for( int k = 0; k < (int)( sizeof( kPlaceWordKind ) / sizeof( kPlaceWordKind[0] ) ); k++ )
        if( MiniMapCfg::EqNoCase( kPlaceWordKind[k].key, t.val ) ) kind = kPlaceWordKind[k].kind;
      if( kind == -1 ) {
        CfgMsg( 0, t.src, t.line, "[place_words]: unknown kind (shrine board forge ore treasure bed chair bench throne fire alchemy kitchen workshop ladder rune scroll demon other none) - ignored" );
        bad++;
        continue;
      }
      const int wl = (int)strlen( t.key );
      if( wl < 3 || strchr( t.key, '*' ) ) {
        CfgMsg( 0, t.src, t.line, "[place_words]: word must have at least 3 letters (no '*') - ignored" );
        bad++;
        continue;
      }
      if( g_pwN >= PW_MAX ) {
        CfgMsg( 0, t.src, t.line, "[place_words]: more than 512 words - the rest is ignored" );
        bad++;
        break;
      }
      MiniMapCfg::Copy( g_pwWord[g_pwN], PW_LEN, t.key );
      g_pwKind[g_pwN] = kind;
      g_pwN++;
    }
    if( g_pwN || bad )
      MM_LOG( zSTRING( "places: words from txt files " ) + zSTRING( g_pwN ) + ", bad lines " + zSTRING( bad ) );
  }
  inline int PlaceWordsTxt( const char* s ) {
    if( !s || !*s || g_pwN <= 0 ) return -1;
    int best = -1, bestLen = 0;
    for( int i = 0; i < g_pwN; i++ ) {
      const int wl = (int)strlen( g_pwWord[i] );
      if( wl > bestLen && StrHasUp( s, g_pwWord[i] ) ) {
        best    = g_pwKind[i];
        bestLen = wl;
      }
    }
    return best;
  }
  inline int PlaceKindTxt( zCVob* v ) {
    PlaceWordsEnsure();
    if( g_pwN <= 0 || !v || !oCMobInter::classDef ) return -1;
    zCClassDef* const def = v->_GetClassDef();
    if( !SwitchIs( oCMobInter::classDef, def ) || SwitchIs( oCMobContainer::classDef, def )
        || SwitchKindOf( v ) >= 0 )
      return -1;
    const bool door = SwitchIs( oCMobDoor::classDef, def );
    oCMobInter* const mi = (oCMobInter*)v;
    int k = PlaceWordsTxt( mi->name.ToChar() );
    if( k == -1 ) k = PlaceWordsTxt( mi->sceme.ToChar() );
    if( k == -1 ) k = PlaceWordsTxt( mi->onStateFuncName.ToChar() );
    if( k == -1 && v->visual ) {
      zSTRING vn = v->visual->GetVisualName();
      k = PlaceWordsTxt( vn.ToChar() );
    }
    if( door && k != ICON_BED && k != -2 ) return -1;
    return k;
  }
  inline int PlaceKindOf( zCVob* v ) {
    const int t = PlaceKindTxt( v );
    if( t == -2 ) return -1;
    if( t >= 0 ) return t;
    bool fb = false;
    const int k = PlaceKindOfWords( v, &fb );
    if( fb && WtpIsPlatform( v ) ) return -1;
    if( fb && g_placeLearn ) {
      const int l = PlaceKindLearned( v );
      if( l > 0 ) return l;
    }
    return k;
  }
  inline void PlaceLearnWorld( zCVob** arr, int cnt ) {
    if( !g_placeLearn ) return;
    PlEnsure();
    if( !g_plState || !g_plState->ready || !g_plWork || cnt <= 0 ) return;
    PlaceLearn::TPlMob* mobs = MiniMapCommon::MmAlloc<PlaceLearn::TPlMob>( (size_t)( cnt ) );
    if( !mobs ) return;
    int n = 0;
    for( int i = 0; i < cnt; i++ ) {
      zCVob* const v = arr[i];
      if( !v ) continue;
      bool fb = false;
      const int k = PlaceKindOfWords( v, &fb );
      if( k < 0 ) continue;
      oCMobInter* const mi = (oCMobInter*)v;
      PlaceLearn::TPlMob& m = mobs[n++];
      m.func     = PlFuncOf( mi );
      m.item     = PlaceLearn::PlHash( mi->useWithItem.ToChar() );
      m.named    = mi->name.Length() > 0;
      m.fallback = fb;
      m.kind     = (unsigned char)( ( k == ICON_RUNE || k == ICON_SCROLL || k == ICON_DEMON ) ? ICON_ALCHEMY : k );
    }
    const int now = PlaceLearn::PlLearnWorld( *g_plState, *g_plWork, mobs, n );
    MiniMapCommon::MmFree( mobs );
    const int total = PlaceLearn::PlLearnedTotal( *g_plState );
    if( now > 0 || g_plLogged < 0 ) {
      g_plLogged = total;
      MM_LOG( zSTRING( "places: learned from scripts " ) + zSTRING( total ) + " mob functions/tools (new in this world " + zSTRING( now ) + ")" );
    }
  }
  inline bool PlaceIsFurniture( int kind ) {
    return kind >= ICON_BED && kind <= ICON_THRONE;
  }
  inline bool PlaceIsClutter( int kind ) {
    return kind == PLACE_OTHER || kind == PLACE_REST;
  }
  inline bool PlaceKindEnabled( int kind ) {
    if( PlaceIsFurniture( kind ) ) return g_showFurniture != 0;
    if( kind == ICON_FIRE )        return g_showFire != 0;
    if( PlaceIsClutter( kind ) )   return g_showClutter != 0;
    if( kind == ICON_CRAFT )       return g_showCraft != 0;
    if( kind == ICON_ALCHEMY )     return g_showAlchemy != 0;
    if( kind == ICON_RUNE )        return g_showRunes != 0;
    if( kind == ICON_SCROLL )      return g_showScrolls != 0;
    if( kind == ICON_DEMON )       return g_showDemon != 0;
    if( kind == ICON_KITCHEN )     return g_showKitchen != 0;
    if( kind == ICON_WORKSHOP )    return g_showWorkshop != 0;
    if( kind == ICON_LADDER )      return g_showLadders != 0;
    return true;
  }
  inline bool PlaceKindShown( int kind, int show ) {
    if( show <= 0 ) return false;
    return PlaceKindEnabled( kind );
  }
  inline const char* PlaceKindName( int kind ) {
    switch( kind ) {
      case ICON_SHRINE:   return T( TX_HOV_PL_SHRINE );
      case ICON_BOARD:    return T( TX_HOV_PL_BOARD );
      case ICON_CRAFT:    return T( TX_HOV_PL_CRAFT );
      case ICON_ALCHEMY:  return T( TX_HOV_PL_ALCHEMY );
      case ICON_RUNE:     return T( TX_HOV_PL_RUNE );
      case ICON_SCROLL:   return T( TX_HOV_PL_SCROLL );
      case ICON_DEMON:    return T( TX_HOV_PL_DEMON );
      case ICON_KITCHEN:  return T( TX_HOV_PL_KITCHEN );
      case ICON_WORKSHOP: return T( TX_HOV_PL_WORKSHOP );
      case ICON_LADDER:   return T( TX_HOV_PL_LADDER );
      case ICON_ORE:      return T( TX_HOV_PL_ORE );
      case ICON_TREASURE: return T( TX_HOV_PL_TREASURE );
      case ICON_BED:      return T( TX_HOV_PL_BED );
      case ICON_CHAIR:    return T( TX_HOV_PL_CHAIR );
      case ICON_BENCH:    return T( TX_HOV_PL_BENCH );
      case ICON_THRONE:   return T( TX_HOV_PL_THRONE );
      case ICON_FIRE:     return T( TX_HOV_PL_FIRE );
      case PLACE_REST:    return T( TX_HOV_PL_REST );
    }
    return T( TX_HOV_PL_OTHER );
  }
  inline const char* PlaceKindWords( int kind ) {
    switch( kind ) {
      case ICON_SHRINE: return "kapliczka posag oltarz modlitwa innos adanos beliar shrine statue altar prayer schrein statue altar gebet";
      case ICON_BOARD:  return "tablica ogloszen ksiega pulpit ksiazka board notice book lectern tafel brett buch pult";
      case ICON_CRAFT:  return "rzemioslo kuznia kowadlo palenisko szlifierka oselka wiadro miech craft smithy anvil forge grindstone bellows handwerk schmiede amboss esse schleifstein blasebalg";
      case ICON_ALCHEMY: return "rzemioslo alchemia stol alchemiczny mikstury ziola ubijak magia craft alchemy potion herb magic handwerk alchemie trank kraut magie";
      case ICON_RUNE:    return "rzemioslo runy runa stol runiczny magia przemiana craft rune runes runic table magic handwerk rune runentisch magie";
      case ICON_SCROLL:  return "rzemioslo zwoje zwoj pulpit pisanie magia craft scroll scrolls lectern writing magic handwerk schriftrolle pult schreiben magie";
      case ICON_DEMON:   return "demonologia demon demony przyzywanie magia demonology demon summoning magic daemonologie daemon beschwoerung magie";
      case ICON_KITCHEN: return "rzemioslo kuchnia kuchenka piec kociol patelnia rozen ognisko pieczenie wino craft kitchen stove oven cauldron pan spit campfire roast wine handwerk kueche herd ofen kessel pfanne spiess lagerfeuer braten wein";
      case ICON_WORKSHOP: return "rzemioslo warsztat pila strzaly luki garbarz skora zlotnik zloto kartograf mapa wino craft workshop saw arrows bows tanner leather goldsmith cartographer wine handwerk werkstatt saege pfeile boegen gerber leder goldschmied kartograph wein";
      case ICON_LADDER: return "drabina drabiny wspinaczka ladder climb leiter klettern";
      case ICON_ORE:    return "zloze ruda kopanie kopalnia zelazo zloto srebro sol drewno pieniek drzewo winorosl ore vein mining iron gold silver salt wood stump tree erz ader eisen silber salz holz baum";
      case ICON_TREASURE: return "skarb skarby zakopany kopiec grob lopata kopanie wykopac greg treasure buried dig shovel grave schatz vergraben schaufel grab graben";
      case ICON_BED:    return "lozko lozka hamak spanie sen bed hammock sleep bett haengematte schlafen";
      case ICON_CHAIR:  return "krzeslo taboret siedzenie chair stool seat stuhl hocker sitz";
      case ICON_BENCH:  return "lawka sofa kanapa siedzenie bench sofa couch seat bank sofa sitz";
      case ICON_THRONE: return "fotel tron siedzenie armchair throne seat sessel thron sitz";
      case ICON_FIRE:   return "ognisko ogien pochodnia lampa swiatlo fire campfire torch lamp light feuer lagerfeuer fackel lampe licht";
      case ICON_LEVER:  return "dzwignia przelacznik lever switch hebel schalter";
      case ICON_WHEEL:  return "kolowrot wheel winch kurbelrad winde";
      case ICON_TRIGGER: return "plyta przycisk mechanizm plate button trigger platte knopf";
      case PLACE_REST:  return "inne fajka wodna other waterpipe sonstiges wasserpfeife";
    }
    return "interakcja inne deski naprawa mlyn ubijak interaction other planks repair mill stomper interaktion reparatur muehle";
  }
  inline bool PlaceWordHit( const char* s, const char* needle ) {
    if( !s || !needle || !needle[0] ) return false;
    bool start = true;
    for( ; *s; s++ ) {
      const unsigned char c = (unsigned char)*s;
      if( c == ' ' || c == '/' || c == '-' || c == '_' || c == '(' || c == ',' || c < 32 ) {
        start = true;
        continue;
      }
      if( start ) {
        const char* h = s;
        const char* b = needle;
        while( *b && *h && (char)SearchFold( (unsigned char)*h ) == *b ) { h++; b++; }
        if( !*b ) return true;
      }
      start = false;
    }
    return false;
  }
  inline bool SearchMobHit( zCVob* v, int kind ) {
    if( !v || ( g_searchCat != SCAT_ALL && g_searchCat != SCAT_PLACES ) ) return false;
    if( !g_searchNorm[0] ) return g_searchCat == SCAT_PLACES && kind >= 0;
    int len = (int)strlen( g_searchNorm );
    if( len < 2 ) return false;
    zSTRING nm = ( (oCMOB*)v )->GetName();
    if( PlaceWordHit( nm.ToChar(), g_searchNorm ) ) return true;
    if( len < 3 ) return false;
    if( kind < 0 ) {
      char cn[64];
      SearchNormalize( T( TX_HOV_CHEST ), cn, 64 );
      return PlaceWordHit( cn, g_searchNorm )
          || PlaceWordHit( "skrzynia kufer pojemnik chest container truhe kiste", g_searchNorm );
    }
    char cat[64];
    SearchNormalize( kind == ICON_LEVER ? T( TX_HOV_SW_LEVER ) : kind == ICON_WHEEL ? T( TX_HOV_SW_WHEEL )
                     : kind == ICON_TRIGGER ? T( TX_HOV_SW_TRIGGER ) : PlaceKindName( kind ), cat, 64 );
    return PlaceWordHit( cat, g_searchNorm ) || PlaceWordHit( PlaceKindWords( kind ), g_searchNorm );
  }
  inline void SearchMatchPlaces() {
    g_placeMatchN  = 0;
    g_switchMatchN = 0;
    int found = 0;
    if( SearchHasFilter() ) {
      if( g_placeCache && g_placeCount > 0 && SearchEnsure( g_placeMatch, g_placeMatchCap, g_placeCount ) ) {
        for( int i = 0; i < g_placeCount; i++ ) {
          const bool hit = SearchMobHit( g_placeCache[i], g_placeKind[i] );
          g_placeMatch[i] = hit;
          if( hit ) found++;
        }
        g_placeMatchN = g_placeCount;
      }
      if( g_switchCache && g_switchCount > 0 && SearchEnsure( g_switchMatch, g_switchMatchCap, g_switchCount ) ) {
        for( int i = 0; i < g_switchCount; i++ ) {
          const bool hit = SearchMobHit( g_switchCache[i], g_switchKind[i] );
          g_switchMatch[i] = hit;
          if( hit ) found++;
        }
        g_switchMatchN = g_switchCount;
      }
    }
    g_searchPlaceFound = found;
  }
  inline void PlaceCacheFree( bool release = true ) {
    if( g_placeCache && release ) {
      for( int i = 0; i < g_placeCount; i++ )
        if( g_placeCache[i] )
          g_placeCache[i]->Release();
    }
    MiniMapCommon::MmFree( g_placeCache );
    MiniMapCommon::MmFree( g_placeKind );
    g_placeCache  = 0;
    g_placeKind   = 0;
    g_placeCount  = 0;
    g_placeNext   = 0;
    g_placeWorld  = 0;
    g_placeMatchN = 0;
    g_switchMatchN = 0;
    g_searchPlaceFound = 0;
  }
  inline void PlaceCacheRefresh( oCWorld* world ) {
    if( !oCMobInter::classDef ) return;
    if( g_placeLearn ) {
      int nm = 0;
      for( zCListSort<zCVob>* node = world->voblist; node && nm < PLACE_MAX; node = node->next )
        if( node->data && node->data->homeWorld == (zCWorld*)world
            && SwitchIs( oCMobInter::classDef, node->data->_GetClassDef() ) )
          nm++;
      zCVob** tmp = nm > 0 ? MiniMapCommon::MmAlloc<zCVob*>( (size_t)( nm ) ) : 0;
      if( tmp ) {
        int c = 0;
        for( zCListSort<zCVob>* node = world->voblist; node && c < nm; node = node->next )
          if( node->data && node->data->homeWorld == (zCWorld*)world
              && SwitchIs( oCMobInter::classDef, node->data->_GetClassDef() ) )
            tmp[c++] = node->data;
        PlaceLearnWorld( tmp, c );
        MiniMapCommon::MmFree( tmp );
      }
    }
    int n = 0;
    for( zCListSort<zCVob>* node = world->voblist; node && n < PLACE_MAX; node = node->next )
      if( node->data && node->data->homeWorld == (zCWorld*)world
          && SwitchIs( oCMobInter::classDef, node->data->_GetClassDef() ) )
        n++;
    zCVob**        arr  = 0;
    unsigned char* kind = 0;
    int            cnt  = 0;
    if( n > 0 ) {
      arr  = MiniMapCommon::MmAlloc<zCVob*>( (size_t)( n ) );
      kind = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n ) );
      if( !arr || !kind ) {
        MiniMapCommon::MmFree( arr );
        MiniMapCommon::MmFree( kind );
        return;
      }
      for( zCListSort<zCVob>* node = world->voblist; node && cnt < n; node = node->next ) {
        zCVob* const vob = node->data;
        if( !vob || vob->homeWorld != (zCWorld*)world ) continue;
        const int k = PlaceKindOf( vob );
        if( k < 0 ) continue;
        arr[cnt]  = vob;
        kind[cnt] = (unsigned char)k;
        cnt++;
      }
    }
    for( int i = 0; i < cnt; i++ )
      arr[i]->AddRef();
    zCVob** const        old  = g_placeCache;
    unsigned char* const oldK = g_placeKind;
    const int            oldN = g_placeCount;
    g_placeCache = arr;
    g_placeKind  = kind;
    g_placeCount = cnt;
    g_placeWorld = world;
    for( int i = 0; i < oldN; i++ )
      if( old[i] )
        old[i]->Release();
    MiniMapCommon::MmFree( old );
    MiniMapCommon::MmFree( oldK );
    SearchMatchPlaces();
    SearchSuggestBuild();
    static int loggedN = -1;
    if( loggedN != cnt ) {
      loggedN = cnt;
      int byKind[64] = { 0 };
      for( int i = 0; i < cnt; i++ )
        if( kind[i] < 64 ) byKind[kind[i]]++;
      MM_LOG( zSTRING( "places: " ) + zSTRING( cnt ) + " (shrines " + zSTRING( byKind[ICON_SHRINE] ) + ", boards " + zSTRING( byKind[ICON_BOARD] ) + ", crafting " + zSTRING( byKind[ICON_CRAFT] ) + ", alchemy " + zSTRING( byKind[ICON_ALCHEMY] ) + ", runes " + zSTRING( byKind[ICON_RUNE] ) + ", scrolls " + zSTRING( byKind[ICON_SCROLL] ) + ", demonology " + zSTRING( byKind[ICON_DEMON] ) + ", kitchen " + zSTRING( byKind[ICON_KITCHEN] ) + ", workshop " + zSTRING( byKind[ICON_WORKSHOP] ) + ", ores " + zSTRING( byKind[ICON_ORE] ) + ", treasures " + zSTRING( byKind[ICON_TREASURE] ) + ", other " + zSTRING( byKind[PLACE_OTHER] ) + ", beds " + zSTRING( byKind[ICON_BED] ) + ", chairs " + zSTRING( byKind[ICON_CHAIR] ) + ", benches " + zSTRING( byKind[ICON_BENCH] ) + ", armchairs " + zSTRING( byKind[ICON_THRONE] ) + ", fires " + zSTRING( byKind[ICON_FIRE] ) + ", ladders " + zSTRING( byKind[ICON_LADDER] ) + ", noise " + zSTRING( byKind[PLACE_REST] ) + ")" );
    }
  }
  inline void PlaceMark( int kind, float px, float py, float s ) {
    if( PlaceIsRest( kind ) && g_iconSet == 0 )
      s *= 0.8f;
    if( kind < ICON_COUNT && DrawIcon( kind, px, py, s ) ) return;
    if( ( kind == PLACE_OTHER || kind == PLACE_REST ) && g_iconFromFile[ICON_INTERACT]
        && DrawIcon( ICON_INTERACT, px, py, s ) )
      return;
    const float r = s * ( kind >= PLACE_OTHER ? 0.26f : 0.40f );
    FillCircleTex( px, py, r + 1.0f, zCOLOR( 18, 16, 15, 255 ) );
    FillCircleTex( px, py, r, AtlasColor( kind ) );
  }
  inline void DrawPlaces() {
    if( !ogame || !player ) return;
    oCWorld* world = ogame->GetGameWorld();
    if( !world ) return;
    const bool filtOn = SearchFilterOn();
    if( !g_showPlaces && !filtOn ) return;
    if( g_placeCache && world != g_placeWorld )
      PlaceCacheFree( false );
    const DWORD now = GetTickCount();
    if( !g_placeWorld || (long)( now - g_placeNext ) >= 0 ) {
      g_placeNext = now + PLACE_CACHE_MS;
      PlaceCacheRefresh( world );
      if( !g_placeWorld ) g_placeWorld = world;
    }
    if( !g_placeCache || g_placeCount <= 0 ) return;
    const bool filt = filtOn && g_placeMatch && g_placeMatchN == g_placeCount;
    int limit = ( g_itemLimit == 60 ) ? 200 : g_itemLimit;
    if( g_full || g_showPlaces >= SHOW_WORLD ) limit = 8000;
    const float r2   = (float)g_itemRange * (float)g_itemRange;
    const float size = MarkSizePxK() * 0.85f;
    const zVEC3 pp   = player->GetPositionWorld();
    int drawn = 0;
    for( int i = 0; i < g_placeCount && drawn < limit; i++ ) {
      zCVob* const pl = g_placeCache[i];
      if( !pl || pl->homeWorld != (zCWorld*)world ) continue;
      const int  kind = g_placeKind[i];
      const bool hit  = filt && g_placeMatch[i];
      if( !hit && !PlaceKindShown( kind, g_showPlaces ) ) continue;
      const zVEC3 p = pl->GetPositionWorld();
      const float dx = p[0] - pp[0], dz = p[2] - pp[2];
      if( !hit && g_showPlaces < SHOW_WORLD && dx * dx + dz * dz > r2 ) continue;
      float mx, my;
      WorldToPx( p[0], p[2], mx, my );
      if( mx < g_clipX0 || mx > g_clipX1 || my < g_clipY0 || my > g_clipY1 ) continue;
      if( !MarkerVisible( mx, my ) ) continue;
      PlaceMark( kind, mx, my, size );
      if( hit )
        SearchRing( mx, my, size );
      HoverConsider( pl, mx, my, size, false );
      drawn++;
    }
    if( !g_showSwitches && filtOn ) {
      if( g_switchCache && world != g_switchWorld )
        SwitchCacheFree( false );
      if( !g_switchWorld || (long)( now - g_switchNext ) >= 0 ) {
        g_switchNext = now + SWITCH_CACHE_MS;
        SwitchCacheRefresh( world );
        if( !g_switchWorld ) g_switchWorld = world;
      }
      if( g_switchCache && g_switchMatch && g_switchMatchN == g_switchCount )
        for( int i = 0; i < g_switchCount; i++ ) {
          zCVob* const sw = g_switchCache[i];
          if( !g_switchMatch[i] || !sw || sw->homeWorld != (zCWorld*)world ) continue;
          const zVEC3 p = sw->GetPositionWorld();
          float mx, my;
          WorldToPx( p[0], p[2], mx, my );
          if( mx < g_clipX0 || mx > g_clipX1 || my < g_clipY0 || my > g_clipY1 || !MarkerVisible( mx, my ) ) continue;
          SwitchMark( g_switchKind[i], mx, my, size );
          SearchRing( mx, my, size );
          HoverConsider( sw, mx, my, size, false );
        }
    }
  }
  static TExit    g_exits[EXIT_MAX];
  static int      g_exitCount = 0;
  static oCWorld* g_exitWorld = 0;
  static bool     g_exitMatch[EXIT_MAX];
  static int      g_exitMatchN = 0;
  inline void ExitFree() {
    g_exitCount  = 0;
    g_exitWorld  = 0;
    g_exitMatchN = 0;
  }
  inline void ExitDestName( const char* lvl, char* out, int max ) {
    out[0] = 0;
    if( !lvl || max < 2 ) return;
    const char* s = lvl;
    for( const char* p = lvl; *p; p++ )
      if( *p == '\\' || *p == '/' ) s = p + 1;
    int n = 0;
    for( ; s[n] && s[n] != '.' && n < max - 1; n++ ) {
      char c = s[n];
      if( c == '_' ) c = ' ';
      else if( n == 0 ) c = UpChar( c );
      else if( c >= 'A' && c <= 'Z' ) c = (char)( c + 32 );
      out[n] = c;
    }
    out[n] = 0;
  }
  inline bool ExitCollectCore( oCWorld* world ) {
    __try {
      zCClassDef* const def = oCTriggerChangeLevel::classDef;
      if( !def ) return true;
      zCTree<zCVob>* const root = &world->globalVobTree;
      zCTree<zCVob>* node = root->firstChild;
      int guard = 0;
      while( node && guard < TREE_NODE_LIMIT && g_exitCount < EXIT_MAX ) {
        guard++;
        zCVob* const v = node->data;
        if( v && zCObject::CheckInheritance( def, v->_GetClassDef() ) ) {
          oCTriggerChangeLevel* const tr = (oCTriggerChangeLevel*)v;
          TExit& e = g_exits[g_exitCount++];
          e.vob = v;
          const zVEC3 p = v->GetPositionWorld();
          e.x = p[0];
          e.z = p[2];
          ExitDestName( tr->levelName.ToChar(), e.dest, 40 );
        }
        node = VobTreeNext( node, root );
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline void SearchMatchExits();
  inline void ExitEnsure( oCWorld* world ) {
    if( world == g_exitWorld ) return;
    ExitFree();
    g_exitWorld = world;
    if( !world ) return;
    if( !ExitCollectCore( world ) ) {
      MM_LOG( "exits: exception while collecting" );
      g_exitCount = 0;
    }
    MM_LOG( zSTRING( "exits: " ) + zSTRING( g_exitCount ) );
    SearchMatchExits();
  }
  inline const TExit* ExitOf( zCVob* v ) {
    if( !v ) return 0;
    for( int i = 0; i < g_exitCount; i++ )
      if( g_exits[i].vob == v ) return &g_exits[i];
    return 0;
  }
  inline bool SearchExitHit( const TExit& e ) {
    if( g_searchCat != SCAT_ALL && g_searchCat != SCAT_PLACES ) return false;
    if( !g_searchNorm[0] ) return g_searchCat == SCAT_PLACES;
    int len = (int)strlen( g_searchNorm );
    if( len < 2 ) return false;
    char dn[48];
    SearchNormalize( e.dest, dn, 48 );
    if( PlaceWordHit( dn, g_searchNorm ) ) return true;
    if( len < 3 ) return false;
    char cat[64];
    SearchNormalize( T( TX_HOV_EXIT ), cat, 64 );
    return PlaceWordHit( cat, g_searchNorm )
        || PlaceWordHit( "przejscie wyjscie brama zmiana swiata exit transition gate level "
                         "uebergang ausgang tor", g_searchNorm );
  }
  inline void SearchMatchExits() {
    g_exitMatchN = 0;
    if( !SearchHasFilter() ) return;
    for( int i = 0; i < g_exitCount; i++ )
      g_exitMatch[i] = SearchExitHit( g_exits[i] );
    g_exitMatchN = g_exitCount;
  }
  inline int SearchExitFound() {
    int n = 0;
    if( g_exitMatchN == g_exitCount )
      for( int i = 0; i < g_exitCount; i++ )
        if( g_exitMatch[i] ) n++;
    return n;
  }
  inline void SearchSuggestExits() {
    if( !g_searchNorm[0] || g_exitMatchN != g_exitCount ) return;
    for( int i = 0; i < g_exitCount; i++ )
      if( g_exitMatch[i] )
        SearchSuggestAdd( zSTRING( T( TX_HOV_EXIT ) ) + ": " + zSTRING( g_exits[i].dest ), 1 );
  }
  inline void DrawExits() {
    if( !ogame || !player ) return;
    oCWorld* const world = ogame->GetGameWorld();
    if( !world ) return;
    const bool filtOn = SearchFilterOn();
    if( !g_showExits && !filtOn ) return;
    ExitEnsure( world );
    if( g_exitCount <= 0 ) return;
    const bool  filt = filtOn && g_exitMatchN == g_exitCount;
    const float r2   = (float)g_itemRange * (float)g_itemRange;
    const float size = MarkSizePxK() * 0.95f;
    const zVEC3 pp   = player->GetPositionWorld();
    for( int i = 0; i < g_exitCount; i++ ) {
      const TExit& e = g_exits[i];
      if( !e.vob || e.vob->homeWorld != (zCWorld*)world ) continue;
      const bool hit = filt && g_exitMatch[i];
      if( !hit && !g_showExits ) continue;
      const float dx = e.x - pp[0], dz = e.z - pp[2];
      if( !hit && g_showExits < SHOW_WORLD && dx * dx + dz * dz > r2 ) continue;
      float mx, my;
      WorldToPx( e.x, e.z, mx, my );
      if( mx < g_clipX0 || mx > g_clipX1 || my < g_clipY0 || my > g_clipY1 || !MarkerVisible( mx, my ) ) continue;
      if( !DrawIcon( ICON_EXIT, mx, my, size ) ) {
        const float r = size * 0.40f;
        FillCircleTex( mx, my, r + 1.0f, zCOLOR( 18, 16, 15, 255 ) );
        FillCircleTex( mx, my, r, ColExit() );
      }
      if( hit )
        SearchRing( mx, my, size );
      HoverConsider( e.vob, mx, my, size, false );
    }
  }
  static zCViewDraw* g_white = 0;
  inline zCTexture* WhiteTex() {
    if( !g_white ) {
      g_white = new zCViewDraw();
      if( !g_white ) return 0;
      g_white->SetTexture( zSTRING( "WHITE.TGA" ) );
      MM_LOG( g_white->Texture ? "white texture loaded"
                                        : "NO white texture WHITE.TGA" );
    }
    return g_white->Texture;
  }
  inline void TexRect( float x, float y, float w, float h, const zCOLOR& c ) {
    if( w <= 0.0f || h <= 0.0f || !zrenderer ) return;
    zCTexture* t = WhiteTex();
    if( !t ) return;
    t->CacheIn( -1.0f );
    zrenderer->DrawTile( t, zVEC2( x, y ), zVEC2( x + w, y + h ), MINIMAP_Z,
                         zVEC2( 0.0f, 0.0f ), zVEC2( 1.0f, 1.0f ), c );
  }
  inline void FillTriTex( float ax, float ay, float bx, float by,
                          float cx, float cy, const zCOLOR& col ) {
    const float px[3] = { ax, bx, cx };
    const float py[3] = { ay, by, cy };
    float minY = ay, maxY = ay;
    if( by < minY ) minY = by;
    if( by > maxY ) maxY = by;
    if( cy < minY ) minY = cy;
    if( cy > maxY ) maxY = cy;
    for( float y = minY; y <= maxY; y += 1.0f ) {
      float x0 = 0.0f, x1 = 0.0f;
      int   n  = 0;
      for( int i = 0; i < 3; i++ ) {
        const int   j  = ( i + 1 ) % 3;
        const float ya = py[i];
        const float yb = py[j];
        if( ( ya <= y && yb > y ) || ( yb <= y && ya > y ) ) {
          const float t = ( y - ya ) / ( yb - ya );
          const float x = px[i] + t * ( px[j] - px[i] );
          if( n == 0 ) {
            x0 = x;
            x1 = x;
          }
          else {
            if( x < x0 ) x0 = x;
            if( x > x1 ) x1 = x;
          }
          n++;
        }
      }
      if( n >= 2 && x1 > x0 ) {
        float span = x1 - x0;
        if( span < 1.0f ) span = 1.0f;
        TexRect( x0, y, span, 1.0f, col );
      }
    }
  }
  inline void PlayerPx( float& px, float& py ) {
    if( !g_full ) {
      px = g_cx;
      py = g_cy;
      return;
    }
    const zVEC3 p = player->GetPositionWorld();
    WorldToPx( p[0], p[2], px, py );
  }
  inline void DrawPlayerArrow() {
    const zVEC3 at = player->GetAtVectorWorld();
    float fx, fy;
    DirToPx( at[0], at[2], fx, fy );
    const float len = sqrtf( fx * fx + fy * fy );
    if( len < 0.0001f ) {
      fx = 0.0f;
      fy = -1.0f;
    }
    else {
      fx /= len;
      fy /= len;
    }
    const float sx = -fy;
    const float sy = fx;
    float arrowLen;
    if( g_playerSize > 0 )
      arrowLen = (float)g_playerSize;
    else if( g_arrowSize > 0 )
      arrowLen = (float)g_arrowSize;
    else if( g_full )
      arrowLen = 13.0f;
    else {
      const float bw = g_clipX1 - g_clipX0;
      const float bh = g_clipY1 - g_clipY0;
      arrowLen = ( ( bw < bh ) ? bw : bh ) * 0.045f;
      if( arrowLen < 8.0f ) arrowLen = 8.0f; else if( arrowLen > 18.0f ) arrowLen = 18.0f;
    }
    float ax, ay;
    PlayerPx( ax, ay );
    if( DrawIconRot( ICON_PLAYER, ax, ay, arrowLen * 1.15f, fx, fy ) ) return;
    for( int pass = ( g_playerEdge ? 0 : 1 ); pass < 2; pass++ ) {
      const float r = arrowLen * 0.5f * ( pass == 0 ? 1.35f : 1.0f );
      const float back = r * 0.75f;
      const float wing = r * 0.62f;
      const float tipX = ax + fx * r;
      const float tipY = ay + fy * r;
      const float b1x  = ax - fx * back + sx * wing;
      const float b1y  = ay - fy * back + sy * wing;
      const float b2x  = ax - fx * back - sx * wing;
      const float b2y  = ay - fy * back - sy * wing;
      const float midX = ax - fx * back * 0.35f;
      const float midY = ay - fy * back * 0.35f;
      const zCOLOR c = ( pass == 0 ) ? zCOLOR( 24, 20, 18, 255 ) : ColPlayer();
      FillTriTex( tipX, tipY, b1x, b1y, midX, midY, c );
      FillTriTex( tipX, tipY, b2x, b2y, midX, midY, c );
      static bool logged = false;
      if( !logged && pass == 1 ) {
        logged = true;
        MM_LOG( zSTRING( "arrow: centre " ) + zSTRING( (int)g_cx ) + " " + zSTRING( (int)g_cy ) + ", tip " + zSTRING( (int)tipX ) + " " + zSTRING( (int)tipY ) + ", wings " + zSTRING( (int)b1x ) + " " + zSTRING( (int)b1y ) + " / " + zSTRING( (int)b2x ) + " " + zSTRING( (int)b2y ) + ", window " + zSTRING( (int)g_clipX0 ) + " " + zSTRING( (int)g_clipY0 ) + " - " + zSTRING( (int)g_clipX1 ) + " " + zSTRING( (int)g_clipY1 ) );
      }
    }
  }
  static zCViewDraw* g_posBlit  = 0;
  static bool        g_posTried = false;
  inline zCTexture* PlayerTexture() {
    if( g_posTried ) return g_posBlit ? g_posBlit->Texture : 0;
    g_posTried = true;
    if( !g_playerTex[0] ) return 0;
    g_posBlit = new zCViewDraw();
    if( !g_posBlit ) return 0;
    const char oldDefault = zCTexture::s_useDefaultTex;
    zCTexture::s_useDefaultTex = 0;
    g_posBlit->SetTexture( zSTRING( g_playerTex ) );
    zCTexture::s_useDefaultTex = oldDefault;
    int tw = 0, th = 0;
    if( g_posBlit->Texture )
      g_posBlit->Texture->GetPixelSize( tw, th );
    if( !g_posBlit->Texture || tw <= 0 || th <= 0 || tw > 96 || th > 96 ) {
      MM_LOG( zSTRING( "player marker: no usable texture " ) + zSTRING( g_playerTex ) + " (" + zSTRING( tw ) + "x" + zSTRING( th ) + ") - drawing own arrow" );
      DetachViewTexture( g_posBlit, 0 );
      return 0;
    }
    MM_LOG( zSTRING( "player marker: game texture " ) + zSTRING( g_playerTex ) + ", " + zSTRING( tw ) + "x" + zSTRING( th ) + " px" );
    return g_posBlit->Texture;
  }
  inline void DrawPlayerMark() {
    zCTexture* t = PlayerTexture();
    if( !t || !zrenderer ) {
      DrawPlayerArrow();
      return;
    }
    int tw = 0, th = 0;
    t->GetPixelSize( tw, th );
    if( tw <= 0 || th <= 0 ) {
      DrawPlayerArrow();
      return;
    }
    float w = (float)tw;
    float h = (float)th;
    float limit = (float)g_playerSize;
    if( limit <= 0.0f ) {
      const float bw = g_clipX1 - g_clipX0;
      const float bh = g_clipY1 - g_clipY0;
      limit = ( ( bw < bh ) ? bw : bh ) * 0.10f;
      if( limit < 10.0f ) limit = 10.0f; else if( limit > 22.0f ) limit = 22.0f;
      if( g_full )        limit = 16.0f;
    }
    const float big = ( w > h ) ? w : h;
    if( big > limit && big > 0.0f ) {
      const float s = limit / big;
      w *= s;
      h *= s;
    }
    float mx, my;
    PlayerPx( mx, my );
    t->CacheIn( -1.0f );
    zrenderer->DrawTile( t, zVEC2( mx - w * 0.5f, my - h * 0.5f ),
                            zVEC2( mx + w * 0.5f, my + h * 0.5f ),
                            MINIMAP_Z, zVEC2( 0.0f, 0.0f ), zVEC2( 1.0f, 1.0f ),
                            zCOLOR( 255, 255, 255, 255 ) );
  }
  inline float FrameT() {
    float t = (float)(int)( UiScale() + 0.7f );
    if( t < 1.0f ) t = 1.0f; else if( t > 3.0f ) t = 3.0f;
    return t;
  }
  inline float NorthH() {
    float hN = 9.0f * UiScale();
    if( hN < 6.0f ) hN = 6.0f;
    return hN;
  }
  inline void ThickLine( float x0, float y0, float x1, float y1, float t,
                         const zCOLOR& c ) {
    const float dx = x1 - x0, dy = y1 - y0;
    const float len = sqrtf( dx * dx + dy * dy );
    if( len < 0.0001f ) return;
    const float px = -dy / len, py = dx / len;
    const int   n  = (int)( t * 2.0f + 0.5f );
    for( int i = 0; i < ( n < 1 ? 1 : n ); i++ ) {
      const float o = ( n <= 1 ) ? 0.0f : ( -t * 0.5f + t * (float)i / (float)( n - 1 ) );
      RawLine( x0 + px * o, y0 + py * o, x1 + px * o, y1 + py * o, c );
    }
  }
  struct TStyleTex {
    zCTexture* tex;
    bool       tried;
    TStAtlas   at;
  };
  static TStyleTex g_styleTex[2] = { { 0, false }, { 0, false } };
  static TStQuad   g_styleQ[ST_QUADS];
  static const char* const kMapTitleFont = "FONT_OLD_20_WHITE.TGA";
  static zCFont* g_titleFont      = 0;
  static bool    g_titleFontTried = false;
}
