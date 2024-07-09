// sanshengtang.c (kunlun)

#include <ansi.h>
#include <room.h>

inherit ROOM;

void create()
{
         set("short", HIY"三圣堂"NOR);
         set("long",@long
昆仑三圣堂乃“昆仑三圣”何足道所建，原为了修身养性，读书练功。随
着昆仑派日益壮大，名声远播，这里便成了昆仑派聚众议事，接待各路江湖英
雄的地方。“昆仑三圣”也只好另觅安静所在了。
long);
         set("exits",([
             "north" : __DIR__"lang1",
             "southwest" : __DIR__"jiuqulang2",
         ]));

         set("objects",([
             CLASS_D("kunlun") + "/banshuxian.c" :   1,
             __DIR__"npc/xingfang.c" :   1,
             __DIR__"npc/kl-dizi3.c" :   1,
         ]));

         set("valid_startroom", 1);
         set("no_clean_up", 0);
         setup();
}
