//GNU General Public License v3.0 atomicNXbit

#include <stdio.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>


struct user_regs_struct regs;


int procAttach(pid_t x){

    ptrace(PTRACE_ATTACH, x, NULL, NULL);
    waitpid(x, NULL, 0);
    if(errno){
        perror("ptrace_attach");
        return -1;
    } 
    printf("\nSuccessfully attached");
    ptrace(PTRACE_GETREGS, x, NULL, &regs);
    unsigned long instr = ptrace(PTRACE_PEEKDATA, x, regs.rip, NULL);
    printf("\nIP -> %llX and current instruction being ran: %lX", regs.rip, instr);
    printf("\nSP -> %llX", regs.rsp);
    printf("\nBP -> %llX", regs.rbp);

    ptrace(PTRACE_DETACH, x, NULL, NULL);
    if(errno){
        perror("ptrace_detach");
        return -2;
    }
    printf("\nSuccessfully detached");
    return 0;
}

int main(){

    int userInput;
    printf("PID: ");
    scanf("%d", &userInput);
    procAttach(userInput);
    return 0;

}
