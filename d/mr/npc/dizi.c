// dizi.h

mapping my_skills;
mapping my_learned;
mapping valid_types = 
([
       "finger":       "指法",
       "hand":         "手法",
       "cuff":         "拳法",
       "claw":         "爪法",
       "strike":       "掌法",
       "leg":          "腿法"
]);

mapping *command=({
([
       "id"   : "1",
       "name" : "苏州南门",
        "file" : "/d/suzhou/southgate",
]),
([                   
       "id"   : "2",
       "name" : "燕子坞",
        "file" : "/d/mr/yanziwu",
]),               
([                   
       "id"   : "3",
        "name" : "嘉峪关西城门",
       "file" : "/d/xingxiu/jygw",
]),
([
       "id"   : "4",
       "name" : "大渡口",
       "file" : "/d/xingxiu/road2",
]),
([     
       "id"   : "5",
       "name" : "归云庄",
       "file" : "/d/thd/guiyun/houting",
]),
([
       "id"   : "6",
       "name" : "桃花岛",
       "file" : "/d/thd/jicui",
]),
([
       "id"  : "7",
       "name" : "少林寺大雄宝殿",
       "file" : "/d/shaolin/daxiong",
]),
([
       "id"  : "8",
       "name" : "星宿海",
       "file" : "/d/xingxiu/ryd",
]),
});

void init()
{
       object me = this_player();
       object ob = this_object();
       ::init();
       if (//interactive(me)  && 
		   me->query_temp("dizi")==ob &&
		   me->query("family/family_name") == "姑苏慕容")
       {
               if( !ob->query("cloned") )
               {
                       ob->add_temp("apply/attack",  random(30));
                       ob->add_temp("apply/dodge",  100+random(50));
                       ob->set_temp("skill_target", me->query_temp("skill_target"));
                       ob->set_temp("skill_id", me->query_temp("skill_id"));
                       ob->set_temp("skill_lvl", me->query_temp("skill_lvl"));
                       ob->set("family/master_id", me->query("id"));
                       ob->set_temp("skill_type", me->query_temp("skill_type"));
						ob->set_temp("lead",me);
                       ob->set_leader(me);
                       if( !ob->query_temp("leader"))
                       tell_object(me, ob->name()+"决定跟你一起行动。\n");
//                       remove_call_out("do_clone");
                       remove_call_out("check_fight");
                       remove_call_out("dest");
                       do_clone(me,ob);
                       call_out("check_fight", 1, ob);
                       call_out("dest", 720, ob);
               }
               else 
               {
                       remove_call_out("dest");
                       remove_call_out("check_fight");
                       call_out("check_fight", 1, ob);
                       call_out("dest", 720, ob);
               }
       }
       add_action("do_touxue", "touxue");
       add_action("push", "push");
}

