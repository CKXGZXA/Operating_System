void RoundRun()     /*时间片轮转调度算法*/
{
	int flag = 1;
	GetFirst();
	while ( run != NULL )
	{
		Output();          
        while ( flag )
		{
			run->count++;
            run->cputime++;            
            run->needtime--;
			if ( run->needtime == 0 ) /*进程执行完毕*/
			{
				run->state = 'F';             
                InsertFinish( run );              
                flag = 0;
			}else if ( run->count == run->round )                   /*时间片用完*/
			{
				run->state = 'W';

				run->count				= 0;    /*计数器清零，为下次做准备*/
				InsertTime( run ); 
                flag= 0;
			}
		}

		flag = 1;
		GetFirst();
	}
}