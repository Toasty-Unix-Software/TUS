	vcmptrue_uspd xmm7, xmm1, xmm2
	vcmptrue_uspd xmm7, xmm2
	vcmptrue_uspd xmm7, xmm0, xmm5
	vcmptrue_uspd ymm4, ymm7, yword [0xa12]
	vcmptrue_uspd ymm4, yword [0xa12]
	vcmptrue_uspd ymm7, ymm0, yword [0xb13]
	vcmptrue_uspd k6, xmm1, oword [0x467]
	vcmptrue_uspd k6, xmm2, xmm7
	vcmptrue_uspd k4, ymm2, yword [0x655]
	vcmptrue_uspd k5, ymm3, yword [0xbfd]

%ifdef ERROR
	vcmptrue_uspd xmm9, xmm14, xmm10
	vcmptrue_uspd ymm9, ymm13, ymm15
	vcmptrue_uspd k7, xmm12, xmm11
	vcmptrue_uspd k1, ymm12, ymm12
%endif
