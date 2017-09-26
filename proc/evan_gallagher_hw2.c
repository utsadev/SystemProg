#include<signal.h>
#include<setjmp.h>
#include"tlpi_hdr.h"

static jmp_buf env;



void intHandler()
{
    printf("This is an interrupt handler\n");
    longjmp(env,2);
    exit(EXIT_SUCCESS);
}

void func1()
{
    printf("Good Morning!\n");
    while(1) /*Infinite loop*/
    {
        printf("Looping...\n");
        sleep(1);
    }

}

void func2()
{
    printf("Good Bye!\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
/*signal calls interrupt handler (intHandler) when control + 'c' is typed*/
    //env is declared at the begging on program
    signal(SIGINT, intHandler);
    if(setjmp(env)==0)
        func1();
    
    func2();

    exit(EXIT_SUCCESS); 
}