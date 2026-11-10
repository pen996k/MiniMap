namespace GOTHIC_NAMESPACE
{
#include "MiniMap.h"

	void __fastcall oCGame_Init( oCGame* self, void* vtable );
	auto Hook_oCGame_Init = Union::CreateHook( SIGNATURE_OF( &oCGame::Init ), &oCGame_Init, Union::HookType::Hook_Detours );
	void __fastcall oCGame_Init( oCGame* self, void* vtable ) { Hook_oCGame_Init( self, vtable ); MM_LOG( "=== MiniMap loaded (union-framework) ===" ); }

	void __fastcall CGameManager_Done( CGameManager* self, void* vtable );
	auto Hook_CGameManager_Done = Union::CreateHook( SIGNATURE_OF( &CGameManager::Done ), &CGameManager_Done, Union::HookType::Hook_Detours );
	void __fastcall CGameManager_Done( CGameManager* self, void* vtable ) { MiniMap::SaveMaps(); MiniMap::Shutdown(); Hook_CGameManager_Done( self, vtable ); }

	void __fastcall oCGame_Render( oCGame* self, void* vtable );
	auto Hook_oCGame_Render = Union::CreateHook( SIGNATURE_OF( &oCGame::Render ), &oCGame_Render, Union::HookType::Hook_Detours );
	void __fastcall oCGame_Render( oCGame* self, void* vtable ) { MiniMap::EatKeysEarly(); Hook_oCGame_Render( self, vtable ); }

	void __fastcall oCGame_MainWorld_Render( Union::Registers& reg );
	auto Partial_zCWorld_Render = Union::CreatePartialHook( reinterpret_cast<void*>( zSwitch( 0x0063DC76, 0x0066498B, 0x0066BA76, 0x006C87EB ) ), &oCGame_MainWorld_Render );
	void __fastcall oCGame_MainWorld_Render( Union::Registers& reg ) { MiniMap::Update(); }

	void __fastcall zCMenu_Render( zCMenu* self, void* vtable );
	auto Hook_zCMenu_Render = Union::CreateHook( SIGNATURE_OF( &zCMenu::Render ), &zCMenu_Render, Union::HookType::Hook_Detours );
	void __fastcall zCMenu_Render( zCMenu* self, void* vtable ) { Hook_zCMenu_Render( self, vtable ); MiniMap::OnMenu(); }

	void __fastcall oCGame_WriteSaveGame( oCGame* self, void* vtable, int slot, zBOOL saveGlobals );
	auto Hook_oCGame_WriteSaveGame = Union::CreateHook( SIGNATURE_OF( &oCGame::WriteSavegame ), &oCGame_WriteSaveGame, Union::HookType::Hook_Detours );
	void __fastcall oCGame_WriteSaveGame( oCGame* self, void* vtable, int slot, zBOOL saveGlobals ) { MiniMap::Detach(); Hook_oCGame_WriteSaveGame( self, vtable, slot, saveGlobals ); }

	void __fastcall oCGame_LoadGame( oCGame* self, void* vtable, int slot, const zSTRING& levelPath );
	auto Hook_oCGame_LoadGame = Union::CreateHook( SIGNATURE_OF( &oCGame::LoadGame ), &oCGame_LoadGame, Union::HookType::Hook_Detours );
	void __fastcall oCGame_LoadGame( oCGame* self, void* vtable, int slot, const zSTRING& levelPath ) { MiniMap::Detach(); Hook_oCGame_LoadGame( self, vtable, slot, levelPath ); }

	void __fastcall oCGame_LoadSaveGame( oCGame* self, void* vtable, int slot, zBOOL loadGlobals );
	auto Hook_oCGame_LoadSaveGame = Union::CreateHook( SIGNATURE_OF( &oCGame::LoadSavegame ), &oCGame_LoadSaveGame, Union::HookType::Hook_Detours );
	void __fastcall oCGame_LoadSaveGame( oCGame* self, void* vtable, int slot, zBOOL loadGlobals ) { MiniMap::Detach(); Hook_oCGame_LoadSaveGame( self, vtable, slot, loadGlobals ); }

	void __fastcall oCGame_ChangeLevel( oCGame* self, void* vtable, const zSTRING& levelpath, const zSTRING& startpoint );
	auto Hook_oCGame_ChangeLevel = Union::CreateHook( SIGNATURE_OF( &oCGame::ChangeLevel ), &oCGame_ChangeLevel, Union::HookType::Hook_Detours );
	void __fastcall oCGame_ChangeLevel( oCGame* self, void* vtable, const zSTRING& levelpath, const zSTRING& startpoint ) { MiniMap::Detach(); Hook_oCGame_ChangeLevel( self, vtable, levelpath, startpoint ); }

	void __fastcall CGameManager_ApplySomeSettings( CGameManager* self, void* vtable );
	auto Hook_CGameManager_ApplySomeSettings = Union::CreateHook( SIGNATURE_OF( &CGameManager::ApplySomeSettings ), &CGameManager_ApplySomeSettings, Union::HookType::Hook_Detours );
	void __fastcall CGameManager_ApplySomeSettings( CGameManager* self, void* vtable ) { Hook_CGameManager_ApplySomeSettings( self, vtable ); MiniMap::OnOptionsApplied(); }

	void __cdecl oCVob_ShowVobDebugInfo( zCCamera* cam );
	auto Hook_oCVob_ShowVobDebugInfo = Union::CreateHook( reinterpret_cast<void*>( MiniMap::ADDR_ShowVobDebugInfo ), &oCVob_ShowVobDebugInfo, Union::HookType::Hook_Detours );
	void __cdecl oCVob_ShowVobDebugInfo( zCCamera* cam ) { Hook_oCVob_ShowVobDebugInfo( cam ); if( MiniMap::LatePaintNow() ) MiniMap::Paint(); }

	void __fastcall zCView_Blit( zCView* self, void* vtable );
	auto Hook_zCView_Blit = Union::CreateHook( SIGNATURE_OF( &zCView::Blit ), &zCView_Blit, Union::HookType::Hook_Detours );
	void __fastcall zCView_Blit( zCView* self, void* vtable ) { Hook_zCView_Blit( self, vtable ); if( self == screen && MiniMap::BlitPaintNow() ) MiniMap::Paint(); }
}
