// Room: /d/wizard/wizard_room.c

inherit ROOM;
#include <ansi.h>
#include <mudlib.h>

string area_mulu();

void create()
{
	set("short", "巫师休息室");
	set("long",
"这里就是" + CHINESE_MUD_NAME +"巫师的休息所在，早粉刷过的四周墙壁上到\n"
"处挂着"HIC"一百元大票"NOR"，屋子的正中央放着一张"HIY"纯金桌"NOR"，上面摆着一本厚厚的\n"
"巫师留言簿(board)，房间里没有椅子，巫师只能站着发呆了。\n"
HIW"巫师请调整负责的目录(mulu)\n"NOR
);
	set("exits", ([
      		"up" : __DIR__"zoulang",
      		"enter" : "/adm/etc/admin_room",
	  	"down" : __DIR__"guest_room",
		"east" : __DIR__"wizard_meet"
	]));
	set("item_desc",([
		"mulu" : (: area_mulu :),
        ]));

	set("no_save", 1);
	set("no_fight", 1);
	setup();
	call_other("/clone/board/wiz_b", "???");
}

void init()
{
	object ob = this_player();

  	if (userp(ob) && !wiz_level(ob)) {
		ob->move("/d/wizard/guest_room");
		message_vision("$N从巫师休息室重重地摔了下来。\n", ob);
		ob->unconcious();
	}
}

string area_mulu()
{
	write("\n
各门派按所处地分配，统一使用地名，而不是门派名。
比如，  murong派 应该在 /area/jiangnan/yanziwu
	mingjiao 应该在 /area/kunlun/guangmingding
	kunlun派 应该在 /area/kunlun/xxxshan...

现在划分 area 为：

	昆仑(kunlun)：	昆仑山脉地域。
			包括明教，昆仑派，雪山派(剑派)，密宗等。
	西域(xiyu)：	从嘉峪关开始往西地域。
			包括星宿海，伊犁，白陀山，天山，嘉峪关等。
	西南(xinan)：	秦岭以南，云贵川三地。
			包括娥眉，大理，天龙寺，青城，苗疆，无量山，五指山，成都等。
	东北(dongbei)：	山海关外所有区域。
			包括鹿鼎山，雅各布等。");

	return "
	草原(caoyuan)：	西域和北域。
			包括回疆，西夏，蒙古等。
	江南(jiangnan)：长江以南，不含云贵川的所有区域。
			包括苏州，嘉兴，杭州，泉州，衡山，天台山，燕子坞，梅庄，天地会总舵，
			福州，佛山，归云庄，等。
	江北(jiangbei)：长江以北，黄河以南的中间地带。
			包括泰山，嵩山，武当山，襄阳，华山，崆峒山，扬州，长安，终南山，金陵等。
	河北(hebei)：	黄河以北，直到山海关间的所有区域。
			包括北京，王屋山，恒山，五台山，山海关，天龙门等。
	海外(haiwai)：	所有海岛，以及出了中华地域的区域。
			包括莫斯科，波斯，扶桑，冰火岛，桃花岛，侠客岛等。
	特殊(special)：	所有特殊区域，不在map搜索范围内。
			包括地狱，巫师地带，擂台等。

        所有目录已在 test site 建立，各巫师请按各自负责的门派改动，老目录为 /oldd。\n";
}
int valid_enter(int t)
{
	return 0;
}
