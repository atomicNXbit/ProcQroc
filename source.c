/*
Copyright (C) 2025 atomicNXbit

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
