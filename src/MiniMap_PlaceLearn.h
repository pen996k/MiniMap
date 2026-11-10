#pragma once
namespace PlaceLearn {
  enum { PL_SHRINE = 19, PL_BOARD = 20, PL_CRAFT = 21, PL_ORE = 22, PL_TREASURE = 23,
         PL_ALCHEMY = 34, PL_KITCHEN = 35, PL_WORKSHOP = 36,
         PL_OTHER = 50, PL_REST = 51 };
  enum { PL_KINDS = 8 };
  inline int PlIdx( int k ) {
    if( k >= PL_SHRINE && k <= PL_TREASURE ) return k - PL_SHRINE;
    if( k >= PL_ALCHEMY && k <= PL_WORKSHOP ) return 5 + k - PL_ALCHEMY;
    return -1;
  }
  inline int PlKindOfIdx( int i ) { return i < 5 ? PL_SHRINE + i : PL_ALCHEMY + i - 5; }
  inline bool PlImportant( int k ) { return PlIdx( k ) >= 0; }
  inline bool PlCraftKind( int k ) { return k == PL_CRAFT || ( k >= PL_ALCHEMY && k <= PL_WORKSHOP ); }
  const unsigned short PL_CRAFT_BITS = (unsigned short)( ( 1u << 2 ) | ( 1u << 5 ) | ( 1u << 6 ) | ( 1u << 7 ) );
  const unsigned short PL_OTHER_BIT  = (unsigned short)0x100;
  using namespace Gothic_II_Addon;
  enum { PLT_ASSIGNINT = zPAR_OP_IS, PLT_EQ = zPAR_OP_EQUAL, PLT_RET = zPAR_TOK_RET,
         PLT_CALL = zPAR_TOK_CALL, PLT_CALLEXT = zPAR_TOK_CALLEXTERN, PLT_PUSHINT = zPAR_TOK_PUSHINT,
         PLT_PUSHVAR = zPAR_TOK_PUSHVAR, PLT_PUSHINST = zPAR_TOK_PUSHINST, PLT_JUMP = zPAR_TOK_JUMP,
         PLT_JUMPF = zPAR_TOK_JUMPF, PLT_SETINST = zPAR_TOK_SETINSTANCE,
         PLT_ARRAY = zPAR_TOK_PUSHVAR + zPAR_TOK_FLAGARRAY };
  inline int PlTokLen( unsigned char t ) {
    switch( t ) {
      case PLT_CALL: case PLT_CALLEXT: case PLT_PUSHINT: case PLT_PUSHVAR:
      case PLT_PUSHINST: case PLT_JUMP: case PLT_JUMPF: case PLT_SETINST: return 5;
      case PLT_ARRAY: return 6;
    }
    return 1;
  }
  struct TPlSym {
    unsigned char type;
    bool isConst, isExternal, dotted;
    int  ele;
    int  value;
  };
  typedef bool ( *TPlSymFn )( void* ctx, int index, TPlSym& out );
  enum { PL_FUNCS = 2048, PL_ASG = 8, PL_EXT = 32, PL_PAIRS = 2048, PL_MENU = 16,
         PL_SITES = 2048, PL_ITEMS = 1024, PL_GUARD = 6000, PL_SEEN = 128 };
  enum { PL_REC = 4096, PL_INST = 2048, PL_WRAPS = 16, PL_GOT = 64, PL_CHAIN = 4,
         PL_OWN_F = 0x8000 };
  struct TPlWrap { int adr; unsigned char n; signed char give, take; };
  struct TPlRec  { unsigned short owner, inst; unsigned char take; };
  struct TPlGot  { int inst; unsigned char take; };
  struct TPlFunc {
    int  sym;
    unsigned char nAsg, nExt;
    unsigned short pair[PL_ASG];
    unsigned short site[PL_EXT];
    unsigned char learned;
  };
  struct TPlState {
    bool  ready;
    int   nFuncs;
    TPlFunc f[PL_FUNCS];
    int   nPairs;
    int   pairVar[PL_PAIRS], pairVal[PL_PAIRS];
    unsigned short pairRecipes[PL_PAIRS];
    unsigned char  pairMenu[PL_PAIRS];
    int   nMenu, menu[PL_MENU];
    int   nSites, siteSym[PL_SITES];
    unsigned short siteCnt[PL_SITES];
    int   nItems;
    unsigned int  itemHash[PL_ITEMS];
    unsigned char itemKind[PL_ITEMS];
    int   truncated;
    int   dialogs, recipeDialogs;
    int   nWraps;
    TPlWrap wrap[PL_WRAPS];
    int   nInst;
    int   inst[PL_INST];
    unsigned char instK0[PL_INST], instK[PL_INST];
    int   nRec;
    TPlRec rec[PL_REC];
    unsigned char fprod[PL_FUNCS];
  };
  template<class F>
  inline void PlWalk( const unsigned char* code, int size, int adr, F& fn ) {
    if( !code || adr < 0 || adr >= size ) return;
    int i = adr, jumpMax = 0;
    for( int guard = 0; i < size && guard < PL_GUARD; guard++ ) {
      const unsigned char t = code[i];
      if( t == PLT_RET && jumpMax <= i ) break;
      const int len = PlTokLen( t );
      if( i + len > size ) break;
      int arg = 0;
      if( len >= 5 )
        memcpy( &arg, code + i + 1, 4 );
      if( ( t == PLT_JUMP || t == PLT_JUMPF ) && arg > jumpMax && arg < size ) jumpMax = arg;
      if( !fn( t, arg ) ) break;
      i += len;
    }
  }
  inline bool PlIsIntGlobal( const TPlSym& s ) {
    return s.type == 2 && !s.isConst && !s.dotted && s.ele == 1;
  }
  inline bool PlIsIntConst( const TPlSym& s ) {
    return s.type == 2 && s.isConst && s.ele == 1;
  }
  inline int PlPairIndex( TPlState& st, int var, int val, bool add ) {
    for( int i = 0; i < st.nPairs; i++ )
      if( st.pairVar[i] == var && st.pairVal[i] == val ) return i;
    if( !add || st.nPairs >= PL_PAIRS ) return -1;
    st.pairVar[st.nPairs] = var;
    st.pairVal[st.nPairs] = val;
    st.pairRecipes[st.nPairs] = 0;
    st.pairMenu[st.nPairs] = 0;
    return st.nPairs++;
  }
  inline int PlSiteIndex( TPlState& st, int sym ) {
    for( int i = 0; i < st.nSites; i++ )
      if( st.siteSym[i] == sym ) return i;
    if( st.nSites >= PL_SITES ) return -1;
    st.siteSym[st.nSites] = sym;
    st.siteCnt[st.nSites] = 0;
    return st.nSites++;
  }
  struct TPlScan {
    void* ctx; TPlSymFn sym; TPlState* st; TPlFunc* out;
    int pt[2], pa[2];
    bool operator()( unsigned char t, int arg ) {
      if( t == PLT_ASSIGNINT && pt[1] == PLT_PUSHVAR && ( pt[0] == PLT_PUSHINT || pt[0] == PLT_PUSHVAR ) ) {
        TPlSym v;
        if( sym( ctx, pa[1], v ) && PlIsIntGlobal( v ) ) {
          int val = pa[0];
          bool ok = true;
          if( pt[0] == PLT_PUSHVAR ) {
            TPlSym c;
            ok = sym( ctx, pa[0], c ) && PlIsIntConst( c );
            if( ok ) val = c.value;
          }
          if( ok ) {
            const int p = PlPairIndex( *st, pa[1], val, true );
            bool dup = p < 0;
            for( int k = 0; k < out->nAsg && !dup; k++ ) dup = out->pair[k] == p;
            if( !dup ) {
              if( out->nAsg < PL_ASG ) out->pair[out->nAsg++] = (unsigned short)p;
              else st->truncated++;
            }
            else if( p < 0 ) st->truncated++;
          }
        }
      }
      if( t == PLT_CALLEXT ) {
        const int s = PlSiteIndex( *st, arg );
        bool dup = s < 0;
        for( int k = 0; k < out->nExt && !dup; k++ ) dup = out->site[k] == s;
        if( !dup ) {
          if( out->nExt < PL_EXT ) out->site[out->nExt++] = (unsigned short)s;
          else st->truncated++;
        }
        else if( s < 0 ) st->truncated++;
      }
      pt[0] = pt[1]; pa[0] = pa[1];
      pt[1] = t;     pa[1] = arg;
      return true;
    }
  };
  inline int PlFind( const TPlState& st, int sym ) {
    int lo = 0, hi = st.nFuncs - 1;
    while( lo <= hi ) {
      const int mid = ( lo + hi ) >> 1;
      if( st.f[mid].sym == sym ) return mid;
      if( st.f[mid].sym < sym ) lo = mid + 1; else hi = mid - 1;
    }
    return -1;
  }
  inline bool PlIsMenu( const TPlState& st, int var ) {
    for( int i = 0; i < st.nMenu; i++ )
      if( st.menu[i] == var ) return true;
    return false;
  }
  inline void PlBuildFuncs( TPlState& st, const unsigned char* code, int size,
                            void* ctx, TPlSymFn sym, const int* s1, int n ) {
    memset( &st, 0, sizeof( st ) );
    for( int i = 0; i < n; i++ ) {
      TPlSym s;
      if( !sym( ctx, s1[i], s ) || s.type != 5 || !s.isConst || s.isExternal ) continue;
      if( st.nFuncs >= PL_FUNCS || ( st.nFuncs > 0 && st.f[st.nFuncs - 1].sym >= s1[i] ) ) {
        st.truncated++;
        continue;
      }
      TPlFunc& f = st.f[st.nFuncs++];
      f.sym = s1[i];
      TPlScan sc = { ctx, sym, &st, &f, { -1, -1 }, { 0, 0 } };
      PlWalk( code, size, s.value, sc );
    }
    for( int p = 0; p < st.nPairs; p++ ) {
      const int var = st.pairVar[p];
      if( PlIsMenu( st, var ) ) continue;
      int nv = 0;
      for( int q = 0; q < st.nPairs && nv < 3; q++ )
        if( st.pairVar[q] == var ) nv++;
      if( nv >= 3 ) {
        if( st.nMenu < PL_MENU ) st.menu[st.nMenu++] = var;
        else st.truncated++;
      }
    }
    for( int p = 0; p < st.nPairs; p++ )
      st.pairMenu[p] = PlIsMenu( st, st.pairVar[p] ) ? 1 : 0;
    for( int i = 0; i < st.nFuncs; i++ )
      for( int e = 0; e < st.f[i].nExt; e++ )
        if( st.siteCnt[st.f[i].site[e]] < 65535 ) st.siteCnt[st.f[i].site[e]]++;
  }
  struct TPlCond {
    void* ctx; TPlSymFn sym; const TPlState* st;
    int keyVar[8], keyVal[8], n;
    int pt[2], pa[2];
    bool operator()( unsigned char t, int arg ) {
      if( t == PLT_EQ ) {
        for( int o = 0; o < 2; o++ ) {
          const int p = o, q = 1 - o;
          if( pt[p] != PLT_PUSHVAR || !PlIsMenu( *st, pa[p] ) ) continue;
          int val = 0; bool ok = false;
          if( pt[q] == PLT_PUSHINT ) { val = pa[q]; ok = true; }
          else if( pt[q] == PLT_PUSHVAR ) {
            TPlSym c;
            if( sym( ctx, pa[q], c ) && PlIsIntConst( c ) ) { val = c.value; ok = true; }
          }
          if( ok ) {
            bool dup = false;
            for( int k = 0; k < n && !dup; k++ ) dup = keyVar[k] == pa[p] && keyVal[k] == val;
            if( !dup && n < 8 ) { keyVar[n] = pa[p]; keyVal[n] = val; n++; }
          }
        }
      }
      pt[0] = pt[1]; pa[0] = pa[1];
      pt[1] = t;     pa[1] = arg;
      return true;
    }
  };
  struct TPlRecipe {
    const unsigned char* code; int size;
    int createIdx, removeIdx;
    bool hasCreate, hasRemove;
    int depth;
    int seen[PL_SEEN], nSeen;
    bool operator()( unsigned char t, int arg ) {
      if( t == PLT_CALLEXT ) {
        if( arg == createIdx ) hasCreate = true;
        if( arg == removeIdx ) hasRemove = true;
      }
      else if( t == PLT_CALL && depth > 0 ) {
        bool was = false;
        for( int k = 0; k < nSeen && !was; k++ ) was = seen[k] == arg;
        if( !was && nSeen < PL_SEEN ) {
          seen[nSeen++] = arg;
          depth--;
          PlWalk( code, size, arg, *this );
          depth++;
        }
      }
      return !( hasCreate && hasRemove );
    }
  };
  inline void PlBuildRecipes( TPlState& st, const unsigned char* code, int size,
                              void* ctx, TPlSymFn sym, const int* cond, const int* info,
                              int n, int createIdx, int removeIdx ) {
    st.dialogs = n;
    st.recipeDialogs = 0;
    if( st.nMenu && createIdx >= 0 && removeIdx >= 0 )
      for( int i = 0; i < n; i++ ) {
        TPlSym cs;
        if( !sym( ctx, cond[i], cs ) || cs.type != 5 || cs.isExternal ) continue;
        TPlCond c = { ctx, sym, &st, { 0 }, { 0 }, 0, { -1, -1 }, { 0, 0 } };
        PlWalk( code, size, cs.value, c );
        if( !c.n || info[i] <= 0 ) continue;
        TPlSym is;
        if( !sym( ctx, info[i], is ) || is.type != 5 || is.isExternal ) continue;
        TPlRecipe r = { code, size, createIdx, removeIdx, false, false, 2, { 0 }, 0 };
        PlWalk( code, size, is.value, r );
        if( !( r.hasCreate && r.hasRemove ) ) continue;
        st.recipeDialogs++;
        for( int k = 0; k < c.n; k++ ) {
          const int p = PlPairIndex( st, c.keyVar[k], c.keyVal[k], false );
          if( p >= 0 && st.pairRecipes[p] < 65535 ) st.pairRecipes[p]++;
        }
      }
    st.ready = true;
  }
  inline unsigned char PlCatKind( unsigned int mf ) {
    if( mf & ITM_CAT_NF ) return PL_CRAFT;
    if( mf & ( ITM_CAT_FF | ITM_CAT_MUN ) ) return PL_WORKSHOP;
    if( mf & ( ITM_CAT_POTION | ITM_CAT_RUNE | (unsigned int)ITM_CAT_MAGIC ) ) return PL_ALCHEMY;
    if( mf & ITM_CAT_FOOD ) return PL_KITCHEN;
    return 0;
  }
  inline int PlParamOf( int fsym, int n, int s ) {
    return ( s > fsym && s <= fsym + n ) ? s - fsym - 1 : -1;
  }
  struct TPlExt4 { int idx[4]; int pos[4]; unsigned char take[4]; };
  inline int PlExtMatch( const TPlExt4& e, int arg, int& pos, unsigned char& take ) {
    for( int k = 0; k < 4; k++ )
      if( e.idx[k] >= 0 && e.idx[k] == arg ) { pos = e.pos[k]; take = e.take[k]; return 1; }
    return 0;
  }
  struct TPlRing {
    unsigned char t[8]; int a[8]; int n;
    void Clear() { n = 0; }
    void Push( unsigned char tt, int aa ) {
      if( n == 8 ) { for( int k = 1; k < 8; k++ ) { t[k - 1] = t[k]; a[k - 1] = a[k]; } n = 7; }
      t[n] = tt; a[n] = aa; n++;
    }
    bool From( int back, unsigned char& tt, int& aa ) const {
      if( back < 1 || back > n ) return false;
      tt = t[n - back]; aa = a[n - back];
      return true;
    }
  };
  inline bool PlPushTok( unsigned char t ) {
    return t == PLT_PUSHINT || t == PLT_PUSHVAR || t == PLT_PUSHINST || t == PLT_ARRAY;
  }
  struct TPlWrapScan {
    const TPlExt4* e; int fsym, n; int give, take;
    TPlRing r;
    bool operator()( unsigned char t, int arg ) {
      if( PlPushTok( t ) ) { r.Push( t, arg ); return true; }
      int pos = 0; unsigned char tk = 0;
      if( t == PLT_CALLEXT && PlExtMatch( *e, arg, pos, tk ) ) {
        unsigned char tt; int aa;
        if( r.From( pos, tt, aa ) && tt == PLT_PUSHVAR ) {
          const int q = PlParamOf( fsym, n, aa );
          if( q >= 0 ) {
            if( tk && take < 0 ) take = q;
            if( !tk && give < 0 ) give = q;
          }
        }
      }
      r.Clear();
      return true;
    }
  };
  inline bool PlIsInst( void* ctx, TPlSymFn sym, int x ) {
    TPlSym s;
    return sym( ctx, x, s ) && s.type == 7 && s.value > 0;
  }
  struct TPlItemScan {
    const unsigned char* code; int size; void* ctx; TPlSymFn sym;
    const TPlState* st; const TPlExt4* e;
    int depth; int seen[PL_SEEN]; int nSeen;
    TPlGot got[PL_GOT]; int nGot; int* trunc;
    void Add( int x, unsigned char take ) {
      if( nGot < PL_GOT ) { got[nGot].inst = x; got[nGot].take = take; nGot++; }
      else ( *trunc )++;
    }
    bool operator()( unsigned char t, int arg );
  };
  struct TPlItemWalk {
    TPlItemScan* s; TPlRing r;
    bool operator()( unsigned char t, int arg ) {
      if( PlPushTok( t ) ) { r.Push( t, arg ); return true; }
      int pos = 0; unsigned char tk = 0;
      if( t == PLT_CALLEXT && PlExtMatch( *s->e, arg, pos, tk ) ) {
        unsigned char tt; int aa;
        if( r.From( pos, tt, aa ) && tt == PLT_PUSHINT && PlIsInst( s->ctx, s->sym, aa ) )
          s->Add( aa, tk );
      }
      else if( t == PLT_CALL ) {
        int w = -1;
        for( int k = 0; k < s->st->nWraps && w < 0; k++ )
          if( s->st->wrap[k].adr == arg ) w = k;
        if( w >= 0 ) {
          const TPlWrap& Wr = s->st->wrap[w];
          const int ps[2] = { Wr.give, Wr.take };
          for( int g = 0; g < 2; g++ ) {
            unsigned char tt; int aa;
            if( ps[g] >= 0 && r.From( Wr.n - ps[g], tt, aa ) && tt == PLT_PUSHINT
                && PlIsInst( s->ctx, s->sym, aa ) )
              s->Add( aa, (unsigned char)g );
          }
        }
        else if( s->depth > 0 ) {
          bool was = false;
          for( int k = 0; k < s->nSeen && !was; k++ ) was = s->seen[k] == arg;
          if( !was && s->nSeen < PL_SEEN ) {
            s->seen[s->nSeen++] = arg;
            s->depth--;
            TPlItemWalk sub = { s, { { 0 }, { 0 }, 0 } };
            PlWalk( s->code, s->size, arg, sub );
            s->depth++;
          }
        }
      }
      r.Clear();
      return true;
    }
  };
  struct TPlFlagScan {
    const unsigned char* code; int size; void* ctx; TPlSymFn sym; int mainf;
    int depth; bool found; int val;
    int pt[2], pa[2];
    bool operator()( unsigned char t, int arg ) {
      if( t == PLT_CALL && depth > 0 ) {
        TPlFlagScan sub = { code, size, ctx, sym, mainf, depth - 1, false, 0, { -1, -1 }, { 0, 0 } };
        PlWalk( code, size, arg, sub );
        if( sub.found ) { found = true; val = sub.val; }
      }
      else if( t == PLT_ASSIGNINT && pt[1] == PLT_PUSHVAR && pa[1] == mainf
               && ( pt[0] == PLT_PUSHINT || pt[0] == PLT_PUSHVAR ) ) {
        int v = pa[0]; bool ok = true;
        if( pt[0] == PLT_PUSHVAR ) {
          TPlSym c;
          ok = sym( ctx, pa[0], c ) && PlIsIntConst( c );
          if( ok ) v = c.value;
        }
        if( ok ) { found = true; val = v; }
      }
      const int tt = ( t == PLT_ASSIGNINT || t == PLT_PUSHINT || t == PLT_PUSHVAR || t == PLT_CALL ) ? t : -1;
      pt[0] = pt[1]; pa[0] = pa[1];
      pt[1] = tt;    pa[1] = ( t == PLT_PUSHVAR || t == PLT_PUSHINT ) ? arg : 0;
      return true;
    }
  };
  inline int PlInstIndex( const TPlState& st, int x ) {
    for( int i = 0; i < st.nInst; i++ )
      if( st.inst[i] == x ) return i;
    return -1;
  }
  inline void PlAddRecipe( TPlState& st, const unsigned char* code, int size, void* ctx, TPlSymFn sym,
                           int mainf, int owner, const TPlGot* got, int nGot ) {
    bool give = false, take = false;
    for( int k = 0; k < nGot; k++ ) { if( got[k].take ) take = true; else give = true; }
    if( !give || !take ) return;
    for( int k = 0; k < nGot; k++ ) {
      int xi = PlInstIndex( st, got[k].inst );
      if( xi >= 0 ) {
        bool dup = false;
        for( int r = 0; r < st.nRec && !dup; r++ )
          dup = st.rec[r].owner == owner && st.rec[r].inst == xi && st.rec[r].take == got[k].take;
        if( dup ) continue;
      }
      if( st.nRec >= PL_REC ) { st.truncated++; return; }
      if( xi < 0 ) {
        if( st.nInst >= PL_INST ) { st.truncated++; return; }
        xi = st.nInst++;
        st.inst[xi] = got[k].inst;
        TPlSym is;
        int adr = ( sym( ctx, got[k].inst, is ) ) ? is.value : 0;
        TPlFlagScan fs = { code, size, ctx, sym, mainf, 1, false, 0, { -1, -1 }, { 0, 0 } };
        PlWalk( code, size, adr, fs );
        st.instK0[xi] = fs.found ? PlCatKind( (unsigned int)fs.val ) : 0;
      }
      st.rec[st.nRec].owner = (unsigned short)owner;
      st.rec[st.nRec].inst  = (unsigned short)xi;
      st.rec[st.nRec].take  = got[k].take;
      st.nRec++;
    }
  }
  inline int PlMajority( const unsigned short* cnt, int minN ) {
    int n = 0, best = -1;
    for( int k = 0; k < PL_KINDS; k++ ) {
      n += cnt[k];
      if( best < 0 || cnt[k] > cnt[best] ) best = k;
    }
    if( n < minN || best < 0 || !cnt[best] ) return 0;
    return ( cnt[best] * 3 >= n * 2 ) ? PlKindOfIdx( best ) : PL_WORKSHOP;
  }
  inline void PlOwnerCounts( const TPlState& st, const unsigned char* ik,
                             unsigned short ( *cp )[PL_KINDS], unsigned short ( *cf )[PL_KINDS] ) {
    memset( cp, 0, sizeof( unsigned short ) * PL_KINDS * PL_PAIRS );
    memset( cf, 0, sizeof( unsigned short ) * PL_KINDS * PL_FUNCS );
    for( int r = 0; r < st.nRec; r++ ) {
      const TPlRec& R = st.rec[r];
      if( R.take || !ik[R.inst] ) continue;
      const int b = PlIdx( ik[R.inst] );
      if( b < 0 ) continue;
      if( R.owner & PL_OWN_F ) { if( cf[R.owner & 0x7FFF][b] < 65535 ) cf[R.owner & 0x7FFF][b]++; }
      else if( cp[R.owner][b] < 65535 ) cp[R.owner][b]++;
    }
  }
  struct TPlProdWork {
    unsigned short cp[PL_PAIRS][PL_KINDS];
    unsigned short cf[PL_FUNCS][PL_KINDS];
    unsigned char  okp[PL_PAIRS], okf[PL_FUNCS];
    unsigned short mask[PL_INST];
  };
  inline void PlBuildProduce( TPlState& st, TPlProdWork& w, const unsigned char* code, int size,
                              void* ctx, TPlSymFn sym, const int* ext, int mainf,
                              const int* fsyms, int nf, const int* cond, const int* info, int n ) {
    st.nWraps = 0; st.nInst = 0; st.nRec = 0;
    memset( st.fprod, 0, sizeof( st.fprod ) );
    if( mainf < 0 ) return;
    TPlExt4 e = { { ext[0], ext[1], ext[2], ext[3] }, { 2, 1, 2, 1 }, { 0, 0, 1, 1 } };
    for( int i = 0; i < nf; i++ ) {
      TPlSym s;
      if( !sym( ctx, fsyms[i], s ) || s.type != 5 || !s.isConst || s.isExternal || s.ele < 2 || s.ele > 8 ) continue;
      TPlWrapScan ws = { &e, fsyms[i], s.ele, -1, -1, { { 0 }, { 0 }, 0 } };
      PlWalk( code, size, s.value, ws );
      if( ( ws.give >= 0 || ws.take >= 0 ) && ws.give != ws.take ) {
        if( st.nWraps >= PL_WRAPS ) { st.truncated++; continue; }
        TPlWrap& Wr = st.wrap[st.nWraps++];
        Wr.adr = s.value; Wr.n = (unsigned char)s.ele;
        Wr.give = (signed char)ws.give; Wr.take = (signed char)ws.take;
      }
    }
    TPlItemScan sc;
    for( int fi = 0; fi < st.nFuncs; fi++ ) {
      TPlSym s;
      if( !sym( ctx, st.f[fi].sym, s ) ) continue;
      sc.code = code; sc.size = size; sc.ctx = ctx; sc.sym = sym; sc.st = &st; sc.e = &e;
      sc.depth = 2; sc.nSeen = 0; sc.nGot = 0; sc.trunc = &st.truncated;
      TPlItemWalk iw = { &sc, { { 0 }, { 0 }, 0 } };
      PlWalk( code, size, s.value, iw );
      PlAddRecipe( st, code, size, ctx, sym, mainf, PL_OWN_F | fi, sc.got, sc.nGot );
    }
    if( st.nMenu )
      for( int i = 0; i < n; i++ ) {
        TPlSym cs;
        if( !sym( ctx, cond[i], cs ) || cs.type != 5 || cs.isExternal ) continue;
        TPlCond c = { ctx, sym, &st, { 0 }, { 0 }, 0, { -1, -1 }, { 0, 0 } };
        PlWalk( code, size, cs.value, c );
        if( !c.n || info[i] <= 0 ) continue;
        TPlSym is;
        if( !sym( ctx, info[i], is ) || is.type != 5 || is.isExternal ) continue;
        sc.code = code; sc.size = size; sc.ctx = ctx; sc.sym = sym; sc.st = &st; sc.e = &e;
        sc.depth = 2; sc.nSeen = 0; sc.nGot = 0; sc.trunc = &st.truncated;
        TPlItemWalk iw = { &sc, { { 0 }, { 0 }, 0 } };
        PlWalk( code, size, is.value, iw );
        for( int k = 0; k < c.n; k++ ) {
          const int p = PlPairIndex( st, c.keyVar[k], c.keyVal[k], false );
          if( p >= 0 )
            PlAddRecipe( st, code, size, ctx, sym, mainf, p, sc.got, sc.nGot );
        }
      }
    memcpy( st.instK, st.instK0, (size_t)( st.nInst ) * sizeof( st.instK[0] ) );
    for( int round = 0; round < PL_CHAIN; round++ ) {
      PlOwnerCounts( st, st.instK, w.cp, w.cf );
      for( int p = 0; p < st.nPairs; p++ ) w.okp[p] = (unsigned char)PlMajority( w.cp[p], 1 );
      for( int f = 0; f < st.nFuncs; f++ ) w.okf[f] = (unsigned char)PlMajority( w.cf[f], 1 );
      memset( w.mask, 0, sizeof( w.mask ) );
      for( int r = 0; r < st.nRec; r++ ) {
        const TPlRec& R = st.rec[r];
        if( !R.take || st.instK[R.inst] ) continue;
        const int k = ( R.owner & PL_OWN_F ) ? w.okf[R.owner & 0x7FFF] : w.okp[R.owner];
        if( k > 0 ) w.mask[R.inst] |= (unsigned short)( 1u << PlIdx( k ) );
      }
      bool changed = false;
      for( int i = 0; i < st.nInst; i++ ) {
        const unsigned short m = w.mask[i];
        if( st.instK[i] || !m ) continue;
        int hi = 0;
        for( int b = 0; b < PL_KINDS; b++ ) if( m & ( 1u << b ) ) hi = b;
        st.instK[i] = ( m & ( m - 1 ) ) ? (unsigned char)PL_WORKSHOP : (unsigned char)PlKindOfIdx( hi );
        changed = true;
      }
      if( !changed ) break;
    }
    PlOwnerCounts( st, st.instK, w.cp, w.cf );
    for( int fi = 0; fi < st.nFuncs; fi++ ) {
      unsigned short c[PL_KINDS];
      for( int b = 0; b < PL_KINDS; b++ ) c[b] = w.cf[fi][b];
      const TPlFunc& F = st.f[fi];
      for( int a = 0; a < F.nAsg; a++ ) {
        const int p = F.pair[a];
        if( !st.pairMenu[p] ) continue;
        for( int b = 0; b < PL_KINDS; b++ ) {
          const unsigned int s2 = (unsigned int)c[b] + w.cp[p][b];
          c[b] = (unsigned short)( s2 > 65535u ? 65535u : s2 );
        }
      }
      st.fprod[fi] = (unsigned char)PlMajority( c, 3 );
    }
  }
  struct TPlMob {
    int  func;
    unsigned int item;
    bool named, fallback;
    unsigned char kind;
  };
  inline unsigned char PlItemKind( const TPlState& st, unsigned int h ) {
    if( !h ) return 0;
    for( int i = 0; i < st.nItems; i++ )
      if( st.itemHash[i] == h ) return st.itemKind[i];
    return 0;
  }
  inline unsigned char PlLearned( const TPlState& st, const TPlMob& m ) {
    if( !m.fallback ) return 0;
    if( m.func >= 0 && m.func < st.nFuncs ) return st.f[m.func].learned;
    if( m.named ) return PlItemKind( st, m.item );
    return 0;
  }
  inline int PlEff( const TPlState& st, const TPlMob& m ) {
    return m.fallback ? PlLearned( st, m ) : m.kind;
  }
  inline unsigned short PlBit( int k ) {
    const int i = PlIdx( k );
    return i >= 0 ? (unsigned short)( 1u << i ) : PL_OTHER_BIT;
  }
  inline int PlBits( unsigned short mask ) {
    int n = 0;
    for( ; mask; mask &= (unsigned short)( mask - 1 ) ) n++;
    return n;
  }
  inline int PlOne( unsigned short mask ) {
    if( !mask || ( mask & PL_OTHER_BIT ) ) return 0;
    if( mask & ( mask - 1 ) ) return ( mask & ~PL_CRAFT_BITS ) ? 0 : PL_WORKSHOP;
    for( int k = 0; k < PL_KINDS; k++ )
      if( mask == ( 1u << k ) ) return PlKindOfIdx( k );
    return 0;
  }
  struct TPlWork {
    unsigned short funcMask[PL_FUNCS];
    unsigned char  funcNeed[PL_FUNCS];
    unsigned char  funcR4[PL_FUNCS];
    unsigned short pairMask[PL_PAIRS];
    unsigned short siteMask[PL_SITES];
    unsigned short siteCntK[PL_SITES][PL_KINDS];
    unsigned short siteCntG[PL_SITES];
    unsigned int   itemH[PL_ITEMS];
    unsigned short itemMask[PL_ITEMS];
    unsigned short itemN[PL_ITEMS];
    int nItem;
  };
  inline int PlWorkItem( TPlWork& w, unsigned int h ) {
    for( int j = 0; j < w.nItem; j++ )
      if( w.itemH[j] == h ) return j;
    return -1;
  }
  inline void PlVote( int& vote, bool& conflict, int k ) {
    if( !k ) return;
    if( vote && vote != k ) {
      if( PlCraftKind( vote ) && PlCraftKind( k ) ) { vote = PL_WORKSHOP; return; }
      conflict = true;
    }
    vote = k;
  }
  inline int PlLearnWorld( TPlState& st, TPlWork& w, const TPlMob* mobs, int n ) {
    if( !st.ready ) return 0;
    int learnedNow = 0;
    for( int round = 0; round < 3; round++ ) {
      memset( w.funcMask, 0, sizeof( w.funcMask ) );
      memset( w.funcNeed, 0, sizeof( w.funcNeed ) );
      memset( w.funcR4, 0, sizeof( w.funcR4 ) );
      memset( w.pairMask, 0, sizeof( w.pairMask ) );
      memset( w.siteMask, 0, sizeof( w.siteMask ) );
      memset( w.siteCntK, 0, sizeof( w.siteCntK ) );
      memset( w.siteCntG, 0, sizeof( w.siteCntG ) );
      w.nItem = 0;
      for( int i = 0; i < n; i++ ) {
        const TPlMob& m = mobs[i];
        const int k = PlEff( st, m );
        if( m.fallback && !k && m.func >= 0 ) w.funcNeed[m.func] = 1;
        if( !k || k == PL_OTHER || k == PL_REST ) continue;
        if( m.func >= 0 )
          w.funcMask[m.func] |= PlBit( k );
        if( m.item ) {
          int j = PlWorkItem( w, m.item );
          if( j < 0 ) {
            if( w.nItem >= PL_ITEMS ) { st.truncated++; continue; }
            j = w.nItem++;
            w.itemH[j] = m.item; w.itemMask[j] = 0; w.itemN[j] = 0;
          }
          w.itemMask[j] |= PlBit( k );
          if( w.itemN[j] < 65535 ) w.itemN[j]++;
        }
      }
      for( int fi = 0; fi < st.nFuncs; fi++ ) {
        const unsigned short fm = w.funcMask[fi];
        if( !fm ) continue;
        const TPlFunc& F = st.f[fi];
        for( int a = 0; a < F.nAsg; a++ )
          if( st.pairMenu[F.pair[a]] )
            w.pairMask[F.pair[a]] |= fm;
        for( int e = 0; e < F.nExt; e++ ) {
          const int s = F.site[e];
          w.siteMask[s] |= fm;
          for( int q = 0; q < PL_KINDS; q++ )
            if( ( fm & ( 1u << q ) ) && w.siteCntK[s][q] < 65535 ) w.siteCntK[s][q]++;
          if( ( fm & PL_CRAFT_BITS ) && w.siteCntG[s] < 65535 ) w.siteCntG[s]++;
        }
      }
      for( int i = 0; i < n; i++ ) {
        const TPlMob& m = mobs[i];
        if( !m.fallback || m.func < 0 || !m.named || !m.item || !w.funcNeed[m.func] ) continue;
        const int j = PlWorkItem( w, m.item );
        if( j < 0 || w.itemN[j] < 2 ) continue;
        const int k = PlOne( w.itemMask[j] );
        if( !k ) continue;
        unsigned char& v = w.funcR4[m.func];
        if( v == 0xFF ) continue;
        v = ( !v || v == k ) ? (unsigned char)k
          : ( PlCraftKind( v ) && PlCraftKind( k ) ) ? (unsigned char)PL_WORKSHOP : (unsigned char)0xFF;
      }
      bool changed = false;
      for( int fi = 0; fi < st.nFuncs; fi++ ) {
        TPlFunc& F = st.f[fi];
        if( F.learned || !w.funcNeed[fi] ) continue;
        if( w.funcMask[fi] && !PlOne( w.funcMask[fi] ) ) continue;
        int vote = 0;
        bool conflict = false, recipe = false;
        PlVote( vote, conflict, PlOne( w.funcMask[fi] ) );
        for( int a = 0; a < F.nAsg; a++ ) {
          const int p = F.pair[a];
          if( !st.pairMenu[p] ) continue;
          PlVote( vote, conflict, PlOne( w.pairMask[p] ) );
          if( st.pairRecipes[p] >= 4 ) recipe = true;
        }
        for( int e = 0; e < F.nExt; e++ ) {
          const int s = F.site[e];
          const unsigned short sm = w.siteMask[s];
          const int k = PlOne( sm );
          if( !k ) continue;
          const int c = ( PlBits( sm ) == 1 ) ? w.siteCntK[s][PlIdx( k )] : w.siteCntG[s];
          if( c >= 3 && st.siteCnt[s] <= 2 * c )
            PlVote( vote, conflict, k );
        }
        if( w.funcR4[fi] == 0xFF ) conflict = true;
        else PlVote( vote, conflict, w.funcR4[fi] );
        PlVote( vote, conflict, st.fprod[fi] );
        if( recipe ) {
          if( !vote ) vote = PL_WORKSHOP;
          else if( !PlCraftKind( vote ) ) conflict = true;
        }
        if( vote && !conflict ) {
          F.learned = (unsigned char)vote;
          learnedNow++;
          changed = true;
        }
      }
      if( !changed ) break;
    }
    return learnedNow;
  }
  inline unsigned int PlHash( const char* s ) {
    if( !s || !*s ) return 0;
    unsigned int h = 2166136261u;
    for( ; *s; s++ ) {
      unsigned char c = (unsigned char)*s;
      if( c >= 'a' && c <= 'z' ) c = (unsigned char)( c - 32 );
      h = ( h ^ c ) * 16777619u;
    }
    return h ? h : 1;
  }
  inline int PlLearnedTotal( const TPlState& st ) {
    int t = st.nItems;
    for( int i = 0; i < st.nFuncs; i++ )
      if( st.f[i].learned ) t++;
    return t;
  }
}
