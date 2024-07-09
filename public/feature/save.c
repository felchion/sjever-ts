// save.c
// Modified by mxzhao 2004/02/18
// Modified by mxzhao 2004/05/07 for add backup function

#include <mudlib.h>
#include <ansi.h>

int backup();

int update_viptime(object me)
{
	return 1;
}

int save()
{
	string file,data;
	object me = this_object();


	if (!stringp(file = me->query_save_file()))
	{
		return 0;
	}
	
	if(!IN_MAIN && file!="/data/cpd") return 0;

	assure_file(file += __SAVE_EXTENSION__);



	if (!save_object(file))
	{
		return 0;
	}

	return 1;
}

int restore()
{
	string file;
	mixed ret;
	object me = this_object();

	if (!stringp(file = me->query_save_file()))
	{
		return 0;
	}

	file += __SAVE_EXTENSION__;
	if (!geteuid()) seteuid(getuid());

	if (file_size(file) == -1)	return 0;
	return restore_object(file);
}