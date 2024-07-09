// miehuo2.h Cht@SJ 2000-3-12 修改
#include <ansi.h>
int do_miehuo(string arg)
{
        object me, ob;
        object *inv;
        me = this_object();
        ob = this_player();
        inv = deep_inventory(me);
        if( ob->is_busy())
                return notify_fail("你正忙着呢。\n");
        if( ob->is_fighting())
                return notify_fail("你正在战斗。\n");
        if( !sizeof(inv))
                return notify_fail("炉子里面没有东西。\n");
        if( me->query("marks/open") )
                return notify_fail("炉子正开着呢，瞎灭什么火？\n");
        if( !me->query("marks/doing") && !me->query("marks/fired") )
                return notify_fail("炉子还没点火！\n");
        if( !ob->query_temp("doing", 1) )
                return notify_fail("你没在熬药。\n");
        if( ob->query_temp("doing", 1) && !ob->query_temp("done", 1) )
                return notify_fail("你还没熬完药呢！\n");

        message_vision("\n$N把丹炉内的柴火渐渐地熄灭了。\n", ob);
        remove_call_out("do_drug");
        remove_call_out("do_check");
        remove_call_out("do_ready");
        remove_call_out("do_confirm");
        remove_call_out("do_failed");
        call_out("do_drug", 3 , me);
        me->delete("marks/aoyao");
        me->delete("marks/fired");
        me->delete("marks/burned");
        me->delete("marks/doing");
        ob->delete_temp("doing");
        ob->start_busy(14);
        return 1;

}

void do_drug()
{
        int a, b, i, x, y, z;
        object obj, *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        a = 0;
        b = 0;
        x = 0;
        y = 0;
        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        if( inv[i]->name() == HIC"川贝"NOR)
                        {
                                a = a+1;
                         }
                        if( inv[i]->name() == HIW"生地"NOR)
                        {
                                b = b+1;
                         }
                         if( (int)ob->query_temp("done", 1) == 1)
                         {
                                z = 1;
                         }
                }

// 金创药
                if(  sizeof(inv) != 3
                ||  a != 2
                ||  b != 1
                ||  z != 1)
                {
                 remove_call_out("do_drug1");
                 call_out("do_drug1", 0, me);
                }

                else
                {
                        for(i=0; i<sizeof(inv); i++)
                        {      destruct(inv[i]);   }
                obj=new("/clone/medicine/jinchuang");
                obj->move(me);
                remove_call_out("do_over");
                call_out("do_over", 0, obj);
                }
        }
        return;
}

void do_drug1()
{
        int a, b, c, i, z;
        object obj, *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        a = 0;
        b = 0;
        c = 0;
        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        
                        if( inv[i]->name() == HIR"藏红花"NOR)
                        {
                                a = a+1;
                         }
                        if( inv[i]->name() == HIY"鲨胆"NOR)
                        {
                                b = b+1;
                         }
                        if( inv[i]->name() == HIC"田七"NOR)
                        {
                                c = c+1;
                         }
                         if( (int)ob->query_temp("done", 1) == 1)
                         {
                                z = 1;
                         }
                }

// 田七鲨胆散
                if(  sizeof(inv) != 3
                ||  a != 1
                ||  b != 1
                ||  c != 1
                ||  z != 1)
                {
                 remove_call_out("do_drug2");
                 call_out("do_drug2", 0, me);
                }
                else
                {
                        for(i=0; i<sizeof(inv); i++)
                        {      destruct(inv[i]);   }
                obj=new("/clone/medicine/shadansan");
                obj->move(me);                                                                 
                remove_call_out("do_over");
                call_out("do_over", 0, obj);
                }
        }
        return;
}

void do_drug2()
{
        int a, b, c, d, i, z;
        object obj, *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        a = 0;
        b = 0;
        c = 0;
        d = 0;
        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        
            if( inv[i]->name() == GRN"何首乌"NOR)
                        {
                                a = a+1;
                         }
            if( inv[i]->name() == HIW"菊梗"NOR)
                        {
                                b = b+1;
                         }
                        if( inv[i]->name() == HIC"田七"NOR)
                        {
                                c = c+1;
                         }
            if( inv[i]->name() == YEL"雄黄"NOR)
                        {
                                d = d+1;
                         }
                         if( (int)ob->query_temp("done", 1) == 1)
                         {
                                z = 1;
                         }
                }

