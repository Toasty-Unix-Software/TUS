	vpmovsxbw xmm2, qword [0x414]
	vpmovsxbw xmm0, xmm2
	vpmovsxbw ymm6, oword [0xc5d]
	vpmovsxbw ymm2, oword [0xff9]
	vpmovsxbw xmm4, xmm6
	vpmovsxbw xmm7, qword [0xeb1]
	vpmovsxbw ymm3, xmm0
	vpmovsxbw ymm0, xmm7

%ifdef ERROR
	vpmovsxbw xmm14, xmm8
	vpmovsxbw ymm14, xmm14
	vpmovsxbw xmm9, xmm13
	vpmovsxbw ymm10, xmm10
	vpmovsxbw xmm23, xmm20
	vpmovsxbw ymm18, xmm21
	vpmovsxbw xmm21, xmm19
	vpmovsxbw ymm21, xmm23
%endif
