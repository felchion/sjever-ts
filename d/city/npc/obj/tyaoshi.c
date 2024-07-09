// by XiaoYao

#include <ansi.h>
inherit ITEM;

void create()
{
  set_name(YEL"铜钥匙"NOR, ({"tong yaoshi", "key", "yaoshi"}));
  set_weight(200);
  if (clonep())
    set_default_object(__FILE__);
  else {
    set("unit", "把");
    set("long", "这看起来似乎是一把闺房的钥匙，但不知道派什么用。\n");
    set("value", 200);
    set("material", "bronze");
    set("unique", 1);
    set("no_get",1);
    set("no_drop",1);
    set("no_get_from",1);
    
  }
  setup();
}
