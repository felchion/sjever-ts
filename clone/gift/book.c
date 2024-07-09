//奖励系统之书籍篇

#include <ansi.h>
inherit ITEM;

string long();
string *clr = ({RED"",GRN"",YEL"",BLU"",MAG"",CYN"",WHT"",HIR"",HIG"",HIY"",HIB"",HIM"",HIC"",HIW""});
string *str_skill_other = ({"taoism",                                           //武当：道学心法
                            "begging",                                          //丐帮：叫化绝活
                            "bangjue",                                          //丐帮：打狗棒口诀
                            "stealing",                                         //丐帮：妙手空空
                            "checking",                                         //丐帮：
                            "zhenqijue",                                        //华山：正气诀
                            "tiezhang-xinfa",                                   //铁掌：铁掌心法
                            "huanxi-chan",                                      //大轮寺：欢喜禅
                            "shenlong-yaoli",                                   //神龙岛：神龙药理
                            "dacheng-fofa",                                     //峨嵋：大乘佛法
                            "qimen-bagua",                                      //桃花岛：奇门八卦
                            "buddhism",                                         //天龙寺，少林：禅宗心法
                            "art",                                              //昆仑：琴棋书画
                            "daode-jing",                                       //九阴：道德经
                            "poison",						//基本毒计
                            "medicine",						//本草术理
                            "betting",						//赌术					
                            "sailing",						//航海术
                            "gunnery",						//炮弹术
                            "trade",						//讨价还价
                            "wuxing-zhen",					//公共阵法：五行阵
                            "zhenwu-qijiezhen",					//武当阵法
                            "dagou-zhen",					//丐帮打狗阵
                            "linji-jianzhen",					//娥眉剑阵
                            "shenghuo-fumozhen",				//明教圣火伏魔阵
                            "fumoquan-zhen",					//少林伏魔圈阵
                            "shenlong-wudizhen",				//神龙无敌阵
                            "qimen-baguazhen",					//奇门八卦阵
                            //Quest Skill
                            "jiuyin-zhengong",
                            "hamagong",
                            "kuihua-xinfa",
                            //Work Skill
                            "caikuang",
                            "nongsang",
                            "duanzao",
                            "zhizao",                            
                                                        });

void create()
{
        
        set_name(NOR + WHT "剑典残篇" NOR, ({ "jiandian book","book" }));
        if (clonep())
                set_default_object(__FILE__);
        else {
                set_weight(300);
                set("unit", "本");
                set("long", "这是一张残缺不全的书页。\n" NOR);
                set("value", 30);
                set("no_sell", "什么世道，就一张破纸头也拿来卖钱？");
                set("material", "paper");
                set("cloned",0);
                set("skill_name", "基本剑法");      // 研究技能的名称
                set("skill_type", "sword");         // 研究技能的种类
                set("power_point", 30000);          // 增加技能的点数
                set("need_exp", 300000);            // 研究所需的最低经验
                set("min_lvl", 100);                // 研究所需的最低等级
                set("max_lvl", 800);                // 能够研究的最高等级
        }
        setup();
}

void init()
{
                object me;
                object ob = this_object();
                mapping skills;
                string* sname;
                int i,me_level;
                
                add_action("do_read", "read");
                me=this_player();
                skills=me->query_skills();
                me_level=me->query("max_pot")-100;
                    
                if (environment(ob)==me && ob->query("gift_target")==me->query("id") && !query("cloned"))
                {
                if (mapp(skills)) sname  = keys(skills);
                sname = sname - str_skill_other;
                
                
                
                i=random(sizeof(sname));
                                
                set("skill_type", sname[i]);
                set("skill_name", to_chinese(sname[i]));
                if (me->query("combat_exp") < 10000)
                	set_name(clr[random(sizeof(clr))]+query("skill_name")+"入门指南"NOR,({query("skill_type")+"'s book","book"}));
                else if (me->query("combat_exp") > 10000000)
                	set_name(clr[random(sizeof(clr))]+query("skill_name")+"精要"NOR,({query("skill_type")+"'s book","book"}));
		else
                	set_name(clr[random(sizeof(clr))]+query("skill_name")+"残篇"NOR,({query("skill_type")+"'s book","book"}));
         
                set("need_exp",me->query("combat_exp")/1000000*1000000);
                if (me->query("combat_exp") < 10000){
		set("min_lvl",me_level);
                set("max_lvl",me_level+10);
		}                
                else{
                set("min_lvl",me_level-50);
                set("max_lvl",me_level+10);
                }
                set("power_point",(me_level+1)*(me_level+1)/6 + random(500));
                set("cloned",1);
         
                if (me->query("combat_exp") > 100000 && sname[i]=="literate")
                {
                        set("min_lvl",100);
                        set("max_lvl",300);
                        set("power_point",1000 + random(8000));
                }
        }
}

string long()
{
        string msg;

        if (query("power_point"))
        {
                msg = "这是一本" + name() + NOR + "，看样子"
                      "已经颇有年时了。\n页片上用隶体密密麻麻写满了蝇"
                      "头小字，似乎都和" + clr[random(sizeof(clr))] + query("skill_name") + NOR +
                      "有关，\n你可以试着读读(read)看，也许对武学能够"
                      "有所帮助。\n";
                msg += "在中间有一排依稀可辨的古篆字：\n【 "HIG + chinese_number(query("need_exp")) + "、"+
                chinese_number(query("min_lvl")) +"、" + chinese_number(query("max_lvl")) + "、" +
                chinese_number(query("power_point")) +NOR" 】\n\n";
        } else
                msg = "这是一张残缺不全的书页，可字迹已无法辨清了。\n";

        return msg;
}

int do_read(string arg)
{
        object me = this_player();

        int na = this_object()->name();
        string booksk = this_object()->query("skill_type");

        if (me->is_busy())
                return notify_fail("你现在正在忙。\n");

        if (me->is_fighting())
                return notify_fail("战斗中无法研读新知。\n");

        if (arg && id(arg))
        {
                if (me->query("combat_exp") < query("need_exp"))
                        return notify_fail("你觉得残页上记载的东西太过深奥，凭自"
                                           "己的武学似乎难以参详。\n");
                
                if (me->query_skill("literate",1) >= me->query("int")*10 && booksk=="literate")
                        return notify_fail("你因先天所制，已无法再进修更高深的学问了。\n");
                
                if (me->query_skill(booksk, 1) < query("min_lvl"))
                        return notify_fail("你觉得自己对这项技能的了解尚过浅薄，"
                                           "一时无法参详透彻。\n");

                if (me->query_skill(booksk, 1) >= query("max_lvl"))
                        return notify_fail("你觉得残页上记载的东西太过浅显，无法"
                                           "从中获得任何帮助。\n");

/*                if (! me->can_improve_skill(booksk))//此处应该加上不能让skills等级超过exp所能支持的上限
                                                    //can_improve_skill()定义写到/feature/skills.c我发现linwu居然也没限制？
                        return notify_fail("你参详了半天，发觉似乎是由于自身经验"
                                           "不足的缘故，很多东西无法理解。\n");
*/
                message_vision(HIW "$N" HIW "聚精会神地阅读「" NOR + na + HIW
                               "」上所记载的东西，似乎有所收获。\n" NOR, me);


                  me->improve_skill(lower_case(booksk), query("power_point"));

                tell_object(me, HIY "你只觉以前存在的诸多疑问豁然开朗，对「" +
                                query("skill_name") + "」有了新的感悟。\n" NOR);
                destruct(this_object());
                return 1;
        }
}
