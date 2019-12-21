#incldue "daq_para.h"

struct DAQ_PARA
{
	//������Ŀ���������²���
	long DAQ_LineNum;			//�ߺ�
	long DAQ_DotNum;			//���
	long DAQ_DotIncrease;			//�������
	char DAQ_DotDirect;			//��ŷ�:0Ϊ����,��0Ϊ����
	long DAQ_ABDiv2;			//AB/2�ľ���
	long DAQ_MNDiv2;			//MN/2�ľ���
	long DAQ_Side;				//�Բ����
	long DAQ_Org_A;				//ԭ��-A��
	char DAQ_BEF50Hz;			//50Hz�ݲ�����:0Ϊ���ݲ�,��0Ϊ�ݲ�
	long DAQ_Second_delay;			//���γ���ʱ

	//������ʽ���������²���
	long DAQ_Surveytype;		//�������ͣ�1Ϊ���2Ϊ���ݣ�3Ϊ�Գ��ļ���4Ϊ����
	char DAQ_SynMode;			//ͬ����ʽ��0Ϊ��ͬ��������Ϊ��ͬ��*******
	char DAQ_PolMode;			//�����ʼ��㷽ʽ��0Ϊ��ͬ��������Ϊ��ͬ��******
	
	//����װ�ý������������²���
	long DAQ_PowerTime;			//����ʱ��
	long DAQ_PowerTms;			//�������
	long DAQ_Positive_Cur;			//�������(������ʾ)
	long DAQ_Reverse_Cur;			//�������(������ʾ)

	//ϵͳ���ý������������²���
	long DAQ_MaxSlfEle;			//����Ե�
	long DAQ_MinCurrent;			//��С����	
	long DAQ_MinOneEle;			//��Сһ�γ�
	
	//���ݲɼ���������ʾ���²���
	long DAQ_SP;				//SP����������
	long DAQ_PowerVolt;			//�����ѹ����������
	U32 DAQ_100K;				//100����Kֵ����ʾ **
	long DAQ_NatureEle;			//��Ȼ��λ����ʾ **
	long DAQ_OneEle;				//һ�ε�λ����ʾ **
	long DAQ_Polarizey_1000;			//�����ʣ���ʾ **
	long DAQ_PowerCurrent;			//�����������ʾ **
	long DAQ_Resistance;			//�����ʣ���ʾ **
	char DAQ_ADplus;				//AD����, ����
	long DAQ_HlfDecay;			//��˥ʱ����ʾ **
	long DAQ_SynPara;			//�ۺϲ�������ʾ **
	long DAQ_AddSum;			//�ۼӺͣ���ʾ **
	long DAQ_DepartDrg;			//ƫ��ȣ���ʾ **
	long DAQ_GNDRes;			//�ӵص��裬��ʾ **
	
	//GPS�������
	char 	DAQ_gps_year;			//GPS �� 
        char 	DAQ_gps_month;			//GPS ��
	char	DAQ_gps_day;			//GPS ��
	char	DAQ_gps_date;			//GPS ����
	char	DAQ_gps_hour;			//GPS Сʱ
	char	DAQ_gps_minus;			//GPS ����
	char	DAQ_gps_second;			//GPS ��
	char 	DAQ_gps_cav;			//GPS 
	float 	DAQ_gps_ns;			//GPS ��γ
	char 	DAQ_gps_cns;
	float 	DAQ_gps_ew;			//GPS ����
	char 	DAQ_gps_cew;
        char 	DAQ_gps_year_buf;			//GPS �� �ݴ�
        char 	DAQ_gps_month_buf;			//GPS ���ݴ�
	char	DAQ_gps_day_buf;			//GPS ���ݴ�
	char	DAQ_gps_date_buf;			//GPS �����ݴ�
	char	DAQ_gps_hour_buf;			//GPS Сʱ�ݴ�
	char	DAQ_gps_minus_buf;			//GPS �����ݴ�
	char	DAQ_gps_second_buf;			//GPS ���ݴ�
	char 	DAQ_gps_cav_buf;			//GPS �ݴ�
	float 	DAQ_gps_ns_buf;				//GPS ��γ�ݴ�
	char 	DAQ_gps_cns_buf;
	float 	DAQ_gps_ew_buf;				//GPS �����ݴ�
	char 	DAQ_gps_cew_buf;
} DateACQPara;



/****************��ʼ������************/
void DateACQParaInitialize(void)
{
	DateACQPara.DAQ_LineNum = 1;
	DateACQPara.DAQ_DotNum = 1;
	DateACQPara.DAQ_DotIncrease = 10;	
	DateACQPara.DAQ_DotDirect = 1;
	DateACQPara.DAQ_ABDiv2 = 100;
	DateACQPara.DAQ_MNDiv2 = 100;
	DateACQPara.DAQ_Side = 100;
	DateACQPara.DAQ_Org_A = 100;
	DateACQPara.DAQ_BEF50Hz = 1;
	DateACQPara.DAQ_Second_delay = 11;
	DateACQPara.DAQ_Surveytype = 2;
	DateACQPara.DAQ_SynMode = 1;
	DateACQPara.DAQ_PolMode = 1;
	DateACQPara.DAQ_PowerTime = 1;
	DateACQPara.DAQ_PowerTms = 1;
	DateACQPara.DAQ_MaxSlfEle = 100;
	DateACQPara.DAQ_MinCurrent = 100;
	DateACQPara.DAQ_MinOneEle = 100;
	DateACQPara.DAQ_SP = 100;
	DateACQPara.DAQ_PowerVolt = 500;
	DateACQPara.DAQ_100K = 100;
	DateACQPara.DAQ_NatureEle = 0;
	DateACQPara.DAQ_OneEle = 0;
	DateACQPara.DAQ_Polarizey_1000 = 0;
	DateACQPara.DAQ_PowerCurrent = 1;
	DateACQPara.DAQ_Resistance = 0;
	DateACQPara.DAQ_ADplus=1;
	DateACQPara.DAQ_HlfDecay = 0;
	DateACQPara.DAQ_SynPara = 0;
	DateACQPara.DAQ_AddSum = 0;
	DateACQPara.DAQ_DepartDrg = 0;
	DateACQPara.DAQ_GNDRes = 0;
	DateACQPara.DAQ_gps_cav = 65;
	DateACQPara.DAQ_gps_ns = 3959.3159;
	DateACQPara.DAQ_gps_cns = 78;
	DateACQPara.DAQ_gps_ew = 11620.7382;
	DateACQPara.DAQ_gps_cew = 69;
}
/****************************************/