int do_clone(object me,object ob)
{
//       object ob;
       mapping hp_status, skill_status, map_status, prepare_status;
       string *sname, *mname, *pname;
       int i, temp;

//       ob = this_object();

/* delete and copy skills */
       ob->set("cloned", 1);
       if ( mapp(skill_status = ob->query_skills()) ) {
               skill_status = ob->query_skills();
               sname  = keys(skill_status);

               temp = sizeof(skill_status);
               for(i=0; i<temp; i++) {
                       ob->delete_skill(sname[i]);
               }
       }

       if ( mapp(skill_status = me->query_skills()) ) {
               skill_status = me->query_skills();
               sname  = keys(skill_status);

               for(i=0; i<sizeof(skill_status); i++) {
                       ob->set_skill(sname[i], skill_status[sname[i]]);
               }
       }

/* delete and copy skill maps */

       if ( mapp(map_status = ob->query_skill_map()) ) {
               mname  = keys(map_status);

               temp = sizeof(map_status);
               for(i=0; i<temp; i++) {
                       ob->map_skill(mname[i]);
               }
       }

       if ( mapp(map_status = me->query_skill_map()) ) {
               mname  = keys(map_status);

               for(i=0; i<sizeof(map_status); i++) {
                       ob->map_skill(mname[i], map_status[mname[i]]);
               }
       }

/* delete and copy skill prepares */

       if ( mapp(prepare_status = ob->query_skill_prepare()) ) {
               pname  = keys(prepare_status);

               temp = sizeof(prepare_status);
               for(i=0; i<temp; i++) {
                       ob->prepare_skill(pname[i]);
               }
       }

       if ( mapp(prepare_status = me->query_skill_prepare()) ) {
               pname  = keys(prepare_status);

               for(i=0; i<sizeof(prepare_status); i++) {
                       ob->prepare_skill(pname[i], prepare_status[pname[i]]);
               }
       }

       hp_status = me->query_entire_dbase();

               ob->set("str", hp_status["str"]);
               ob->set("int", hp_status["int"]);
               ob->set("con", hp_status["con"]);
               ob->set("dex", hp_status["dex"]);

               ob->set("eff_jingli",hp_status["eff_jingli"]);
               ob->set("max_qi",    hp_status["max_qi"]);
               ob->set("eff_qi",    hp_status["max_qi"]);
               ob->set("qi",        hp_status["max_qi"]);
               ob->set("jingli",    hp_status["eff_jingli"]);
               ob->set("max_jingli",hp_status["max_jingli"]);
               ob->set("max_jing",  hp_status["max_jing"]);
               ob->set("eff_jing",  hp_status["eff_jing"]);
               ob->set("jing",      hp_status["eff_jing"]);
               ob->set("max_neili", hp_status["max_neili"]);
               ob->set("neili",     hp_status["max_neili"]);
               ob->set("jiali",     hp_status["jiali"]);
               ob->set("combat_exp",hp_status["combat_exp"]*2/3);
               return 1;
}

string ask_job()
{
       object me = this_player();
       object ob = this_object();
if( ob->query("family/master_id") != me->query("id"))
               return "您又不是我师傅，我没兴趣回答你的问题。";

       return "庄主叫师傅您和我一同出庄去偷学"+ ob->query_temp("skill_lvl")+"级"+ob->query_temp("skill_target")+"["+ob->query_temp("skill_id")+"]。";
}

string ask_command()
{
       object ob = this_player();
       string str="";
        int i=sizeof(command);
if( this_object()->query("family/master_id") != ob->query("id"))
               return "您又不是我师傅，我没兴趣回答你的问题。";
        while (i--) 
        {
               str += command[i]["name"];
               str += "(" + command[i]["id"] + ") \n";
        }

    return "您可以用push <id>的命令让我去您想让我去的地方。\n"         + str;
}

