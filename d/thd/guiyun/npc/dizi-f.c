// /u/jpei/guiyun/npc/dizi-f.c

inherit NPC;

void create()
{
	int name_no = random(3); 
	string *name_string = ({"文玉燕","李芳姑","石翠翠"});

	set_name("女弟子", ({"nv dizi", "nv", "dizi"}) );
	set("long", "她正在专心致志地练习功夫。\n");
	set("gender", "女性" );
	set("age", 23);
	set("attitude", "peaceful");
	set("str",18);
	set("int",28);
	set("con",19);
	set("dex",22);
	set("max_qi", 650);
	set("max_jing", 250);
	set("neili", 500);
	set("max_neili", 320);
	set("jiali", 10);
	set("combat_exp", 15000);

	set_skill("force", 30);
	set_skill("bihai-chaosheng", 27);
	set_skill("dodge", 30);
	set_skill("suibo-zhuliu", 28);
	set_skill("parry", 32);
	set_skill("strike", 22);
	set_skill("luoying-zhang", 18);
	set_skill("leg", 20);
	set_skill("xuanfeng-tui", 15);
	set_skill("sword", 30);
	set_skill("yuxiao-jian", 25);
	set_skill("taoism", 20);
	set_skill("literate", 35);
	set_skill("qimen-bagua", 40);

	map_skill("force", "bihai-chaosheng");
	map_skill("dodge", "suibo-zhuliu");
	map_skill("strike", "luoying-zhang");
	map_skill("leg", "xuanfeng-tui");
	map_skill("parry", "luoying-zhang");
	map_skill("sword", "yuxiao-jian");
	prepare_skill("strike", "luoying-zhang");

	set("shen_type", 0);
    create_family("桃花岛", 4, "弟子");
	set("class", "fighter");

	set("inquiry", ([
		"name": "奴家叫做" + name_string[name_no] + "，从十五岁起便投在这里学艺。",
		"rumors": "听说师祖收徒很重视悟性和文化，我得多学些读书写字了。",
		"here": "这里是归云庄，你随便转转吧，累了请到客房休息。",
		"东邪": "那是师祖呀，，蒙他老人家恩准，我才能跟陆庄主学艺。",
		"黄药师": "那是师祖呀，，蒙他老人家恩准，我才能跟陆庄主学艺。",
		"寒雨": "说起来也是他创造了我，不过这家伙是个造ｑｕｅｓｔ狂。",
		"jpei": "说起来也是他创造了我，不过这家伙是个造ｑｕｅｓｔ狂。",
		"桃花岛": "听说是师祖住的地方，可惜没有去看过。",
		"陆乘风": "是我的师父，这里的庄主，你找他老人家有什么事吗？",
		"归云庄": "听说这里是花了庄主无数心血才建起来的，与别的庄院相比如何？",
		"太湖": "就在庄前不远，是一个烟波浩淼的大湖。",
	]) );

	setup();

	carry_object("/clone/misc/cloth.c")->wear();
}
