/* SPDX-License-Identifier: GPL-2.0-only */

#include <cpu/x86/post_code.h>

.global bootblock_pre_c_entry
bootblock_pre_c_entry:

cache_as_ram:
	post_code(0x20)
	/*
	 * Nothing to do here on qemu, RAM works just fine without any
	 * initialization.
	 */

	/* Clear the cache memory region. This will also clear CAR GLOBAL */
	movl	$_car_region_start, %edi
	movl	$_car_region_end, %ecx
	sub	%edi, %ecx
	shr	$2, %ecx
	xorl	%eax, %eax
	rep	stosl

	post_code(0x21)

	movl	$_ecar_stack, %esp

	/* Align the stack and keep aligned for call to bootblock_c_entry() */
	and	$0xfffffff0, %esp

	/* Restore the BIST result and timestamps. */
#if defined(__x86_64__)
	movd	%mm1, %rdi
	shld	%rdi, 32
	movd	%mm1, %rsi
	or	%rsi, %rdi
	movd	%mm2, %rsi
#else
	sub	$4, %esp

	movd	%mm0, %ebx
	movd	%mm1, %eax
	movd	%mm2, %edx

	pushl	%ebx
	pushl	%edx
	pushl	%eax
#endif

before_c_entry:
	post_code(0x29)
	call	bootblock_c_entry_bist
	/* Never returns */
.Lhlt:
	post_code(POST_DEAD_CODE)
	hlt
	jmp	.Lhlt