int do_touxue(string arg)
{
        object ob, me, obj;
        string skill, skill_name, teacher;
        int master_skill, gin_cost, lvl, iii, flag=0;
        me = this_player();
        obj = this_object();
        if (me->is_busy())
                return notify_fail("你现在正忙着呢。\n");

        if( !me->is_fighting() )
                return notify_fail("不在战斗中怎么能偷学呢？\n");

        if( me->query("family/family_name") != "姑苏慕容")
               return notify_fail("你不是姑苏慕容弟子，根本就不会偷学。\n");

        if( me->query_skill("douzhuan-xingyi", 1) < 100 )
               return notify_fail("你的斗转星移级别不够，无法运用偷学这个高级技能。\n");

        if(me->query_skill_mapped("parry") != "douzhuan-xingyi")
                return notify_fail("你现在没有用斗转星移，怎么能偷学到别人的武功？\n");

        if(!arg || (sscanf(arg, "%s %s", teacher, skill)!=2 ))
                return notify_fail("指令格式：touxue <某人> <技能>\n");

        if( !(ob = present(teacher, environment(me))) || !ob->is_character())
                return notify_fail("你要向谁偷学？\n");

        if( !present(this_object(), environment(me)))
               return notify_fail("你的弟子没有在这里，你偷学了给谁看？\n");

        if( this_object()->query("family/master_id") != me->query("id"))
               return notify_fail("旁边的这个人好象不是你的弟子吧？\n");

        if( !living(ob) )
                return notify_fail("嗯....你得先把" + ob->name() + "弄醒再说。\n");

        if( (int)ob->query("combat_exp", 1) < (int)me->query("combat_exp", 1)*2/5 )
               return notify_fail("这种人的武功你也偷学,不闲丢人？\n");

    if( ob->query_temp("combat_yield") )
               return notify_fail("他现在是打不还手，你怎么偷学他的武功？\n");

        for(iii=1;iii<sizeof(valid_types);iii++)
        {
                if (me->query_temp("skill_type") == valid_types[iii]) flag=1;
        }
/*        if (flag)
        {
        if( ob->query_skill_prepared(me->query_temp("skill_type")) != me->query_temp("skill_id") )
                        return notify_fail("他现在不能使用"+me->query_temp("skill_target")+"，你怎么偷学他的武功？\n");
        }
        else
        {
        if( ob->query_skill_mapped(me->query_temp("skill_type")) != me->query_temp("skill_id") )
               return notify_fail("他现在不能使用"+me->query_temp("skill_target")+"，你怎么偷学他的武功？\n");
        }
*/
        if( !master_skill = ob->query_skill(skill, 1) )
                return notify_fail("这项技能你恐怕必须找别人偷学了。\n");

       if( !me->query_condition("mr_job"))
               return notify_fail("你还没有领任务呢。\n");

       if( skill != me->query_temp("skill_id") )
               return notify_fail("这一次你是要偷学这种武功么？\n");


       lvl = obj->query_skill(skill, 1);
       if( lvl >= obj->query_temp("skill_lvl") )
       {
               write("弟子(Dizi)在你耳边悄悄告诉你：庄主交给咱们的任务已经完成了，咱们还是回燕子坞吧。\n");
               obj->set_temp("job_done", 1);
               return 1;
       }

        gin_cost = 800 / (int)me->query("int");


        if( ((int)ob->query("jing") > gin_cost/5 + 1 )
        &&  ((int)ob->query("jingli") > gin_cost/5 + 1 ))
        {
                if( userp(ob) )
                 ob->receive_damage("jing", gin_cost/5 + 1);
                 ob->receive_damage("jingli", gin_cost/5 + 1);
        }
        else
        {
                write("但是" + ob->name() + "显然太累了，你已经没有办法偷学到什么了。\n");
                return 1;
        }


        if( (int)me->query("jing") > gin_cost && (int)me->query("jingli") > gin_cost )
        {
               if(skill_name = SKILL_D(skill)->query_skill_name(master_skill))
                        printf("你根据斗转星移的精髓，仔细琢磨着%s的每个招数，尤其对「%s」这一招似乎有些心得。\n", ob->name(), skill_name);
                else
                        printf("你把所偷学的"+to_chinese(skill)+"的精华尽数讲解给你的弟子，你的弟子也偷偷琢磨着%s的招数，似乎有些心得。\n", ob->name());
                        obj->add_skill(skill, obj->query("int"));
        }
       else
       {
               gin_cost = me->query("jing");
                write("你太累了，还是先休息一下吧。\n");
        }
               if( random(100) >= 90)
               {
               message_vision("$N突然觉得有什么事情不大对了，双眼顿起杀意。\n", ob);
                       ob->kill_ob(me);
               }


        me->receive_damage("jing", gin_cost );
        me->receive_damage("jingli", gin_cost );
       me->start_busy(2);
        return 1;
}

varargs void add_skill(string skill, int amount, int weak_mode)
{
    int spi;
        if( !find_object(SKILL_D(skill))
        &&  file_size(SKILL_D(skill)+".c") < 0 )
            error("F_SKILL: No such skill (" + skill + ")\n");

        if( !weak_mode||userp(this_object()) )
       {
                if( !mapp(my_skills) ) my_skills = ([]);
                if( undefinedp(my_skills[skill]) ) my_skills[skill] = 0;
        }

        spi = query("spi");
        if( sizeof(my_learned) > spi )
                amount /= sizeof(my_learned) - spi;

        if( !amount ) amount = 1;

        if( !mapp(my_learned) ) my_learned = ([ skill : amount ]);
        else (int)my_learned[skill] += amount;

        if( (!weak_mode||userp(this_object()))
        &&   my_learned[skill] > (my_skills[skill] + 1) * (my_skills[skill] + 1) ) {
             my_skills[skill]++;
             my_learned[skill] = 0;
             this_object()->set_skill(skill,my_skills[skill]+1);
       }
}

