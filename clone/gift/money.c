//奖励系统之金钱篇
#include <ansi.h>
#include <get_place.h>

inherit ITEM;
int check(object ob);

string *no_kill_list = ({
         "mu ren", "jin ren", "yin ren", "zhong shentong", "zhu cong", "shi ren", "huo du","he shiwo"
});


void create()
{
    set_name(HIW "锦盒" NOR, ({ "jin he","he","jinhe"}));
        set_weight(1000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "个");
                set("long", 
                        "一个檀木做成的古香古色的小盒子。\n"
                        "你可以检查(jiancha)它，看看里面有什么秘密。\n"
                        );
                set("no_sell", "我的天…你…你知道这是啥么？这你也敢拿来卖？");
                set("material", "wood");
                set("cloned",0);
                set("viewed",0);
                set("no_drop",1);
             }
        setup();
}

void init()
{

        object *living;
        object target, objwhere;
        string str;
        int i,j;
        
        if(!query("cloned")){
        living = livings();
        for (i=0;i < sizeof(living);i++){
                j = random(sizeof(living));
                if (!j || j == sizeof(living) -1) continue;
                if(check(living[j]) ){
                        target = living[j];
                        objwhere = environment(target);
                        str = objwhere->query("short");
                        if (!sizeof(objwhere->query("exits"))
                        || str == "武僧堂" 
                        || str == HIR"心禅堂"NOR
                        || str == "金刚伏魔圈" ) continue;
                        if( get_place(base_name(environment(target)))!="" ) break;
                }
        } 
        
        set("cwheredig",get_place(base_name(environment(target)))+environment(target)->query("short"));
        set("wheredig",base_name(environment(target)));
        set("cloned",1);
        }
        add_action("do_dig", "dig");
        add_action("do_dig", "search");
        add_action("do_jiancha", "jiancha");
}

int do_jiancha(string arg)
{
        object me=this_player();
        object ob=this_object();
        string bzhere;
        if(arg!="锦盒" && arg!="jing he" && arg!="he" && arg!="jinhe") return notify_fail("你要检查什么？\n");
        
        if(!query("viewed")) //该物品还没有被检查
        {
                
                if(me->query("id")!=query("gift_target")){//如果拿到的是别人的盒子
                        message_vision("$N手脚僵硬的搞鼓着一个锦盒。\n", me);
                        if (random(me->query("kar")) > 10 && random(100) < 50){
                        message_vision("$N东摸西摸，无意中碰到一个机关，,\"啪\"的一声，盒子的夹层打了开来。\n", me);
                        bzhere=ob->query("cwheredig");
                        set("viewed",1);
                        set("long","一个檀木做成的古香古色的小盒子，里面的夹层已经打开。\n"
                                           "一张发黄的纸条上写到模糊的几个字:\n"
                                           "吾纵横江湖时曾在"+HIY+bzhere+NOR+"留下些许物事，待有缘人前去挖掘(dig)。\n");

                        return notify_fail( "一张发黄的纸条上写到模糊的几个字:\n"
                                           "吾纵横江湖时曾在"+HIY+bzhere+NOR+"留下些许物事，待有缘人前去挖掘(dig)。\n");
                        }
                }
                else{
                message_vision("$N反复看着盒子，想找出里面有什么秘密。\n", me);
                if(random (100) > 50)//是本人的盒子
                {
                        message_vision("$N心领神会，轻手熟路的在按钮上一按，盒子的夹层打了开来。\n", me);
                        bzhere=ob->query("cwheredig");
                        set("viewed",1);
                        set("long","一个檀木做成的古香古色的小盒子，里面的夹层已经打开。\n"
                                           "一张发黄的纸条上写到模糊的几个字:\n"
                                           "吾纵横江湖时曾在"+HIY+bzhere+NOR+"留下些许物事，待有缘人前去挖掘(dig)。\n");
                        return notify_fail( "一张发黄的纸条上写到模糊的几个字:\n"
                                           "吾纵横江湖时曾在"+HIY+bzhere+NOR+"留下些许物事，待有缘人前去挖掘(dig)。\n");
                } 
                }
                return notify_fail("你看了半天，也没有明白这盒子到底是怎么回事。\n");                                       
        }
        return notify_fail("盒子的夹层已经打开，你可以仔细看盒子（look jinhe）然后采取相应行动。\n");
}

