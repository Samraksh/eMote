
	.global arm_write_cr1
	.global arm_invalidate_tlb
	.global arm_read_cr1
	.global arm_write_ttbr
	.global arm_write_dacr

arm_write_cr1:
	mcr		p15, 0, r0, c1, c0, 0
	bx		lr


/* void arm_invalidate_tlb(void) */
arm_invalidate_tlb:
	mov		r0, #0
	mcr 	p15, 0, r0, c8, c7, 0
	bx		lr

/* uint32_t arm_read_cr1(void) */
arm_read_cr1:
	mrc		p15, 0, r0, c1, c0, 0
	bx		lr

/* void arm_write_ttbr(uint32_t val) */
arm_write_ttbr:
	mcr 	p15, 0, r0, c2, c0, 0
	bx		lr

/* void arm_write_dacr(uint32_t val) */
arm_write_dacr:
	mcr 	p15, 0, r0, c3, c0, 0
	bx		lr
