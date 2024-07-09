// add quest by yanyang 2000/8/25 , about a armor


inherit ROOM;
#include <ansi.h>
string look_bei();
void close_passage();
void create()
{
        set("short", HIR"天然洞穴"NOR);
        set("long", @LONG
这石洞系天然生成，较之外面人工开凿的石室大了十来倍。放眼望去，洞
内共有十余具骸骨，或做或卧，神态各有不同，有的骸骨散开在地，有的却仍
具完好人形，更有些骨坛灵位之属。每具骸骨之旁都放着兵刃，暗器，用具，
珍宝等物。洞口处立着一座石碑(bei)。
LONG
        );

        set("exits", ([
            "out" : __DIR__"dong-3",
        ]));

        set("item_desc",([
        "bei"         :       (: look_bei :),
        ]));

        set("objects", ([
            __DIR__"obj/haigu" : 1,  
//            __DIR__"obj/ycj" : 1,  by tangfeng
        ]));

        set("no_clean_up", 0);
        setup();

}
string look_bei()
{
        return
        "\n"
"    ?  擅动室内物品者\n"    
        "\n"
HIR"        死伤莫怪！    \n"NOR
        "\n";
}

void init()
{
        add_action("do_move", "move");
        add_action("do_find", "find");
}

int do_move(string arg)
{
      object me;
      me=this_player();

      if ((int)me->query_temp("marks/baowu")) 
      {
      if ((int)me->query_str() < 30)
          return notify_fail("你的力量不够，无法将石碑推开。\n");
      if (!arg || arg !="bei") 
          return notify_fail("你要推什么？？\n");
      write("你扎下马步，深深的吸了口气，将石碑缓缓的向旁推开。\n");
      message("vision",me->name() + "双膀用力，气沉丹田，石碑缓缓向后移开，下面露出一个黑洞洞的入口。\n", environment(me),({me}));
      set("exits/enter", __DIR__"taijie-1");
      remove_call_out("close_passage");
      call_out("close_passage", 5);
      return 1;
      }
      return notify_fail("找死啊！\n");
}

int do_find(string arg)
{
      object me=this_player();
      object kaijia;
      mapping fam = me->query("family");
      
      if(!fam || fam["family_name"] != "铁掌帮" || fam["master_name"] != "上官剑南" || !me->query_temp("marks/baowu")){
        write(HIR"铁掌重地，擅动室内物品，死伤莫怪！\n"NOR);
        return 1;
        }

      if (!arg || arg != "baowu")
        return notify_fail("你要找什么？\n");
      if(me->query("jingli")<100) return notify_fail("你没有精力再找东西了！\n");
      if(me->is_busy()) return notify_fail("你现在心情还没有平静下来！\n");
      if(random(10)>6) {
        me->start_busy(2);
        return notify_fail(BLU"突然，你发现身边的一具骸骨好像动了一下，你被吓出了一身冷汗！\n"NOR);
        }
      if(random(10)!=6) {
        me->add("jingli",-100);
        return notify_fail("你小心翼翼在遗骸中仔细的寻找着。\n"); 
        }
      kaijia = unew(ARMOR_D("yuxue-kaijia"));
      if(!clonep(kaijia)) { 
        write("结果，找了半天，你没找到一件象样的东西出来。\n");
        return 1;
        }  
      write("终于，你在一具骸骨旁发现一件血红色的铠甲。\n");
      message_vision("$N将铠甲小心的拿起，揩掉上面的泥土。\n",me);
      kaijia->move(me);
      return 1;
}

void close_passage()
{
      if( !query("exits/enter") ) return;
      message("vision", "石碑发出隆隆的声音，缓缓移回原处。\n", this_object() );
      delete("exits/enter");
}
