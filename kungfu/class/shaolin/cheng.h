// cheng.h for fight and get letter from Cheng

void init()
{
	::init();

	if (interactive(this_player()) && this_player()->query_temp("fighting") && living(this_player()))
	{
		COMBAT_D->do_attack(this_object(), this_player(), query_temp("weapon"), 1);
		this_player()->add_temp("beat_count", 1);
	}
}

int accept_object(object ob, object obj)
{
	if (obj->id("tuijian xin3")
	&& ob->query_temp("have_letter"))
	{
		ob->set_temp("apprentice_ok",1);
	        command("say 好，" + ob->query("name") + "，你愿意拜我为师吗？");
		remove_call_out("destroying");
		call_out("destroying", 1, obj);
		return 1;
	}

        command("smile");
        command("say 这东西给我可没有什么用。");
	return 0;
}

void destroying(object obj)
{
	if (obj)
		destruct(obj);
}

void attempt_apprentice(object ob)
{
	object me  = this_object();
	mapping ob_fam;
	mapping my_fam  = me->query("family");

	string name, new_name;

	name = ob->query("name");

	if (!(ob_fam = ob->query("family")) || ob_fam["family_name"] != "少林派")
	{
		command("say " + RANK_D->query_respect(ob) + "与本派素无来往，不知此话从何谈起？");
		return;
	}

	if ( (string)ob->query("class")!="bonze" && ob_fam["family_name"] == "少林派")
	{
		command("say " + RANK_D->query_respect(ob) + "是俗家弟子，不能在寺内学艺。");
		return;
	}

	if ( ob_fam["generation"] <= my_fam["generation"] )
	{
		command("say " + RANK_D->query_respect(ob) + "，贫僧哪里敢当 !");
		return;
	}

	if ( ob_fam["generation"] == (my_fam["generation"] + 1) && name[0..0] == "慧")
	{
		command("say " + ob_fam["master_name"] + "的徒弟怎么跑到我这儿来了，哈哈哈！");
		command("recruit " + ob->query("id"));
	}

	if ( ob_fam["generation"] == (my_fam["generation"] + 2) )
	{
		if ( ob->query_temp("apprentice_ok") )
		{
			ob->delete_temp("have_letter");
			ob->delete_temp("apprentice_ok");

			command("say 是" + ob_fam["master_name"] + "叫你来找我的吧，哈哈哈！");
			command("say 贫僧又得一可塑之才，真是可喜可贺！");

			new_name = "慧" + name[1..1];
			ob->set("name", new_name);

			command("say 从今以后你的法名叫做" + new_name + "，恭喜你荣升为少林派慧字辈弟子！");
			command("recruit " + ob->query("id"));
		}
		else
		{
			command("say " + RANK_D->query_respect(ob) + "，你没有推荐信，不能越级拜师。");
		}
	}
}