// 续命八丹
                if(  sizeof(inv) != 4
                ||  a != 1
                ||  b != 1
                ||  c != 1
                ||  d != 1
                ||  z != 1)
                {
                 remove_call_out("do_drug3");
                 call_out("do_drug3", 0, me);
                }
                else
                {
                        for(i=0; i<sizeof(inv); i++)
                        {      destruct(inv[i]);   }
                obj=new("/clone/medicine/badan");
                obj->move(me);
                remove_call_out("do_over");
                call_out("do_over", 0, obj);
                }
        }
        return;
}

void do_drug3()
{
        int a, b, i, z;
        object obj, *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        a = 0;
        b = 0;

        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        
                        if( inv[i]->name() == HIR"藏红花"NOR)
                        {
                                a = a+1;
                         }
                        if( inv[i]->name() == HIC"防风"NOR)
                        {
                                b = b+1;
                         }
                         if( (int)ob->query_temp("done", 1) == 1)
                         {
                                z = 1;
                         }
                }

// 百草丹
                if(  sizeof(inv) != 3
                ||  a != 2
                ||  b != 1
                ||  z != 1)
                {
                 remove_call_out("do_drug4");
                 call_out("do_drug4", 0, me);
                }

                else
                {
                        for(i=0; i<sizeof(inv); i++)
                        {      destruct(inv[i]);   }
                obj=new("/clone/medicine/baicao");
                obj->move(me);
                remove_call_out("do_over");
                call_out("do_over", 0, obj);
                }
        }
        return;
}

void do_drug4()
{
        int a, b, c, i, z;
        object obj, *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        a = 0;
        b = 0;
        c = 0;
        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        if( inv[i]->name() == HIR"藏红花"NOR)
                        {
                                a = a+1;
                         }
                        if( inv[i]->name() == HIG"荷叶"NOR)
                        {
                                b = b+1;
                         }
                        if( inv[i]->name() == HIW"雪莲子"NOR)
                        {
                                c = c+1;
                         }
                         if( (int)ob->query_temp("done", 1) == 1)
                         {
                                z = 1;
                         }
                }

// 九花玉露丸
                if(  sizeof(inv) != 5
                ||  a != 1
                ||  b != 3
                ||  c != 1
                ||  z != 1)
                {
                 remove_call_out("do_drug5");
                 call_out("do_drug5", 0, me);
                }

                else
                {
                        for(i=0; i<sizeof(inv); i++)
                        {      destruct(inv[i]);   }
                obj=new("/clone/medicine/jiuhuawan");
                obj->move(me);
                remove_call_out("do_over");
                call_out("do_over", 0, obj);
                }
        }
        return;
}

void do_drug5()
{
        int a, b, c, d, e, i, z;
        object obj, *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        a = 0;
        b = 0;
        c = 0;
        d = 0;
        e = 0;
        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        if( inv[i]->name() == HIW"虎骨"NOR)
                        {
                                a = a+1;
                         }
                        if( inv[i]->name() == HIW"雪莲子"NOR)
                        {
                                b = b+1;
                         }
                        if( inv[i]->name() == HIC"茯苓"NOR)
                        {
                                c = c+1;
                         }
                        if( inv[i]->name() == HIW"白芷"NOR)
                        {
                                d = d+1;
                         }
                        if( inv[i]->name() == HIG"人参"NOR)
                        {
                                e = e+1;
                         }
                         if( (int)ob->query_temp("done", 1) == 1)
                         {
                                z = 1;
                         }
                }

// 白虎夺命丹
                if(  sizeof(inv) != 5
                ||  a != 1
                ||  b != 1
                ||  c != 1
                ||  d != 1
                ||  e != 1
                ||  z != 1)
                {
                 remove_call_out("do_drug6");
                 call_out("do_drug6", 0, me);
                }

                else
                {
                        for(i=0; i<sizeof(inv); i++)
                        {      destruct(inv[i]);   }
                obj=new("/clone/medicine/duoming-dan");
                obj->move(me);
                remove_call_out("do_over");
                call_out("do_over", 0, obj);
                }
        }
        return;
}

void do_drug6()
{
        int a, b, c, d, i, z;
        object obj, *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        a = 0;
        b = 0;
        c = 0;
        d = 0;
        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        if( inv[i]->name() == HIG"荷叶"NOR)
                        {
                                a = a+1;
                         }
                        if( inv[i]->name() == HIB"蝉蜕"NOR)
                        {
                                b = b+1;
                         }
                        if( inv[i]->name() == HIG"人参"NOR)
                        {
                                c = c+1;
                         }
                        if( inv[i]->name() == GRN"何首乌"NOR)
                        {
                                d = d+1;
                         }
                         if( (int)ob->query_temp("done", 1) == 1)
                         {
                                z = 1;
                         }
                }

