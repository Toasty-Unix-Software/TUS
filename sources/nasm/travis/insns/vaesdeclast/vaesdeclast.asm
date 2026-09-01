default rel
	vaesdeclast xmm7, xmm5, oword [0xfa3]
	vaesdeclast xmm7, oword [0xfa3]
	vaesdeclast xmm6, xmm4, xmm2
	vaesdeclast ymm1, ymm2, ymm7
	vaesdeclast ymm1, ymm7
	vaesdeclast ymm6, ymm5, yword [0x32a]
	vaesdeclast zmm3, zmm1, zmm4
	vaesdeclast zmm3, zmm4
	vaesdeclast zmm5, zmm6, zmm2
	vaesdeclast xmm15, xmm8, xmm9
	vaesdeclast ymm12, ymm14, ymm10
	vaesdeclast zmm14, zmm11, zmm14
	vaesdeclast xmm19, xmm28, xmm25
	vaesdeclast ymm16, ymm28, ymm22
	vaesdeclast zmm23, zmm31, zmm16
	vaesdeclast xmm6, xmm7, oword [eax+1]
	vaesdeclast xmm7, xmm3, oword [eax+64]
	vaesdeclast ymm4, ymm2, yword [eax+1]
	vaesdeclast ymm0, ymm5, yword [eax+64]
	vaesdeclast zmm1, zmm5, zword [eax+1]
	vaesdeclast zmm4, zmm3, zword [eax+64]
	vaesdeclast xmm1, xmm1, [0x42e]
	vaesdeclast ymm0, ymm2, [0x2ae]
	vaesdeclast zmm4, zmm1, [0xbed]
