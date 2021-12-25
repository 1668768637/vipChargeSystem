#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<string.h>
#define LEN sizeof(card_account)
struct card_account
{
	char user_name[9];     //名字，最多可以输入四个汉字，字符串格式，可以输入中文
	int card_number;       //会员卡号，五个数字,年份+月份+日+三位数的排序
	char sex[4];           //性别
	char time_begin[11];   //办卡时间，格式为   2019.03.03    英文状态下输入
	float pay_sum;         //消费总额 
	float discount_rate;   //折扣率
	float charge;          //实际收费
    float consume_sum;     //消费总额
	float residue_money;   //剩余金额
	card_account *next;
};


card_account *head=NULL;
int total_number=0;       //会员总数
char CPN[52];             //存放公司名称
char password[17];        //存放登录密码


void prepare();           //用于载入文本中的信息，载入成功输出成功提示，否则输出错误提示
void welcome();
void new_nember();        //新建一个会员信息并写入文档     (抽象数据类型链表增加操作)
void delete_number();     //删除一个会员信息并从文档中删除     (抽象数据类型链表删除操作)
void renew();             //续费和修改文档信息     (抽象数据类型链表更改操作)；
void once_pay();        //结算单次消费信息
void search();           //用于输出符合查找条件的会员信息，查找方式有按姓名、卡号、办卡日期     (抽象数据类型链表查找操作)
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
		printf("请输入您的选择：");
		scanf("%d",&mark);
		switch(mark)
		{
			case 1:new_nember();break;
			case 2:renew();break;
			case 3:once_pay();break;
			case 4:delete_number();break;
			case 5:search();break;
			case 0:exit(0);
			default :printf("输入项无效！！！请重新输入。\n");
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
    printf("想要添加几人信息：");
    scanf("%d",&n);
    printf("**********************************************************************************\n格式如张三 2019010202 男 2019.03.03 6000\n");
    printf("姓名    卡号   性别  入会时间  缴费金额\n");
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
               number_mark=1;                          //卡号重复
               flag=1;                                 //进入过修改程序
           }
           p=p->next;
       }
       while(number_mark==1)
       {
           number_mark=0;
            p=head;
            printf("您输入的卡号重复，请重新输入所有信息：\n");
            scanf("%s %d %s %s %f",r->user_name,&r->card_number,r->sex,r->time_begin,&r->pay_sum);
            while(p!=0)
            {
                if(p->card_number==r->card_number)
                number_mark=1;                         //卡号重复
                p=p->next;
            }
       }
       r->consume_sum=0;
       if(flag==1&&(i!=(n-1)))printf("修改成功！请继续完成您的输入(如果已经输入所有信息，请忽略).\n");
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
       if((fp=fopen("number.txt","ab+"))==NULL)    //对文件进行追加
            fp=fopen("number.txt","wb+");
        fprintf(fp,"%s %d %s %s %f %f %f %f %f\n",r->user_name,r->card_number,r->sex,r->time_begin,r->pay_sum,r->discount_rate,r->charge,r->residue_money,r->consume_sum);
        fclose(fp);
    }
    printf("已成功添加%d个会员！\n",n);
    system("pause");
}
void delete_number()
{
    int d,flag=0;
    card_account *p,*before;
    p=head;
    FILE *fp;
    system("cls");
    printf("请输入需要删除的会员卡号：");
    scanf("%d",&d);
    while(p)
    {
        if(p->card_number==d)
        {
            printf("当前余额：%.2f",p->residue_money);
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
            printf("删除成功。\n");
            total_number--;
            break;
        }
        before=p;
        p=p->next;
    }
    if(flag==0)printf("找不到该卡号\n");
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
    printf("请输入需要修改的会员号：");
    scanf("%d",&d);
    while(q)
    {
        if(q->card_number==d)
        {
            while(e!=0)
            {
                system("cls");
                printf("请输入需要修改的信息：\n");
                printf("1.姓名\n");
                printf("2.入会时间\n");
                printf("3.性别\n");
                printf("4.会员卡缴费\n");
                printf("0.退出\n");
                scanf("%d",&e);
                switch(e)
                {
                    case 1:
                    printf("修改姓名：%s\n",q->user_name);
                    printf("想要修改的姓名：");
                    scanf("%s",user_name);
                    strcpy(q->user_name,user_name);
                    printf("\n修改成功\n");
                    e=0;
                    break;
                    case 2:
                    printf("已记录的入会时间：%s\n",q->time_begin);
                    printf("想要修改的入会时间：");
                    scanf("%s",time_begin);
                    strcpy(q->time_begin,time_begin);
                    printf("\n修改成功\n");
                    e=0;
                    break;
                    case 3:
                    printf("修改性别：%s\n",q->sex);
                    printf("想要修改的性别：");
                    scanf("%s",sex);
                    strcpy(q->sex,sex);
                    printf("\n修改成功\n");
                    e=0;
                    break;
                    case 4:
                    printf("您的余额：%.2f\n",q-> residue_money);
                    printf("请输入想要充值的金额：");
                    scanf("%f", &residue_money);
                    q-> residue_money+= residue_money;
                    q->pay_sum+=residue_money;
                    printf("\n修改成功\n");
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
    if(q==0)printf("找不到该卡号！\n");
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
    printf("请输入您的卡号：");
    scanf("%d",&d);
    while(p)
    {
        if(p->card_number==d)
        {
            printf("请输入本次消费金额：");
            scanf("%f",&pay_sum);
            printf("实际收费：");
            charge=pay_sum*p->discount_rate;
            p->consume_sum+=charge;
            printf("%.2f,",charge);
            printf("剩余金额：");
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
	if((fp=fopen("data.txt","rb+"))==0)    //打开失败，代表首次使用
	{
		printf("检测到您是首次使用，请输入您的公司名称：");
		scanf("%s",CPN);
        printf("请设置您的登录密码(不超过16个长度的数字和字母的组合):\n");
        scanf("%s",&password);
		if((fp=fopen("data.txt","wb+"))!=0)   //新建文档用于保存数据
		{
			fwrite(CPN,sizeof(char),strlen(CPN),fp);
            fputc('\n',fp);
            fwrite(password,sizeof(char),strlen(password),fp);
			fclose(fp);
		}
		else
		{
			printf("文件打开失败！！！请尝试重启系统......\n");
		}
		system("cls");
	}
	else                                        //打开成功，获取公司名称
	{
		fgets(CPN,51,fp);
        CPN[strlen(CPN)-1]='\0';
        fgets(password,17,fp);                   //获取登陆密码
	}
	fclose(fp);
	if((fp=fopen("number.txt","rb"))!=0)          //加载会员信息到内存
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
        printf("请输入您的登陆密码：\n");
        scanf("%s",password_scanf);
        if(strcmp(password,password_scanf)==0)
        {
            mark=1;
            printf("登陆成功！\n");
        }
        else
        {
            printf("密码错误，登陆失败！\n");
            mark--;
            if(mark==-3)
            {
                printf("密码错误次数过多，将关闭应用。\n");
                system("pause");
                exit(0);
            }
        }
    }
}
void welcome()
{
    system("cls");
    printf("\t\t\t\t   欢迎使用%s会员卡管理系统\t\t\t\t\n",CPN);
	printf("\t\t\t\t为保证程序正常运行，请按正常步骤退出本系统\t\t\t\t\n");
	for(int i=0;i<99;i++)
	putchar('*');
	putchar('\n');
	printf("*");
	for(int i=0;i<97;i++)
	putchar(' ');
	putchar('*');
	putchar('\n');
	printf("*                                       输入【1】新建会员信息                                     *\n");
	printf("*                                       输入【2】会员信息修改                                     *\n");
	printf("*                                       输入【3】会员消费结算                                     *\n");
	printf("*                                       输入【4】会员退卡                                         *\n");
	printf("*                                       输入【5】查找会员                                         *\n");
	printf("*                                       输入【0】退出本系统                                       *\n");
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
		printf("没有会员信息.\n");
		system("pause") ;
	}
	else
	{
		printf("输入1：按照姓名进行查找\n输入2：按照卡号进行查找\n输入3：按照办卡日期进行查找\n");
		fflush(stdin);
		mode = getchar();
		switch(mode)
		{
			case'1':search_with_name();break;
			case'2':search_with_cardnumber();break;
			case'3':search_with_timebegin();break;
			default:printf("选项错误，将返回主界面。"); 
		}
	}
	system("pause");
}

void search_with_name()
{
	card_account *list = head;
	char name[9];
    int flag = 0;
	printf("请输入你要查找的名字："); 
	scanf("%s",name);
	while(list != NULL)
	{
        printf("  姓名     卡号    性别  入会日期   缴费总额  享受折扣 当前余额\n");
		if(strcmp(name,list->user_name)==0)
		{
			printf("%8s %10d %2s %10s %10.2f    %.2f   %.2f\n",list->user_name,list->card_number,list->sex,list->time_begin,list->pay_sum,list->discount_rate,list->residue_money);
            flag = 1;
		}
		list = list->next;
	}
    if(flag == 0)printf("未查找到对应的会员信息或会员信息为空！\n");
}
void search_with_cardnumber()
{
	card_account *list = head;
	int card_number,flag = 0;
	printf("请输入你要查找的卡号：\n");
	scanf("%d",&card_number);
	while(list != NULL)
	{
		if(card_number == list->card_number)
		{
			printf("  姓名     卡号    性别  入会日期   缴费总额  享受折扣 当前余额\n");
			printf("%8s %10d %2s %10s %10.2f    %.2f   %.2f\n",list->user_name,list->card_number,list->sex,list->time_begin,list->pay_sum,list->discount_rate,list->residue_money);
            flag = 1;
			break;
		}
		list = list->next;
	}
    if(flag == 0)printf("未查找到对应的会员信息或会员信息为空！\n");
}
void search_with_timebegin()
{
	card_account *list = head;
	char time[11];
    int flag = 0;
	printf("请输入入会日期：\n");
	scanf("%s",time);
    printf("  姓名     卡号    性别  入会日期   缴费总额  享受折扣 当前余额\n");
	while(list != NULL)
	{
		if(strcmp(time,list->time_begin)==0)
		{
			printf("%8s %10d %2s %10s %10.2f    %.2f   %.2f\n",list->user_name,list->card_number,list->sex,list->time_begin,list->pay_sum,list->discount_rate,list->residue_money);
            flag = 1;
		}
		list = list->next;
	}
    if(flag == 0)printf("未查找到对应的会员信息或会员信息为空！\n");
}