// 九转结气丸
                if(  sizeof(inv) != 4
                ||  a != 1
                ||  b != 1
                ||  c != 1
                ||  d != 1
                ||  z != 1)
                {
                 remove_call_out("do_drug7");
                 call_out("do_drug7", 0, me);
                }

                else
                {
                        for(i=0; i<sizeof(inv); i++)
                        {      destruct(inv[i]);   }
                obj=new("/clone/medicine/jieqi-wan");
                obj->move(me);
                remove_call_out("do_over");
                call_out("do_over", 0, obj);
                }
        }
        return;
}

void do_drug7()
{
        int a, b, c, d, i, z;
        object obj, *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        a = 0;
        b = 0;
        c = 0;
        d = 0;
        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        if( inv[i]->name() == HIG"嫩竹叶"NOR)
                        {
                                a = a+1;
                         }
                        if( inv[i]->name() == HIW"柴胡"NOR)
                        {
                                b = b+1;
                         }
                        if( inv[i]->name() == YEL"木瓜"NOR)
                        {
                                c = c+1;
                         }
                        if( inv[i]->name() == HIY"黄芪"NOR)
                        {
                                d = d+1;
                         }

                         if( (int)ob->query_temp("done", 1) == 1)
                         {
                                z = 1;
                         }
                }

// 延年聚精丹
                if(  sizeof(inv) != 4
                ||  a != 1
                ||  b != 1
                ||  c != 1
                ||  d != 1
                ||  z != 1)
                {
                 remove_call_out("do_drug8");
                 call_out("do_drug8", 0, me);
                }

                else
                {
                        for(i=0; i<sizeof(inv); i++)
                        {      destruct(inv[i]);   }
                obj=new("/clone/medicine/jujing-dan.c");
                obj->move(me);
                remove_call_out("do_over");
                call_out("do_over", 0, obj);
                }
        }
        return;
}

void do_drug8()
{
        int a, b, c, d, i, z;
        object obj, *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        a = 0;
        b = 0;
        c = 0;
        d = 0;
        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        if( inv[i]->name() == HIG"荷叶"NOR)
                        {
                                a = a+1;
                         }
                        if( inv[i]->name() == HIW"雪莲子"NOR)
                        {
                                b = b+1;
                         }
                        if( inv[i]->name() == HIC"茯苓"NOR)
                        {
                                c = c+1;
                         }
                        if( inv[i]->name() == HIC"川贝"NOR)
                        {
                                d = d+1;
                         }
                         if( (int)ob->query_temp("done", 1) == 1)
                         {
                                z = 1;
                         }
                }

// 玉露清新散
                if(  sizeof(inv) != 4
                ||  a != 1
                ||  b != 1
                ||  c != 1
                ||  d != 1
                ||  z != 1)
                {
                 remove_call_out("do_drug9");
                 call_out("do_drug9", 0, me);
                }
                else
                {
                        for(i=0; i<sizeof(inv); i++)
                        {      destruct(inv[i]);   }
                obj=new("/clone/medicine/qingxin-san");
                obj->move(me);
                remove_call_out("do_over");
                call_out("do_over", 0, obj);
                }
        }
        return;
}

void do_drug9()
{
        int a, b, c, d, i, z;
        object obj, *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        a = 0;
        b = 0;
        c = 0;
        d = 0;
        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        if( inv[i]->name() == CYN"当归"NOR)
                        {
                                a = a+1;
                         }
                        if( inv[i]->name() == HIW"金银花"NOR)
                        {
                                b = b+1;
                         }
                        if( inv[i]->name() == HIW"白薇"NOR)
                        {
                                c = c+1;
                         }
                        if( inv[i]->name() == HIC"防风"NOR)
                        {
                                d = d+1;
                         }
                         if( (int)ob->query_temp("done", 1) == 1)
                         {
                                z = 1;
                         }
                }