void accept_fight(object ob)
{
       object me = this_player();
       if( ob->query("family/master_id") == me->query("id") )
       {
               message_vision("\n$N连忙跪倒在地，说道：弟子不敢。\n", ob);
       }
       else if( ob->query("family/family_name") == "姑苏慕容")
       {
               message_vision("咱们同门之间不宜相斗吧？\n", ob);
       }
       else
       {
               message_vision("\n$N狞笑道：就凭你也跟我斗？\n", ob);
               ob->kill_ob(me);
       }
       return;
}

void check_fight(object ob)
{
       mapping smap, pmap;
       string type, id, *skill;
        int i;
       type = ob->query_temp("skill_type");
       id = ob->query_temp("skill_id");

       smap = ob->query_skill_map();
       pmap = ob->query_skill_prepare();
       if( ob->is_fighting())
       {
               if( ob->query_temp("skill_type") == "blade"
               ||  ob->query_temp("skill_type") == "sword"
               ||  ob->query_temp("skill_type") == "stick"
               ||  ob->query_temp("skill_type") == "staff"
               ||  ob->query_temp("skill_type") == "club"
               ||  ob->query_temp("skill_type") == "whip" )
               {
                       carry_object("/clone/weapon/"+ob->query_temp("skill_type"))->wield();
                       ob->map_skill(type, id);
//                         ob->set_skill(id, 150);
               }
                    if( ob->query_temp("skill_type") == "strike"
                    ||  ob->query_temp("skill_type") == "cuff"
                    ||  ob->query_temp("skill_type") == "leg"
                    ||  ob->query_temp("skill_type") == "claw"
                    ||  ob->query_temp("skill_type") == "hand"
                    ||  ob->query_temp("skill_type") == "finger" )
                       {
                       skill = keys(pmap);
                       for (i=0; i<sizeof(skill); i++) {
                       ob->prepare_skill(skill[i]);
                       }

                       ob->reset_action();
                       ob->map_skill(type, id);
                       ob->map_skill("parry", "canhe-zhi");
//                         ob->set_skill(id, 150);
                       ob->prepare_skill(type, id);
               }
         }
          else call_out("check_fight", 1, ob);
          return;
}                       

int push(string arg)
{
       object me = this_player();
       object ob =this_object();
       int i=sizeof(command);

       if (!arg) return notify_fail("你要去哪里？\n");
        if (me->is_busy() || me->is_fighting())
               return notify_fail("你正忙着呢！\n");
        if (ob->is_busy() || ob->is_fighting())
               return notify_fail("他正忙着呢！\n");

       if( me->query("family/family_name") != "姑苏慕容")
               return notify_fail("你想让他走？门儿也没有！\n");
               
       if( ob->query("family/master_id") != me->query("id"))
               return notify_fail("你又不是他师傅，凭什么命令人家？\n");
       while (i--) 
       {
               if(arg == command[i]["id"])                  
               {
               message_vision("$N对着$n说道：你先去"+command[i]["name"]+"等着我，我随后就到。\n", me,ob);
               me->start_busy(3);
               call_out("do_move", 3, ob, i);
               return 1;
               }
        }
}

void do_move(object ob, int i)
{
       message_vision("\n$N匆匆忙忙地离开。\n\n", ob);
       ob->move(command[i]["file"]);                      
       message_vision("\n$N匆匆忙忙地走了过来。\n\n", ob);
}    

void dest(object ob)
{
	if(!ob) return;
       write(ob->name()+"匆匆离开。\n");  
       destruct(ob);
	   return ;
}
