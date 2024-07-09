#include <ansi.h>
inherit ITEM;

void create()
{
	set_name(HIR "书剑通商令" NOR, ({ "shang ling", "shang", "ling" }));
	set_weight(1);
	if (clonep())
	set("long", "这是一张红底金边的书剑通商令，真是一令在手，全国各地摆摊不愁。
上面有一排篆头小字：
stock    『物品ID』 『价格』  把物品摆上摊位
unstrock 『物品ID』          把物品从摊位上撤下来
注：价格以黄金为单位\n" );
	set("unit", "张");
	set("value", 5);
	set("material", "paper");
	set("no_sell", "这年头，你把吃饭的家伙也拿来卖钱？");
	setup();
}

void init()
{
       add_action("do_stock", "stock");
       add_action("do_unstock", "unstock");
}

int do_stock(string arg)
{
        object me,goods;
        int amount, value;
        mapping all_goods;

        me = this_player();

        if (! me->query("is_vendor"))
                return notify_fail("只有从事商业的人才能摆摊。\n");

        if (! present("shang ling", me))
                return notify_fail("你的书剑通商令不在身上，如今世道乱，小心为妙。\n");

        if (! me->query_temp("on_baitan"))
                return notify_fail("你必须首先摆个摊位(baitan)才能兜售货物。\n");

        if (! arg)
                return notify_fail("指令格式：stock <货物> <多少黄金>\n");
	
	if(sscanf(arg,"%s %d",arg,amount) !=2 )
		return notify_fail("指令格式：stock <货物> <多少黄金>\n");
	
	
//        i = sizeof(args = explode(arg, " "));
//
//        i--;
//        if (! sscanf(args[i], "%d", amount)) 
//                amount = 0;
//        else
//                arg = replace_string(arg, " " + amount, "");

        if (! (goods = present(arg, me)) || ! objectp(goods))
                return notify_fail("你身上并没有这个货物。\n");

        if (goods->is_character())
                return notify_fail("你不能贩卖活物。\n");

        if (goods->query("money_id"))
                return notify_fail("你打算把钱也拿来出售？\n");

        if (goods->query("id") == "shang ling")
                return notify_fail("把这东西卖了，你靠什么来吃饭？\n");

        if (goods->is_item_make())
                return notify_fail("这东西是定了名的，相信没人愿去买。\n");

        if (sizeof(me->query_temp("vendor_goods")) >= 10)
                return notify_fail("你一次只能摆十种货物，把别的先收点起来吧。\n");

        if (amount > 1000)
                return notify_fail("物品出售价格最多定为一千锭黄金。\n");
        
        if (amount < 0)
                return notify_fail("物品出售价格不能小于一锭黄金。\n");

        value = 10000;

//        if (! value)
//                value = goods->query("value");

        if (amount)
        { 
                value = amount * value;
        }

//        if (! value)
//                return notify_fail("这东西不值钱，没人会去买的。\n");

        all_goods = me->query_temp("vendor_goods");

        if (! all_goods)
                all_goods = ([ ]);

        all_goods[base_name(goods)] = value;
        me->set_temp("vendor_goods", all_goods);
        message_vision("$N将"+ goods->name(1) + NOR"定为" +
                       MONEY_D->price_str(value) + NOR"的价格摆上摊子开始出售。\n" NOR, me);
        return 1;
}

int do_unstock(string arg)
{
        object me, goods;
        int i, have_it = 0;
        mapping all_goods;
        string *goods_key;
        
        me = this_player();

        if (! me->query("is_vendor"))
                return notify_fail("只有从事商业的人才能摆摊。\n");

        if (! me->query_temp("on_baitan"))
                return notify_fail("你目前并没有摆摊。\n");

        if (! arg)
                return notify_fail("unstock <货物>\n");

        if (! (goods = present(arg, me)))
                return notify_fail("你并没有兜售这个货物。\n");

        all_goods = me->query_temp("vendor_goods");

        if (! all_goods)
                return notify_fail("你现在并没有兜售任何货物。\n");

        goods_key = keys(all_goods);

        for (i = 0; i < sizeof(goods_key); i++)
        {
                if (base_name(goods) == goods_key[i])
                        have_it = 1;
        }

        if (have_it) 
        {
                map_delete(all_goods, base_name(goods));
                message_vision(CYN"$N将" + goods->name(1) +CYN+
                               "从摊子上收起不卖了。\n"NOR, me);
        } else
                return notify_fail("你并没有兜售这个货物。\n");

        me->set_temp("vendor_goods", all_goods);
        return 1;
}

int query_autoload()
{
        return 1;
} 
