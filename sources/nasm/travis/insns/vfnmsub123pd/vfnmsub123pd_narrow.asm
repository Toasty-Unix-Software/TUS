	vfnmsub123pd xmm5, xmm7, oword [0xbcb]
	vfnmsub123pd xmm3, xmm5, oword [0x5a9]
	vfnmsub123pd ymm0, ymm5, yword [0x715]
	vfnmsub123pd ymm2, ymm4, yword [0x31b]

%ifdef ERROR
	vfnmsub123pd xmm10, xmm10, xmm13
	vfnmsub123pd ymm10, ymm12, ymm8
%endif
