#include <stdio.h>
#include <stdlib.h>

#define N 100

//比较两个操作符的大小判断符号是否入栈
int compare(char a, char b)
{
    int flag=0;
    if(b == '(')
    {
        return 1;
    }
    else if(a =='+'||a == '-')
    {
        switch (b)
        {
        case '+':
            flag = 0;
            break;
        case '-':
            flag = 0;
            break;
        case '*':
            flag = 1;
            break;
        }
    }
    else if(a == '*'||a =='/')
    {
        flag = 0;
    }
    else if(a=='(')
    {
        flag =1;
    }
    else if(a=='#')
    {
        flag = 1;
    }
    return flag;
}
//操作符栈
struct stack_op
{
    char op;
    struct stack_op *next;
};
typedef struct stack_op STACKOP;
//建立新操作符栈
void newops(STACKOP** ops)
{
    *ops = (STACKOP*)malloc(sizeof(struct stack_op));
    if (*ops != NULL)
    {
        (*ops)->op = '#';
        (*ops)->next = NULL;
    }
    else
    {
        printf("No enough memory!");
        exit(0);
    }
}
//入栈
void push(STACKOP **ops,char c)
{
    STACKOP *p ;
    p=(STACKOP*)malloc(sizeof(STACKOP));
    if (ops != NULL)
    {
        p->op=c;
        p->next=*ops;
        *ops = p;
    }
    else
    {
        printf("No enough memory!");
        exit(0);
    }
}
//弹栈
char pop(STACKOP **ops)
{
    char s;
    STACKOP *p=*ops;
    s =p->op;
    *ops =(*ops)->next;
    free(p);
    return s;
}

//转后缀
void transform(char* input ,char* output)
{
    STACKOP *ops = NULL;
    newops(&ops);
    //flag用来判断前一个input的性质(1:操作符,0:数字,-1:负号,2:小数点)
    int i=0, j=0;
    int flag=1;
    char op;
    while(input[i]!='\0')
    {
        //处理数字
        if(input[i]>='0'&&input[i]<='9')
        {
            //前一个是操作符加空格分开,是数字,负号,小数点不加空格
            if(flag == 1)
            {
                *(output+j) = ' ';
                j++;
            }
            *(output+j) = input[i];
            j++;
            i++;
            flag=0;
        }
        else{
            //判断负号
            if(input[i]=='-'&&flag == 1)
            {
                *(output+j) = ' ';
                j++;
                *(output+j) = input[i];
                j++;
                i++;
                flag = -1;
            }
            //处理小数点
            else if(input[i]=='.'&&flag == 0)
            {
                *(output+j) = input[i];
                j++;
                i++;
                flag = 2;
            }
            //处理右括号
            else if(input[i]==')')
            {
                op=pop(&ops);
                while(op!='('&&ops->op!='#')
                {
                    *(output+j) = ' ';
                    j++;
                    *(output+j)=op;
                    j++;
                    op=pop(&ops);
                }
                if(ops->op=='#'){printf("没有左括号");}
                i++;
                flag=1;
            }
            //处理入栈符号
            else if(compare(ops->op,input[i])==1)
            {
                push(&ops,input[i]);
                i++;
                flag = 1;
            }
            //处理弹栈符号
            else if(compare(ops->op,input[i])==0)
            {
                do{
                    op = pop(&ops);
                    *(output+j) = ' ';
                    j++;
                    *(output+j) = op;
                    j++;
                }while(compare(ops->op,input[i])!=1);
                push(&ops,input[i]);
                i++;
                flag = 1;
            }
        }
    }
    //处理input完后栈内的操作符
    while(ops->op!='#')
    {
        op = pop(&ops);
        *(output+j) = ' ';
        j++;
        *(output+j) = op;
        j++;
    }
    *(output+j)='\0';
}



//算10的负k次方
float pow(int k)
{
    float p=1;
    while(k!=0)
    {
        p=p/10;
        k--;
    }
    return p;
}

//数字栈
struct stack_num
{
    float num;
    struct stack_num* next;
};
typedef struct stack_num STACKNUM;
//入栈
void pushnum(STACKNUM **nums,float n)
{
    STACKNUM *p ;
    p=(STACKNUM*)malloc(sizeof(STACKNUM));
    if (p != NULL)
    {
        p->num=n;
        p->next=*nums;
        *nums = p;
    }
    else
    {
        printf("No enough memory!");
        exit(0);
    }
}
//弹栈
float popnum(STACKNUM **nums)
{
    float n=0;
    STACKNUM *p=*nums;
    n=p->num;
    *nums =(*nums)->next;
    free(p);
    return n;
}
//字符数字转数字
float exchangenum(char* output,int* i)
{

            float num=0;
            int flag=1;//标识是负数还是正数
            if(output[*i]=='-'&&output[(*i)+1]>='0'&&output[(*i)+1]<='9')
            {
                flag = -1;
                (*i)++;
            }
            while(output[*i]>='0'&&output[*i]<='9')
            {
                num=(float)(output[*i]-'0')+num*10;
                (*i)++;
            }
            if(output[*i]=='.')
            {
                (*i)++;
                int k=1;//k用来标志小数点后几位
                while(output[*i]>='0'&&output[*i]<='9')
                {
                    num=num+((float)(output[*i]-'0'))*pow(k);
                    (*i)++;
                    k++;
                }
            }
            if(flag==-1){num=-num;}
            return num;
}

//从后缀表达式求值
float Sum(char* output)
{
    char ch;
    int i=0;
    STACKNUM *nums = NULL;
    while(output[i]!='\0')
    {
        if(output[i]==' ')
        {
            i++;
            //处理数字或负数
            if((output[i]=='-'&&output[i+1]>='0'&&output[i+1]<='9')||(output[i]>='0'&&output[i]<='9'))
            {
                float num=0;
                num=exchangenum(output,&i);
                pushnum(&nums,num);
            }
            //处理加号
            else if(output[i]=='+')
            {
                float a=0,b=0;
                b=popnum(&nums);
                a=popnum(&nums);
                pushnum(&nums,a+b);
                i++;
            }
            //处理减号
            else if(output[i]=='-')
            {
                float a=0,b=0;
                b=popnum(&nums);
                a=popnum(&nums);
                pushnum(&nums,a-b);
                i++;
            }
            //处理除号
            else if(output[i]=='/')
            {
                float a=0,b=0;
                b=popnum(&nums);
                a=popnum(&nums);
                if(b!=0)
                {
                   pushnum(&nums,a/b);
                   i++;
                }
                else{
                    printf("0不能做除数!");
                    exit(0);
                }
            }
            //处理乘号
            else if(output[i]=='*')
            {
                float a=0,b=0;
                b=popnum(&nums);
                a=popnum(&nums);
                pushnum(&nums,a*b);
                i++;
            }
        }
        else{i++;}
    }
    return popnum(&nums);
}


int main()
{
    char input[N],output[N];
    gets(input);
    transform(input,output);
    puts(output);
    printf("The result is %f\n",Sum(output));
    return 0;
}
