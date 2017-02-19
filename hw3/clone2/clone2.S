#ifdef PIC
#define SYSCALL_ERROR_LABEL 0f
#else
#define SYSCALL_ERROR_LABEL syscall_error
#endif
#include "sysdep.h"
#define ERRNO_H        1
#include <bits/errno.h>
#define HIDDEN_JUMPTARGET(name) JUMPTARGET(name)
#define CLONE_VM        0x00000100

   /* The userland implementation is:
      int clone (int (*fn)(void *arg), void *child_stack, int flags,
		 void *arg, int vector_id),

      The parameters are passed in register and on the stack from userland:
      rdi: fn
      rsi: child_stack
      rdx: flags
      rcx: arg
      r8d: vector id
      r9d: TID field in parent
   %esp+8: thread pointer
   %esp+8: TID field in child

      The kernel expects:
      rax: system call number
      rdi: flags
      rsi: child_stack
      rdx: TID field in parent
      r10: TID field in child
      r8 : thread pointer
      r9 : vector id*/

           .text
	   .globl clone2
	   .type clone2, @function
	   clone2:
           /* Sanity check arguments.  */
	   cfi_startproc;
           movq    $-0x16,%rax
           testq   %rdi,%rdi               /* no NULL function pointers */
           jz      SYSCALL_ERROR_LABEL
           testq   %rsi,%rsi               /* no NULL stack pointers */
           jz      SYSCALL_ERROR_LABEL

           /* Insert the argument onto the new stack.  */
           subq    $16,%rsi
           movq    %rcx,8(%rsi)

           /* Save the function pointer.  It will be popped off in the
              child in the ebx frobbing below.  */
           movq    %rdi,0(%rsi)

           /* Do the system call.  */
	   movq    %rdx, %rdi
           movq    %r9, %rdx
           movq    %r8, %r9
	   movq    8(%rsp), %r8
	   movq    16(%rsp), %r10

           movl    $0x149,%eax

       /* End FDE now, because in the child the unwind info will be
            wrong.  */
         cfi_endproc;
         syscall

         testq   %rax,%rax
         jl      SYSCALL_ERROR_LABEL
         jz      L(thread_start)

         ret

 L(thread_start):
         cfi_startproc;
         /* Clearing frame pointer is insufficient, use CFI.  */
         cfi_undefined (rip);
        /* Clear the frame pointer.  The ABI suggests this be done, to mark
            the outermost frame obviously.  */
         xorl    %ebp, %ebp

         /* Set up arguments for the function call.  */
         popq    %rax            /* Function to call.  */
         popq    %rdi            /* Argument.  */
         call    *%rax
          /* Call exit with return value from function call. */
         movq    %rax, %rdi
         call    HIDDEN_JUMPTARGET (_exit)
         cfi_endproc;

          cfi_startproc;
 PSEUDO_END (clone2)
