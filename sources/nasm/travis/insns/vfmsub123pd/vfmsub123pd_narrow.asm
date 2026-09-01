	vfmsub123pd xmm6, xmm6, xmm1
	vfmsub123pd xmm5, xmm4, oword [0x922]
	vfmsub123pd ymm6, ymm5, ymm2
	vfmsub123pd ymm5, ymm7, yword [0xb2a]

%ifdef ERROR
	vfmsub123pd xmm11, xmm15, xmm8
	vfmsub123pd ymm8, ymm10, ymm8
%endif
