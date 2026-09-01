	vmaxph xmm6, xmm4, oword [0xdb4]
	vmaxph xmm6, oword [0xdb4]
	vmaxph xmm6, xmm2, oword [0x671]
	vmaxph ymm6, ymm4, yword [0x279]
	vmaxph ymm6, yword [0x279]
	vmaxph ymm5, ymm0, ymm6
	vmaxph zmm4, zmm3, zmm7
	vmaxph zmm4, zmm7
	vmaxph zmm6, zmm4, zmm0

%ifdef ERROR
	vmaxph xmm10, xmm15, xmm8
	vmaxph ymm14, ymm9, ymm12
	vmaxph zmm15, zmm14, zmm8
	vmaxph xmm22, xmm16, xmm19
	vmaxph ymm18, ymm30, ymm17
	vmaxph zmm17, zmm20, zmm28
%endif
