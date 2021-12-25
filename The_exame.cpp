#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<string.h>
#define LEN sizeof(card_account)
struct card_account
{
	char user_name[9];     //���֣������������ĸ����֣��ַ�����ʽ��������������
	int card_number;       //��Ա���ţ��������,���+�·�+��+��λ��������
	char sex[4];           //�Ա�
	char time_begin[11];   //�쿨ʱ�䣬��ʽΪ   2019.03.03    Ӣ��״̬������
	float pay_sum;         //�����ܶ� 
	float discount_rate;   //�ۿ���
	float charge;          //ʵ���շ�
    float consume_sum;     //�����ܶ�
	float residue_money;   //ʣ����
	card_account *next;
};


card_account *head=NULL;
int total_number=0;       //��Ա����
char CPN[52];             //��Ź�˾����
char password[17];        //��ŵ�¼����


void prepare();           //���������ı��е���Ϣ������ɹ�����ɹ���ʾ���������������ʾ
void welcome();
void new_nember();        //�½�һ����Ա��Ϣ��д���ĵ�     (�������������������Ӳ���)
void delete_number();     //ɾ��һ����Ա��Ϣ�����ĵ���ɾ��     (����������������ɾ������)
void renew();             //���Ѻ��޸��ĵ���Ϣ     (������������������Ĳ���)��
void once_pay();        //���㵥��������Ϣ
void search();           //����������ϲ��������Ļ�Ա��Ϣ�����ҷ�ʽ�а����������š��쿨����     (������������������Ҳ���)
void search_with_name();
void search_with_cardnumber();
void search_with_timebegin();

