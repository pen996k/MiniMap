META
{
  Parser = Menu;
  After  = zUnionMenu.d;
};

instance MENUITEM_UNION_AUTO_MINIMAP(C_MENU_ITEM_UNION_DEF)
{
  onselaction[0]   = SEL_ACTION_STARTMENU;
  onselaction_s[0] = "MENU_OPT_MINIMAP";
  flags            = flags | IT_TXT_CENTER;
  text[0]          = "MiniMap";
  text[1]          = "Minimapa i duza mapa swiata.";
};