int do_dig(string arg)
{
        object me=this_player();
        object ob=this_object();
        int bound;
        object money;

        if(me->is_fighting()) return notify_fail("战斗中还要挖？真是找死阿！。\n");
        //if(arg!="宝藏" && arg!="baozang" && arg!="caibao" && arg!="here") return 0;
        if(base_name(environment(me))!=ob->query("wheredig")) return 0;
                             
        /*
        if(random(me->query("kar")<15)) return 0;
        
        if( random(me->query_kar()) > 29 && me->query("user_weapon_imbued") > 1) {
                message_vision(HIG"$N突然挖到一把锈迹斑驳的铁剑，好像很古老的样子。\n"NOR,me);
                message_vision(HIG"$N整个人感觉豁然开朗，忍不住想马上去韦兰那里打造一次。\n"NOR,me);
                 CHANNEL_D->do_channel( this_object(), "rumor",sprintf("有人看到%s获得一次重新"+HIY+"打造"+HIM+"的机会！", me->name(1)));
                me->add("user_weapon_imbued",-1);
                log_file("job/PRIZE", sprintf("%8s%s获得打造机会的奖励。\n", ctime(time())[4..19],
                me->name(1)+"("+me->query("id")+")")  );
                destruct(this_object());
                return 1;       
                
        }
        if(random(100) == 0 && me->query("zhou/fail") > 1) {
                message_vision(HIG"$N突然挖到一把残旧的书籍，上面写着一些稀奇古怪的符号。\n"NOR,me);
                message_vision(HIG"$N整个人感觉豁然开朗，感觉自己离九阴真经又进了一步。\n"NOR,me);
                me->add("zhou/fail",-1);
                 CHANNEL_D->do_channel( this_object(), "rumor",sprintf("有人看到%s获得一次重新解"+HIY+"九阴"+HIM+"的机会！", me->name(1)));
                log_file("job/PRIZE", sprintf("%8s%s获得重新解九阴上的奖励。\n", ctime(time())[4..19],
                me->name(1)+"("+me->query("id")+")")  );
               	destruct(this_object());
               	return 1;       
                
        }      
        if(random(100) == 0 && me->query("zhou/failed") > 1) {
                message_vision(HIG"$N突然挖到一些稀奇古怪的东西，感觉自己又笨又傻。\n"NOR,me);
                message_vision(HIG"$N突然自己告诉自己，我这么笨，为什么不去找老顽通，也许他现在改变对我的看法了呢？。\n"NOR,me);
                me->add("zhou/failed",-1);
                CHANNEL_D->do_channel( this_object(), "rumor",sprintf("有人看到%s获得一次重新解"+HIY+"互搏"+HIM+"的机会！", me->name(1)));
                log_file("job/PRIZE", sprintf("%8s%s获得重新解互搏的奖励。\n", ctime(time())[4..19],
                me->name(1)+"("+me->query("id")+")")  );
                destruct(this_object());
                return 1;       
                
        }       
        if(random(100) > 90) {
                book = new("/clone/gift/book");
               	book->set("gift_target",me->query("id"));
               	book->move(me);
               	message_vision(CYN"$N不停的在这里挖呀挖。\n"NOR,me);
                message_vision(CYN"$N突然挖到一本"+book->query("name")+"。\n"NOR,me);
                message_vision(CYN"$N趁旁人不注意，干净把"+book->query("name")+CYN+"拣了起来，揣入怀里。\n"NOR,me);  
                CHANNEL_D->do_channel( this_object(), "rumor",sprintf("听说%s在%s找到了一%s%s！", me->name(1),query("cwheredig"),
                book->query("unit"),book->query("name")));            
                destruct(this_object());
                return 1;       
                
        }    
        */                     
       if (random(100) > 20){
          switch(random(2)){
            case 0:
              	money=new("/clone/money/gold");
              	bound=random(50) + 100;
              	break;
            case 1:
          		  money=new("/clone/money/thousand-cash");
                bound=random(10)+20;
                break;
            default:
          }
        
       	 	message_vision("$N突然听到\"当\"的一声，好像敲到了什么东西。\n",me);
       	 	message_vision("地上多了一个古绣斑斑的铁盒子，$N趁人不注意，迅速把铁盒喘入怀中。\n",me);
       	 	message_vision("$N从铁盒子里拿出了一些" + money->name(1) + "。\n",me);
       	 	money->set_amount(bound);
       	 	money->move(me);
      	 	destruct(this_object());
      	 	return 1;       
        }
        else{
        	message_vision("$N蹲在这里好像在挖什么东西。\n",me);
        	return 0;        
        }
}


int check(object ob)
{
        object room;
        string room_name;
        
        if(!objectp(ob)
         || !ob->query("startroom")      // add by Yuj@SJ to void job NPC
         || ob->query("env/invisibility")
         || ob->query("no_quest")
         || member_array(ob->query("id"), no_kill_list) != -1
         || userp(ob)
         || ob->query("attitude")=="aggressive"
         || ob->query("have_master")
         || !objectp(room=environment(ob))
         || room->query("no_fight") 
         || room->query("outdoors") == "昆仑翠谷"
         || strsrch(room_name = file_name(room),"/d/") != 0
         || strsrch(room_name, "/d/wizard/") == 0
         || strsrch(room_name, "/d/wuguan/") == 0
         || strsrch(room_name, "/d/death/") == 0
         || strsrch(room_name, "/d/kunlun/") == 0
         || strsrch(room_name, "/d/mingjiao/lsd/") == 0
         || strsrch(room_name, "/d/mingjiao/bhd/") == 0
         || strsrch(room_name, "/d/wudang/") == 0
         || strsrch(room_name, "/d/shaolin/liwu") == 0
         || strsrch(room_name, "/d/shaolin/houyuan") == 0
         || strsrch(room_name, "/d/huashan/jiabi") == 0
         || strsrch(room_name, "/d/shaolin/fumoquan") == 0
         || strsrch(room_name, "/d/shaolin/xinchan") == 0
         || strsrch(room_name, "/d/mingjiao/yuanzi") == 0
         || strsrch(room_name, "/d/mingjiao/wxiang") == 0
         || strsrch(room_name, "/d/baituo/") == 0
         || strsrch(room_name, "/d/thd/") == 0
         || strsrch(room_name, "/d/sld/") == 0) 
         return 0;
        return 1;
}
