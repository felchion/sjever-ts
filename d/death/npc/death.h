void init()
{
  object me = this_player();

  ::init();
  if (!userp(me)) return;
  call_out( "death_stage", 90, me, 0);
}

void death_stage(object ob, int stage)
{
  int i;
  object *inv;
  if( !ob || !present(ob) ) return;

  if( !ob->is_ghost() ) {
    command("say 轮回无常，阳间之人，应当回到阳间去才是。");
    message_vision("两名力士闪了出来，架起$N，一晃就不见了！\n", ob);
    ob->reincarnate();
    if (ob->query("enter_wuguan")) ob->move(START_ROOM);
    else ob->move(REVIVE_ROOM);
    tell_object(ob, HIW"你一阵晕旋，醒来后却发现自己已经回到了阳间！\n"NOR);
    message("vision","你忽然发现人影不知从什么地方冒了出来。\n", environment(ob), ob);
    return;
  }

  message_vision(death_msg[stage],this_object(),ob);
  if( ++stage < sizeof(death_msg) ) {
    call_out( "death_stage", 5, ob, stage );
    return;
  } else
  ob->reincarnate();

  inv = all_inventory(ob);
  for (i = 0; i < sizeof(inv); i++) {
    if( base_name(inv[i])[0..12]=="/clone/money/"
    || (inv[i]->query("imbued") && inv[i]->query("owner")==ob->query("id")) )
    continue;
    DROP_CMD->do_drop(ob, inv[i]);
  }
  UPDATE_D->get_cloth(ob);

  if (ob->query("enter_wuguan")) ob->move(START_ROOM);
  else ob->move(REVIVE_ROOM);
  message("vision","你忽然发现身旁多了一个人影，不过那人影又好像已经在那里很久了，只是你一直没发觉。\n", environment(ob), ob);
}
