// f.c
#include <ansi.h>
inherit F_CLEAN_UP;

int main(object me,string arg)
{
        //string str;
       	//string content;
        //int size;
        
        //sscanf(arg, "%s %d", content, size);
	//str = "/adm/daemons/text2pic_d.c"->Display(arg, size, "■", "△", HIG, NOR + HBBLU);
	//CHANNEL_D->do_channel( me, "rumor", str);
	object ob;
	ob = new("/d/wuguan/npc/dizi1.c");
	me->add_exp_combat(100,ob,"武馆1");
	destruct(ob);
        return 1;
}

