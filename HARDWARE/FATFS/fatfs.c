
/*
************************************************************************
*  函数名称：Set_Bef50Hz(char daq_bef50hz)
*  功能描述：显示文件夹下所有文件名
*  输入参数：none
*  返回参数：none
************************************************************************
*/
int dis_dir_name(char dirname[50],char dir_page)
{
	DIR*dp;
	struct dirent*dirp;
	unsigned int i=0; 
	U16 x=30,y=30;
	if ( (dp = opendir(dirname)) == NULL)
	{
	strcat(dirname,"can not be open");
	dischars(x,y,dirname);	//基本上没用
	return 1;
	}
	while ( (dirp = readdir(dp)) != NULL)
	{
		sprintf(dir_name[i],"%s",dirp->d_name);
		i++;
	}
	dir_total_number=i;//将文件数量传给dir_total_number;
	for (i=0;i<12;i++)
	{
		if(i==0)
		{dischars(x,y,"/mnt/work");}
		else if(i==1)
		{dischars(x,y,dirname);}
		if(i+(dir_page*10)>=dir_total_number) {break;} 
		if(i>=2) {dischars(x,y,dir_name[i+(dir_page*10)]);}
		y+=16;
	}
	close(dp);
	return 1;
}
