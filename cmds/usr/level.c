
#include <ansi.h>
inherit F_CLEAN_UP;

string *skill_level_desc = ({ 
	BLU "不堪一击" NOR,BLU "毫不足虑" NOR,BLU "不足挂齿" NOR,BLU "初学乍练" NOR,BLU "勉勉强强" NOR, 
	HIB "初窥门径" NOR,HIB "初出茅庐" NOR,HIB "略知一二" NOR,HIB "普普通通" NOR,HIB "平平淡淡" NOR, 
	MAG "平淡无奇" NOR,MAG "粗通皮毛" NOR,MAG "半生不熟" NOR,MAG "马马虎虎" NOR,MAG "略有小成" NOR, 
	HIM "已有小成" NOR,HIM "鹤立鸡群" NOR,HIM "驾轻就熟" NOR,HIM "青出于蓝" NOR,HIM "融会贯通" NOR, 
	CYN "心领神会" NOR,CYN "炉火纯青" NOR,CYN "了然于胸" NOR,CYN "略有大成" NOR,CYN "已有大成" NOR, 
	YEL "豁然贯通" NOR,YEL "出类拔萃" NOR,YEL "无可匹敌" NOR,YEL "技冠群雄" NOR,YEL "神乎其技" NOR, 
	HIY "出神入化" NOR,HIY "非同凡响" NOR,HIY "傲视群雄" NOR,HIY "登峰造极" NOR,HIY "无与伦比" NOR, 
	HIG "所向披靡" NOR,HIG "一代宗师" NOR,HIG "精深奥妙" NOR,HIG "神功盖世" NOR,HIG "举世无双" NOR, 
	HIC "惊世骇俗" NOR,HIC "撼天动地" NOR,HIC "震古铄今" NOR,HIC "超凡入圣" NOR,HIC "威镇寰宇" NOR, 
	HIW "空前绝后" NOR,HIW "天人合一" NOR,HIW "深藏不露" NOR,HIW "深不可测" NOR,HIR "返璞归真" NOR
});

	string *knowledge_level_desc = ({
        BLU "新学乍用" NOR,HIB "初窥门径" NOR,MAG "略知一二" NOR,HIM "半生不熟" NOR,
        CYN "马马虎虎" NOR,YEL "已有小成" NOR,YEL "融会贯通" NOR,HIY "心领神会" NOR,
        HIY "了然於胸" NOR,HIG "已有大成" NOR,HIG "非同凡响" NOR,HIC "举世无双" NOR,
        HIC "震古铄今" NOR,HIW "无与伦比" NOR,HIW "超凡入圣" NOR,HIR "空前绝后" NOR
});
int main(object me)
{

        string str = "";
        int i;
	
		i = sizeof(skill_level_desc);
                while(i--) {
                        str = "    " + skill_level_desc[i] + str;
                        if (i % 5 == 0) str = "\n" + str;
                }
                str = "\n\n技能的描述由低到高依次为："+str;
                i = sizeof(knowledge_level_desc);
                while(i--) {
                        str = "    " + knowledge_level_desc[i] + str;
                        if (i % 4 == 0) str = "\n" + str;
                }
                return notify_fail("知识的描述由低到高依次为："+str+"\n");
}
int help(object me)
{
        string str = "\n";
		string str2 = "\n";
        int i = sizeof(skill_level_desc);
		int j = sizeof(knowledge_level_desc);

        while(i--) {
                str = "    " + skill_level_desc[i] + str;
                if (i % 5 == 0) str = "\n" + str;
        }
		while(j--) {
                str2 = "    " + knowledge_level_desc[j] + str2;
                if (j % 4 == 0) str2 = "\n" + str2;
        }
write(@HELP
指令格式 : level
这个指令可以让你显示目前的技能的描述。
知识的描述由低到高依次为：
HELP);
        write(str2);
		write("\n技能的描述由低到高依次为：\n");
		write(str);

    return 1;
}