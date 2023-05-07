; 宏 ------------------------------------------------------------------------------------------------------
;
; 描述符
; usage: handle name
%macro handle  1
global exception_handler_%1 
extern do_handler_%1 

exception_handler_%1:
    cli
	push	rax
	push	rbx
	push	rcx
	push	rdx
	push	rbp
	push	rdi
	push	rsi

	push	r8
	push	r9
	push	r10
	push	r11
	push	r12
	push	r13
	push	r14
	push	r15

    call do_handler_%1

	pop   r15
	pop   r14
	pop   r13
	pop   r12
	pop   r11
	pop   r10
	pop   r9
	pop   r8

	pop   rsi
	pop   rdi
	pop   rbp
	pop   rdx
	pop   rcx
	pop   rbx
	pop   rax

	iretq
%endmacro


handle unknown
handle divider
handle Debug
handle NMI
handle breakpoint
handle overflow
handle bound_range
handle invalid_opcode
handle device_unavailable
handle double_fault
handle invalid_tss
handle segment_not_present
handle stack_segment_fault
handle general_protection
handle page_fault
handle fpu_error
handle alignment_check
handle machine_check
handle smd_exception
handle virtual_exception
handle timer 
handle keyboard
