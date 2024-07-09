// danlu.c 丹炉

inherit ITEM;
inherit F_LIQUID;
#include <ansi.h>
#include "open.c"
#include "add.c"
#include "pour.c"
#include "close.c"
#include "fire.c"
#include "burn.c"
#include "aoyao.c"
#include "miehuo.c"
#include "qu.c"

int do_hehe(object me, object obj, object dest);
int do_haha(object ob, object obj);

void create()
{
       set_name("丹炉", ({ "dan lu", "lu", "yao lu" }) );
       set_weight(3000000);
       set_max_encumbrance(2000);

       if( clonep() )
               set_default_object(__FILE__);
      else {
               set("unit", "个");
               set("no_get", "丹炉对你来说太重了。\n");
               set("no_get_from", 1);
               set("long", "这是一个用来炼丹制药的丹炉。\n"NOR);
               set("value", 100000000);
               set("no_get",1);                 
               set("no_give",1);
               set("no_put",1);
               set("no_drop",1);           
               set("max_liquid", 100);
               set("material", "iron");
               set("marks/open", 0);
       }
       setup();
}
                   
int is_container() { return 1; }

void init()
{
        add_action("do_open", "open"); // open lid
        add_action("do_add",   "add"); // add xxx in lu
        add_action("do_pour", "pour"); // pour water
        add_action("do_close", "close"); // clone lid
        add_action("do_fire", "dianhuo"); // fire
        add_action("do_burn", "burn"); // burn coal  
        add_action("do_aoyao", "aoyao"); // aoyao
        add_action("do_miehuo", "miehuo"); // mie huo
        add_action("do_qu", "qu"); // qu yao from lu
        add_action("do_action", "put"); //forbid these two commands
}

int do_action(string arg)
{
        write("请用(add 药材名 in lu)命令向丹炉内添加药材。\n");
        write("请用(qu 药材名 from lu)命令从丹炉内拿出药材。\n");
        return 1;
}
