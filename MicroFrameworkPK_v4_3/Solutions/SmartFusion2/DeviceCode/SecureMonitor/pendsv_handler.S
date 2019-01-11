//Author: Mukundan Sridharan
//Handles the PendSV interrupt to execute a function call from user space
//into kernel space and returns the result.
//Works together with task_ctx_t definiton in cmx_ctx.h.
//If you change that definition, this will not work anymore

.syntax unified
.cpu cortex-m3
//.fpu softvfp

.thumb

.global PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
	// Disable interrupts:
	cpsid	i

	ldr	r1, =userCallCtx
	ldr r0,[r1]
	adds r0, #48 //get to r8
	ldmia	r0!,{r4-r7} //load 4reg to r4-r7
	mov	r8, r4
	mov	r9, r5
	mov	r10, r6
	mov	r11, r7
	//r0 should now point to after r11, subtract 64 to go address of r4
	subs	r0, #32
	ldmia	r0!,{r4-r7}

	//done loading back r4-r11
	//temp store on stack
	push {r7}

	//load regs r0-r3 from usercallctx
	ldr	r7, =userCallCtx
	ldmia r7!,{r0-r3}

	ldr r7, =userCallPC //branch address
	mov r12, r7

	// Enable interrupts:
	cpsie	i
	bx	r12

	//write results back into user stack
	ldr r7, =userCallCtx
	adds r7, #64 //get to stackpointer address
	ldr r5, [r7]
	adds r5, #5 //copy LR address
	mov r6,r5
	adds r6, #1 //copy pc address
	ldr r4, [r5]
	str r4, [r6]
	ldr r7, =userCallCtx
	stmia	r7!,{r0-r3}
	pop {r7}

	//return path, get out interrupt
	ldr r0, =0xFFFFFFFD
	bx r0


.size PendSV_Handler, .-PendSV_Handler