int main()
{
	prepare();
	int mark;
	while(1)
	{
		welcome();
		printf("����������ѡ��");
		scanf("%d",&mark);
		switch(mark)
		{
			case 1:new_nember();break;
			case 2:renew();break;
			case 3:once_pay();break;
			case 4:delete_number();break;
			case 5:search();break;
			case 0:exit(0);
			default :printf("��������Ч���������������롣\n");
		}
	}
	return 0;
}
void new_nember()
{
    card_account *p;
    int i,n,number_mark=0,flag;
    float charge,residue_money,discount_rate;
    char user_name[9];
    FILE *fp;
    system("cls");
    printf("��Ҫ��Ӽ�����Ϣ��");
    scanf("%d",&n);
    printf("**********************************************************************************\n��ʽ������ 2019010202 �� 2019.03.03 6000\n");
    printf("����    ����   �Ա�  ���ʱ��  �ɷѽ��\n");
    for(i=0;i<n;i++)
    {
        p=head;
        flag=0;
       card_account *r=(card_account *)malloc (sizeof(card_account));
       scanf("%s %d %s %s %f",r->user_name,&r->card_number,r->sex,r->time_begin,&r->pay_sum);
       while(p!=0)
       {
           if(p->card_number==r->card_number)
           {
               number_mark=1;                          //�����ظ�
               flag=1;                                 //������޸ĳ���
           }
           p=p->next;
       }
       while(number_mark==1)
       {
           number_mark=0;
            p=head;
            printf("������Ŀ����ظ�������������������Ϣ��\n");
            scanf("%s %d %s %s %f",r->user_name,&r->card_number,r->sex,r->time_begin,&r->pay_sum);
            while(p!=0)
            {
                if(p->card_number==r->card_number)
                number_mark=1;                         //�����ظ�
                p=p->next;
            }
       }
       r->consume_sum=0;
       if(flag==1&&(i!=(n-1)))printf("�޸ĳɹ�������������������(����Ѿ�����������Ϣ�������).\n");
       switch ((int)(r->pay_sum/1000))
       {
           case 0:r->discount_rate=0;
           case 1:r->discount_rate=0.95;break;
           case 2:r->discount_rate=0.90;break;
           case 3:r->discount_rate=0.85;break;
           case 4:r->discount_rate=0.80;break;
           case 5:r->discount_rate=0.75;break;
           default :r->discount_rate=0.70;
       }
       r->charge=r->pay_sum*(discount_rate/100);
       r->residue_money=r->pay_sum-r->charge;
       total_number++;
       if(total_number==1)
       {
           r->next=NULL;
           head=r;
       }
       else
       {
           card_account *last=head;
           while(last->next!=NULL)
                last=last->next;
            last->next=r;
            r->next=NULL;
       }
       if((fp=fopen("number.txt","ab+"))==NULL)    //���ļ�����׷��
            fp=fopen("number.txt","wb+");
        fprintf(fp,"%s %d %s %s %f %f %f %f %f\n",r->user_name,r->card_number,r->sex,r->time_begin,r->pay_sum,r->discount_rate,r->charge,r->residue_money,r->consume_sum);
        fclose(fp);
    }
    printf("�ѳɹ����%d����Ա��\n",n);
    system("pause");
}
void delete_number()
{
    int d,flag=0;
    card_account *p,*before;
    p=head;
    FILE *fp;
    system("cls");
    printf("��������Ҫɾ���Ļ�Ա���ţ�");
    scanf("%d",&d);
    while(p)
    {
        if(p->card_number==d)
        {
            printf("��ǰ��%.2f",p->residue_money);
            if(p==head)
            {
                head=p->next;
            }
            else
            {
                if(p->next==NULL)
                {
                    p=NULL;
                }
                else
                {
                    before->next=p->next;
                }
            }
            flag=1;
            printf("ɾ���ɹ���\n");
            total_number--;
            break;
        }
        before=p;
        p=p->next;
    }
    if(flag==0)printf("�Ҳ����ÿ���\n");
    remove("number.txt");
    if((fp=fopen("number.txt","wb+"))!=0)
    {
        p=head;
        while(p!=NULL)
        {
            fprintf(fp,"%s %d %s %s %f %f %f %f %f\n",p->user_name,p->card_number,p->sex,p->time_begin,p->pay_sum,p->discount_rate,p->charge,p->residue_money,p->consume_sum);
            p=p->next;
        }
        fclose(fp);
    }
    system("pause");
}
void renew()
{
    int d,e=-1;
    char user_name[9],sex[4],time_begin[7];
    float residue_money;
    card_account *p,*q;
    FILE *fp;
    p=head;
    q=p;
    system("cls");;
    printf("��������Ҫ�޸ĵĻ�Ա�ţ�");
    scanf("%d",&d);
    while(q)
    {
        if(q->card_number==d)
        {
            while(e!=0)
            {
                system("cls");
                printf("��������Ҫ�޸ĵ���Ϣ��\n");
                printf("1.����\n");
                printf("2.���ʱ��\n");
                printf("3.�Ա�\n");
                printf("4.��Ա���ɷ�\n");
                printf("0.�˳�\n");
                scanf("%d",&e);
                switch(e)
                {
                    case 1:
                    printf("�޸�������%s\n",q->user_name);
                    printf("��Ҫ�޸ĵ�������");
                    scanf("%s",user_name);
                    strcpy(q->user_name,user_name);
                    printf("\n�޸ĳɹ�\n");
                    e=0;
                    break;
                    case 2:
                    printf("�Ѽ�¼�����ʱ�䣺%s\n",q->time_begin);
                    printf("��Ҫ�޸ĵ����ʱ�䣺");
                    scanf("%s",time_begin);
                    strcpy(q->time_begin,time_begin);
                    printf("\n�޸ĳɹ�\n");
                    e=0;
                    break;
                    case 3:
                    printf("�޸��Ա�%s\n",q->sex);
                    printf("��Ҫ�޸ĵ��Ա�");
                    scanf("%s",sex);
                    strcpy(q->sex,sex);
                    printf("\n�޸ĳɹ�\n");
                    e=0;
                    break;
                    case 4:
                    printf("������%.2f\n",q-> residue_money);
                    printf("��������Ҫ��ֵ�Ľ�");
                    scanf("%f", &residue_money);
                    q-> residue_money+= residue_money;
                    q->pay_sum+=residue_money;
                    printf("\n�޸ĳɹ�\n");
                    e=0;
                    break;
                    case 0:
                    e=0;
                    break;
                }
            }
        }
        if(e==0)break;
        q=q->next;
    }
    if(q==0)printf("�Ҳ����ÿ��ţ�\n");
    else
    {
        remove("number.txt");
        if((fp=fopen("number.txt","wb+"))!=0)
        {
            while(p!=NULL)
            {
                fprintf(fp,"%s %d %s %s %f %f %f %f %f\n",p->user_name,p->card_number,p->sex,p->time_begin,p->pay_sum,p->discount_rate,p->charge,p->residue_money,p->consume_sum);
                p=p->next;
            }
            fclose(fp);
        }
    }
    system("pause");
}
void once_pay()
{
    int d;
    int card_number;
    float pay_sum,charge;
    card_account *p,*q;
    FILE *fp;
    p=head;
    system("cls");
    printf("���������Ŀ��ţ�");
    scanf("%d",&d);
    while(p)
    {
        if(p->card_number==d)
        {
            printf("�����뱾�����ѽ�");
            scanf("%f",&pay_sum);
            printf("ʵ���շѣ�");
            charge=pay_sum*p->discount_rate;
            p->consume_sum+=charge;
            printf("%.2f,",charge);
            printf("ʣ���");
            p->residue_money=p->residue_money-charge;
            printf("%.2f\n",p->residue_money);
            system("pause");
            break;
        }
        p=p->next;
    }
    remove("number.txt");
    if((fp=fopen("number.txt","wb+"))!=0)
    {
        p=head;
        while(p!=NULL)
        {
            fprintf(fp,"%s %d %s %s %f %f %f %f %f\n",p->user_name,p->card_number,p->sex,p->time_begin,p->pay_sum,p->discount_rate,p->charge,p->residue_money,p->consume_sum);
            p=p->next;
        }
        fclose(fp);
    }
}
void prepare()
{
    int mark=0;
    char password_scanf[17];
	FILE *fp;
	card_account *new_,*last;
	if((fp=fopen("data.txt","rb+"))==0)    //��ʧ�ܣ������״�ʹ��
	{
		printf("��⵽�����״�ʹ�ã����������Ĺ�˾���ƣ�");
		scanf("%s",CPN);
        printf("���������ĵ�¼����(������16�����ȵ����ֺ���ĸ�����):\n");
        scanf("%s",&password);
		if((fp=fopen("data.txt","wb+"))!=0)   //�½��ĵ����ڱ�������
		{
			fwrite(CPN,sizeof(char),strlen(CPN),fp);
            fputc('\n',fp);
            fwrite(password,sizeof(char),strlen(password),fp);
			fclose(fp);
		}
		else
		{
			printf("�ļ���ʧ�ܣ������볢������ϵͳ......\n");
		}
		system("cls");
	}
	else                                        //�򿪳ɹ�����ȡ��˾����
	{
		fgets(CPN,51,fp);
        CPN[strlen(CPN)-1]='\0';
        fgets(password,17,fp);                   //��ȡ��½����
	}
	fclose(fp);
	if((fp=fopen("number.txt","rb"))!=0)          //���ػ�Ա��Ϣ���ڴ�
	{
		while(!feof(fp))
		{
			new_=(card_account*)malloc(LEN);
			fscanf(fp,"%s %d %s %s %f %f %f %f %f\n",new_->user_name,&new_->card_number,new_->sex,new_->time_begin,&new_->pay_sum,&new_->discount_rate,&new_->charge,&new_->residue_money,&new_->consume_sum);
			total_number++;
			if(total_number==1)
			{
				head=new_;
				last=new_;
                new_->next=NULL;
			}
			else
			{
				last->next=new_;
				last=new_;
                last->next=NULL;
			}
		}
	}
    fclose(fp);
    while(mark<=0)
    {
        printf("���������ĵ�½���룺\n");
        scanf("%s",password_scanf);
        if(strcmp(password,password_scanf)==0)
        {
            mark=1;
            printf("��½�ɹ���\n");
        }
        else
        {
            printf("������󣬵�½ʧ�ܣ�\n");
            mark--;
            if(mark==-3)
            {
                printf("�������������࣬���ر�Ӧ�á�\n");
                system("pause");
                exit(0);
            }
        }
    }
}
void welcome()
{
    system("cls");
    printf("\t\t\t\t   ��ӭʹ��%s��Ա������ϵͳ\t\t\t\t\n",CPN);
	printf("\t\t\t\tΪ��֤�����������У��밴���������˳���ϵͳ\t\t\t\t\n");
	for(int i=0;i<99;i++)
	putchar('*');
	putchar('\n');
	printf("*");
	for(int i=0;i<97;i++)
	putchar(' ');
	putchar('*');
	putchar('\n');
	printf("*                                       ���롾1���½���Ա��Ϣ                                     *\n");
	printf("*                                       ���롾2����Ա��Ϣ�޸�                                     *\n");
	printf("*                                       ���롾3����Ա���ѽ���                                     *\n");
	printf("*                                       ���롾4����Ա�˿�                                         *\n");
	printf("*                                       ���롾5�����һ�Ա                                         *\n");
	printf("*                                       ���롾0���˳���ϵͳ                                       *\n");
	printf("*");
	for(int i=0;i<97;i++)
	putchar(' ');
	putchar('*');
	putchar('\n');
	for(int i=0;i<99;i++)
	putchar('*');
	putchar('\n');
}
void search()
{
	char mode;
    system("cls");
	if(head==NULL)
	{
		printf("û�л�Ա��Ϣ.\n");
		system("pause") ;
	}
	else
	{
		printf("����1�������������в���\n����2�����տ��Ž��в���\n����3�����հ쿨���ڽ��в���\n");
		fflush(stdin);
		mode = getchar();
		switch(mode)
		{
			case'1':search_with_name();break;
			case'2':search_with_cardnumber();break;
			case'3':search_with_timebegin();break;
			default:printf("ѡ����󣬽����������档"); 
		}
	}
	system("pause");
}

