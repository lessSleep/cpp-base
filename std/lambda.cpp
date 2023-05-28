// 作者：matrix
// 链接：https://www.zhihu.com/question/54933991/answer/3028067982
// 来源：知乎
// 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
#include "bits/stdc++.h"
int lambda_main (int n)
{
    int (*lambda_instance)(int arg);
    static char *who_str = "abcd";
    int who_digit = 2;
    long long  i = 1;
    lambda_instance = lambda_def (who_digit, who_str);
    printf ("%d\n\n", lambda_instance (n));
    while (i < 10000 * 1000  /* 10000* 10000 * 10 */)
    {
        lambda_instance = lambda_def (who_digit, who_str); // ==> lambda 每次调用每次实例方式 
        printf ("%d\n\n", lambda_instance (i)); // ==> lambda 一次实例多次调用方式
        //lambda_common_fun(i);              // ==> C过程  通用方式调用lambda过程
        i++;
    }
    //wait_return(true);
    return 0;
}

// lambda  过程就一条简单的乘法命令，time_call_c  是wrap 的计时器
int (*lambda_def (int capture, char * capture_str))(int arg)
{
    int inside(int arg )
    {
        char *save =  capture_str;
        int save_digit = capture;
        return arg * save_digit;
    }
    return inside;
}

int main(){
  printf ("调用时间：%d\n\n",  time_call_c(lambda_main, 999));
}
