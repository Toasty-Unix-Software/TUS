	vpmovsqw xmm1, xmm4
	vpmovsqw xmm2, xmm4
	vpmovsqw xmm2, ymm7
	vpmovsqw xmm3, ymm0
	vpmovsqw xmm2, zmm4
	vpmovsqw xmm4, zmm4
	vpmovsqw dword [0x4cb], xmm7
	vpmovsqw dword [0xce9], xmm6

%ifdef ERROR
	vpmovsqw xmm15, xmm14
	vpmovsqw xmm13, ymm10
	vpmovsqw xmm8, zmm10
	vpmovsqw dword [0x64f], xmm15
	vpmovsqw xmm20, xmm25
	vpmovsqw xmm30, ymm22
	vpmovsqw xmm25, zmm28
	vpmovsqw dword [0x180], xmm30
%endif
