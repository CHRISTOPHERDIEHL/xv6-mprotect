#include "types.h"
#include "user.h"
#include "signal.h"

int *p;

void handler(int signum, siginfo_t info)
{
	printf(1,"Handler called, error address is 0x%x error: 0x%x\n", info.addr,info.type);
	if(info.type == PROT_READ)
	{
		printf(1,"ERROR: Writing to a page with insufficient permission.\n");
		mprotect((void *) info.addr, sizeof(int), PROT_READ | PROT_WRITE);
		printf(1,"MPROTECT call finished!\n");
	} else 	if(info.type == PROT_NONE)
	{
		printf(1,"ERROR: READING!.\n");
		mprotect((void *) info.addr, sizeof(int), PROT_READ);
	}
	else
	{
		printf(1, "ERROR: Didn't get proper exception, this should not happen.\n");
	}
	printf(1,"FINISHED IN HANDLER!\n");
}
int main(void)
{
	signal(SIGSEGV,(sighandler_t) handler);
 	p = (int *) sbrk(1);
 	mprotect((void *)p, sizeof(int), PROT_READ);
	printf(1,"About to read. should get memfault\n");
	*p =100;
  printf(1,"p = %d",*p);
 	printf(1, "COMPLETED: value is %d, expecting 100!\n", *p);

 	exit();
}
