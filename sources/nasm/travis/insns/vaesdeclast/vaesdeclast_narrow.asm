	vaesdeclast xmm7, xmm5, oword [0xfa3]
	vaesdeclast xmm7, oword [0xfa3]
	vaesdeclast xmm6, xmm4, xmm2
	vaesdeclast ymm1, ymm2, ymm7
	vaesdeclast ymm1, ymm7
	vaesdeclast ymm6, ymm5, yword [0x32a]
	vaesdeclast zmm3, zmm1, zmm4
	vaesdeclast zmm3, zmm4
	vaesdeclast zmm5, zmm6, zmm2

%ifdef ERROR
	vaesdeclast xmm15, xmm8, xmm9
	vaesdeclast ymm12, ymm14, ymm10
	vaesdeclast zmm14, zmm11, zmm14
	vaesdeclast xmm19, xmm28, xmm25
	vaesdeclast ymm16, ymm28, ymm22
	vaesdeclast zmm23, zmm31, zmm16
%endif
