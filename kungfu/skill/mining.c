// mining.c

inherit SKILL;

void create() 
{ 
	seteuid(getuid()); 
}

string type() 
{ 
	return "skill"; 
}

int valid_learn(object me) 
{
	return notify_fail("采矿技能只能靠采矿来提高。\n");
}

int practice_skill(object me)
{	
	return notify_fail("采矿技能只能靠采矿来提高。\n");
}
