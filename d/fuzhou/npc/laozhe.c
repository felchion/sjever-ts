// laozhe.c 药铺老头

inherit NPC;
inherit F_VENDOR;

void create()
{
        set_name("老者", ({ "lao zhe", "lao", "zhe" }));
        set("str", 20);
        set("gender", "男性");
        set("age", 68);
        set("long", "这是一个老中医，面色红润，白发如雪。\n");
        set("combat_exp", 68250+random(40000));
        set("attitude", "friendly");
	set("inquiry", ([
                "林震南" : "林镖头嘛，最近可是很少见。\n",
                "name" : "老头我是姓纪的，名字你就不必知道了吧。\n",
                "here" : "福州是个港口，会行船的人很多。\n",
	]));
        set("vendor_goods", ({
		(["name":MEDICINE_D("jinchuang"),"number":25]),
		(["name":MEDICINE_D("yangjing"),"number": 10]),
		(["name":MEDICINE_D("baicao"),"number": 2]),
        }));
        setup();
        carry_object(ARMOR_D("cloth"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
}

void init()
{
        add_action("do_list", "list");
        add_action("do_buy", "buy");
}