// 归元丹
                if(  sizeof(inv) != 4
                ||  a != 1
                ||  b != 1
                ||  c != 1
                ||  d != 1
                ||  z != 1)
                {
                 remove_call_out("do_drug10");
                 call_out("do_drug10", 0, me);
                }
                else
                {
                        for(i=0; i<sizeof(inv); i++)
                        {      destruct(inv[i]);   }
                obj=new("/clone/medicine/guiyuan-dan");
                obj->move(me);
                remove_call_out("do_over");
                call_out("do_over", 0, obj);
                }
        }
        return;
}

void do_drug10()
{
        int a, b, c, d, i, z;
        object obj, *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        a = 0;
        b = 0;
        c = 0;
        d = 0;
        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        if( inv[i]->name() == HIG"荷叶"NOR)
                        {
                                a = a+1;
                         }
                        if( inv[i]->name() == CYN"当归"NOR)
                        {
                                b = b+1;
                         }
                        if( inv[i]->name() == HIW"天花粉"NOR)
                        {
                                c = c+1;
                         }
                        if( inv[i]->name() == HIC"藿香"NOR)
                        {
                                d = d+1;
                         }
                         if( (int)ob->query_temp("done", 1) == 1)
                         {
                                z = 1;
                         }
                }

// 大还丹
                if(  sizeof(inv) != 4
                ||  a != 1
                ||  b != 1
                ||  c != 1
                ||  d != 1
                ||  z != 1)
                {
                 remove_call_out("do_drug11");
                 call_out("do_drug11", 0, me);
                }
                else
                {
                        for(i=0; i<sizeof(inv); i++)
                        {      destruct(inv[i]);   }
          obj=new("/clone/medicine/dahuan-dan");
                obj->move(me);
                remove_call_out("do_over");
                call_out("do_over", 0, obj);
                }
        }
        return;
}


void do_drug11()
{
        int a, b, c, d, i, z;
        object obj, *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        a = 0;
        b = 0;
        c = 0;
        d = 0;
        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        if( inv[i]->name() == HIW"天花粉"NOR)
                        {
                                a = a+1;
                         }
                        if( inv[i]->name() == CYN"当归"NOR)
                        {
                                b = b+1;
                         }
                        if( inv[i]->name() == HIC"藿香"NOR)
                        {
                                c = c+1;
                         }
                        if( inv[i]->name() == HIG"桑叶"NOR)
                        {
                                d = d+1;
                         }
                         if( (int)ob->query_temp("done", 1) == 1)
                         {
                                z = 1;
                         }
                }

// 小还丹
                if(  sizeof(inv) != 4
                ||  a != 1
                ||  b != 1
                ||  c != 1
                ||  d != 1
                ||  z != 1)
                {
                 remove_call_out("do_drug12");
                 call_out("do_drug12", 0, me);
                }
                else
                {
                        for(i=0; i<sizeof(inv); i++)
                        {      destruct(inv[i]);   }
                obj=new("/clone/medicine/xiaohuan-dan");
                obj->move(me);
                remove_call_out("do_over");
                call_out("do_over", 0, obj);
                }
        }
        return;
}


void do_drug12()
{
        int i;
        object *inv;
        object me = this_object();
        object ob = this_player();
        inv = deep_inventory(me);

        if( sizeof(inv))
        {
                for(i=0; i<sizeof(inv); i++)
                {
                        destruct(inv[i]);
                }
                message_vision(HIR"\n由于药方错误，$N炼丹失败了。\n"NOR,ob);
                ob->delete_temp("doing");
                if( userp(ob) ) log_file("LIAN_DAN",
                sprintf("%s %s(%s) 因为药方错误炼丹失败 于 %s\n", ob->query("title"), ob->name(1), geteuid(ob), ctime(time())[4..19] ) );
                me->delete("liquid/remaining");
                me->delete("liquid/name");
        }
        return;
}

void do_over(object obj)
{       
        object me = this_object();
        object ob = this_player();

        ob->delete_temp("done");
        me->delete("liquid/remaining");
        me->delete("liquid/name");   

        if( userp(ob) ) log_file("LIAN_DAN",
        sprintf("%s %s(%s)炼制%s(%s) 于 %s\n", ob->query("title"), ob->name(1), geteuid(ob), obj->name(1), obj->query("id"), ctime(time())[4..19] ) );

        if( ((int)ob->query("potential", 1)+30) < (int)ob->query("max_pot", 1))
        {
                ob->add("potential", 10+random(10));
        }
        if( ((int)ob->query("potential", 1)+30) >= (int)ob->query("max_pot", 1))
        {
                ob->set("potential", (int)ob->query("max_pot", 1));
        }
        return;
}
