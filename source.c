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




#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>



struct user_regs_struct regs;

void HexDump(const void *data, size_t size) {
    const unsigned char *p = data;
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", p[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}


int procAttach(pid_t x){


    ptrace(PTRACE_SEIZE, x, 0, 0);
    if(errno){
        perror("ptrace_seize");
        return -1;
    } 

    ptrace(PTRACE_INTERRUPT, x, 0, 0);
    waitpid(x, NULL, 0);

    printf("\nSuccessfully attached");
    ptrace(PTRACE_GETREGS, x, NULL, &regs);
    printf("\nSpecial:");
    printf("\nRIP-> %llX", regs.rip);
    printf("\nRSP-> %llX", regs.rsp);
    printf("\nRBP-> %llX", regs.rbp);
    printf("\nFS-> %llX", regs.fs_base);
    printf("\nGS-> %llX", regs.gs_base);
    printf("\nSystem V: ");
    printf("\nRDI-> %llX", regs.rdi);
    printf("\nRSI-> %llX", regs.rsi);
    printf("\nRDX-> %llX", regs.rdx);
    printf("\nRCX-> %llX", regs.rcx);
    printf("\nR8-> %llX", regs.r8);
    printf("\nR9-> %llX", regs.r9);
    printf("\nGeneral: ");
    printf("\nRAX-> %llX", regs.rax);
    printf("\nRBX-> %llX", regs.rbx);
    printf("\nR12-> %llX", regs.r12);
    printf("\nR13-> %llX", regs.r13);
    printf("\nR14-> %llX", regs.r14);
    printf("\nR15-> %llX", regs.r15);

    

    int getVal[256];
    ssize_t nread;
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = &getVal;
    local[0].iov_len = sizeof(getVal);
    remote[0].iov_base = (void * ) regs.rip;
    remote[0].iov_len = sizeof(getVal);

    nread = process_vm_readv(x, local, 1, remote, 1, 0);
    if(errno){
        ptrace(PTRACE_DETACH, x, NULL, NULL);
        perror("process_vm_readv");
        return -2;
    }
    printf("\n");

    HexDump(getVal, sizeof(getVal));

    ptrace(PTRACE_DETACH, x, NULL, NULL);
    if(errno){
        perror("ptrace_detach");
        return -3;
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
