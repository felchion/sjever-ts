inherit ITEM;
inherit F_LIQUID;

void create()
{
        set_name("茶壶", ({ "cha hu", "hu" }) );
        set_weight(400);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("long", "这是一个棕色的瓷茶壶。\n"
);
                set("unit", "个");
                set("value", 80);
                set("max_liquid", 40);
        }

        set("liquid", ([
                "type": "water",
                "name": "茶水",
                "remaining": 40,
        ]) );
}