void search_with_name()
{
	card_account *list = head;
	char name[9];
    int flag = 0;
	printf("��������Ҫ���ҵ����֣�"); 
	scanf("%s",name);
	while(list != NULL)
	{
        printf("  ����     ����    �Ա�  �������   �ɷ��ܶ�  �����ۿ� ��ǰ���\n");
		if(strcmp(name,list->user_name)==0)
		{
			printf("%8s %10d %2s %10s %10.2f    %.2f   %.2f\n",list->user_name,list->card_number,list->sex,list->time_begin,list->pay_sum,list->discount_rate,list->residue_money);
            flag = 1;
		}
		list = list->next;
	}
    if(flag == 0)printf("δ���ҵ���Ӧ�Ļ�Ա��Ϣ���Ա��ϢΪ�գ�\n");
}
void search_with_cardnumber()
{
	card_account *list = head;
	int card_number,flag = 0;
	printf("��������Ҫ���ҵĿ��ţ�\n");
	scanf("%d",&card_number);
	while(list != NULL)
	{
		if(card_number == list->card_number)
		{
			printf("  ����     ����    �Ա�  �������   �ɷ��ܶ�  �����ۿ� ��ǰ���\n");
			printf("%8s %10d %2s %10s %10.2f    %.2f   %.2f\n",list->user_name,list->card_number,list->sex,list->time_begin,list->pay_sum,list->discount_rate,list->residue_money);
            flag = 1;
			break;
		}
		list = list->next;
	}
    if(flag == 0)printf("δ���ҵ���Ӧ�Ļ�Ա��Ϣ���Ա��ϢΪ�գ�\n");
}
void search_with_timebegin()
{
	card_account *list = head;
	char time[11];
    int flag = 0;
	printf("������������ڣ�\n");
	scanf("%s",time);
    printf("  ����     ����    �Ա�  �������   �ɷ��ܶ�  �����ۿ� ��ǰ���\n");
	while(list != NULL)
	{
		if(strcmp(time,list->time_begin)==0)
		{
			printf("%8s %10d %2s %10s %10.2f    %.2f   %.2f\n",list->user_name,list->card_number,list->sex,list->time_begin,list->pay_sum,list->discount_rate,list->residue_money);
            flag = 1;
		}
		list = list->next;
	}
    if(flag == 0)printf("δ���ҵ���Ӧ�Ļ�Ա��Ϣ���Ա��ϢΪ�գ�\n");
}
