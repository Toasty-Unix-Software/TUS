	vcmptruepd xmm0, xmm4, xmm4
	vcmptruepd xmm0, xmm4
	vcmptruepd xmm0, xmm7, oword [0x8b0]
	vcmptruepd ymm1, ymm7, ymm5
	vcmptruepd ymm1, ymm5
	vcmptruepd ymm5, ymm0, ymm0
	vcmptruepd k2, xmm3, xmm3
	vcmptruepd k6, xmm5, oword [0xce5]
	vcmptruepd k2, ymm1, yword [0x514]
	vcmptruepd k6, ymm3, ymm4

%ifdef ERROR
	vcmptruepd xmm12, xmm13, xmm14
	vcmptruepd ymm11, ymm14, ymm8
	vcmptruepd k2, xmm8, xmm15
	vcmptruepd k3, ymm15, ymm11
%endif
