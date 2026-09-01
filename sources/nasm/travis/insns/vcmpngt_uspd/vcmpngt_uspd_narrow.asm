	vcmpngt_uspd xmm3, xmm4, oword [0xf28]
	vcmpngt_uspd xmm3, oword [0xf28]
	vcmpngt_uspd xmm0, xmm4, xmm2
	vcmpngt_uspd ymm2, ymm4, ymm5
	vcmpngt_uspd ymm2, ymm5
	vcmpngt_uspd ymm4, ymm5, yword [0xc0b]
	vcmpngt_uspd k5, xmm2, xmm5
	vcmpngt_uspd k5, xmm1, xmm0
	vcmpngt_uspd k4, ymm5, ymm3
	vcmpngt_uspd k1, ymm1, yword [0xad9]

%ifdef ERROR
	vcmpngt_uspd xmm12, xmm13, xmm13
	vcmpngt_uspd ymm8, ymm11, ymm10
	vcmpngt_uspd k5, xmm12, xmm11
	vcmpngt_uspd k1, ymm9, ymm8
%endif
