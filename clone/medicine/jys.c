#include <ansi.h>
inherit COMBINED_ITEM;

void create()
{
   set_name("金元散", ({"jinyuan san", "jinyuan","san"}));
   if (clonep())
      set_default_object(__FILE__);
   else {
      set("value", 5000);
      set("unit", "些");
      set("long","这是一份采集灵草制炼而成之药，对常见疾病具有神效。\n");
      set("base_value", 5000);
      set("base_unit", "份");
      set("base_weight", 30);
   }
   set_amount(1);
}

int init()
{
   add_action("do_eat", "fu");
}

int do_eat(string arg)
{
   object me = this_player();

   if(!id(arg))
       return notify_fail("你要吃什么？\n");

   if (!me->query_condition("ill_kesou") && !me->query_condition("ill_shanghan") 
   && !me->query_condition("ill_zhongshu") && !me->query_condition("ill_dongshang")
   && !me->query_condition("ill_fashao"))
           return notify_fail("你现在不需要用金元散。\n");
     
        if(me->query_condition("ill_kesou") > 5)
        {
           me->apply_condition("ill_kesou", me->query_condition("ill_kesou")-5);
           message_vision("$N服下一份金元散，感觉咳嗽一下子好了很多。\n", me);
           add_amount(-1);
           return 1;
        }
        else if(me->query_condition("ill_kesou") > 0) {
           me->clear_condition("ill_kesou");
           message_vision("$N服下一份金元散，感觉咳嗽已经完全好了。\n", me);
           add_amount(-1);
           return 1;
        }
        
        if(me->query_condition("ill_shanghan") > 5)
        {
           me->apply_condition("ill_shanghan", me->query_condition("ill_shanghan")-5);
           message_vision("$N服下一份金元散，感觉伤寒一下子减轻许多。\n", me);
           add_amount(-1);
           return 1;
        }
        else if(me->query_condition("ill_shanghan") > 0) {
           me->clear_condition("ill_shanghan");
           message_vision("$N服下一份金元散，伤寒病已经完全治愈了。\n", me);
           add_amount(-1);
           return 1;
        }
        
        if(me->query_condition("ill_zhongshu") > 5)
        {
           me->apply_condition("ill_zhongshu", me->query_condition("ill_zhongshu")-5);
           message_vision("$N服下一份金元散，夏日的中暑症状减轻许多。\n", me);
           add_amount(-1);
           return 1;
        }
        else if(me->query_condition("ill_zhongshu") > 0) {
           me->clear_condition("ill_zhongshu");
           message_vision("$N服下一份金元散，中暑已经完全治愈了。\n", me);
           add_amount(-1);
           return 1;
        }
        
        if(me->query_condition("ill_dongshang") > 5)
        {
           me->apply_condition("ill_dongshang", me->query_condition("ill_dongshang")-5);
           message_vision("$N服下一份金元散，感觉冻伤一下子好了很多。\n", me);
           add_amount(-1);
           return 1;
        }
        else if(me->query_condition("ill_dongshang") > 0) {
           me->clear_condition("ill_dongshang");
           message_vision("$N服下一份金元散，冻伤已经完全治愈了。\n", me);
           add_amount(-1);
           return 1;
        }
        
        if(me->query_condition("ill_fashao") > 5)
        {
           me->apply_condition("ill_fashao", me->query_condition("ill_fashao")-5);
           message_vision("$N服下一份金元散，感觉发烧一下子好了很多。\n", me);
           add_amount(-1);
           return 1;
        }
        else if(me->query_condition("ill_fashao") > 0) {
           me->clear_condition("ill_fashao");
           message_vision("$N服下一份金元散，发烧已经完全治愈了，体温也恢复到正常。\n", me);
           add_amount(-1);
           return 1;
        }
        
}
