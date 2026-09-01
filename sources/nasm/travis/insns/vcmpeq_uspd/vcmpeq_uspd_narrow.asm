	vcmpeq_uspd xmm1, xmm2, oword [0x7ad]
	vcmpeq_uspd xmm1, oword [0x7ad]
	vcmpeq_uspd xmm6, xmm6, xmm2
	vcmpeq_uspd ymm3, ymm1, ymm0
	vcmpeq_uspd ymm3, ymm0
	vcmpeq_uspd ymm6, ymm3, ymm4
	vcmpeq_uspd k7, xmm0, oword [0xb92]
	vcmpeq_uspd k4, xmm1, xmm3
	vcmpeq_uspd k6, ymm1, ymm0
	vcmpeq_uspd k4, ymm4, yword [0x86f]

%ifdef ERROR
	vcmpeq_uspd xmm10, xmm12, xmm12
	vcmpeq_uspd ymm12, ymm15, ymm11
	vcmpeq_uspd k2, xmm9, xmm12
	vcmpeq_uspd k3, ymm9, ymm8
%endif
