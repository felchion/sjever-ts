#include <ansi.h>

inherit ITEM;

void create()
{
        set_name( HIC "明教简史" NOR, ({ "mingjiao jianshi", "jianshi", "book" }));
        set_weight(10);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "本");
                set("long",
"\n                 『明教简史』\n\n"
"明教源于波斯国，唐时传至中土。当时称为袄教。唐皇在各\n"
"处敕建大云兴明寺，为明教的寺院，明教教义是行善去恶，\n"
"众生平等，若有金银财物，须当救济贫众，不茹荤酒，崇拜\n"
"明尊。明尊即是火神，也即是善神。只因厉朝贪官污吏欺压\n"
"明教，明教教众不忿，往往起事，自北宋方腊方教主以来，\n"
"已不知有多少次了。在北宋期间的四大寇：方腊、宋江、王\n"
"庆、田虎，其中之一的方腊便是第一代教主。在南宋建炎年\n"
"间，王宗石教主在信州起事。绍定年间有余五婆教主在衢州\n"
"起事，理宗绍定年间有张三枪教主在江西、广东一带起事。\n"
"只因明教教素来和朝廷官府作对，朝廷官府便说明教是“魔\n"
"教”，严加禁止，明教在官府的逼迫之下为求生存，作事不\n"
"免隐秘诡怪，以避官府的耳目。正大门派和明教积怨成仇，\n"
"更是势如水火。明教这么大的一个教，教众之中也不免偶有\n"
"不自检点、为非作歹之徒，仗着武功了得，滥杀无辜者有之\n"
"奸淫掳掠者有之，所以明教的声誉便如江河之目下。但自大\n"
"宋亡于蒙古鞑子之手后，明教更成了朝廷之死敌，明教更把\n"
"驱除胡虏为已任，只可惜自阳顶天教主不明死因而亡后，明\n"
"群龙无守，教中诸高手为争教主之位，闹得自相开残杀，四\n"
"五裂，终于有的洗手归隐，有的另立门派，自任教主，教规\n"
"一堕之后，更与名门正派结的怨更深，之后才有六大门派围\n"
"攻光明顶之事。现在在张无忌教主的带领之下，由于对各大\n"
"有恩，与正派人等尽前嫌，成为一个正派的教。\n");
                set("value", 100);
                set("material", "paper");
        }
        setup();
}
