	vpmuludq xmm1, xmm3, xmm3
	vpmuludq xmm1, xmm3
	vpmuludq xmm7, xmm3, xmm4
	vpmuludq ymm6, ymm2, yword [0x4a0]
	vpmuludq ymm6, yword [0x4a0]
	vpmuludq ymm2, ymm1, ymm3
	vpmuludq xmm0, xmm0, xmm5
	vpmuludq xmm0, xmm5
	vpmuludq xmm3, xmm7, oword [0xa65]
	vpmuludq ymm0, ymm4, ymm2
	vpmuludq ymm0, ymm2
	vpmuludq ymm4, ymm1, ymm0

%ifdef ERROR
	vpmuludq xmm10, xmm14, xmm10
	vpmuludq ymm14, ymm12, ymm13
	vpmuludq xmm12, xmm8, xmm11
	vpmuludq ymm15, ymm10, ymm8
	vpmuludq xmm17, xmm19, xmm19
	vpmuludq ymm25, ymm20, ymm21
	vpmuludq xmm24, xmm24, xmm22
	vpmuludq ymm22, ymm25, ymm22
%endif
