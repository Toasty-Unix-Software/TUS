	vcmpnle_uspd xmm6, xmm4, oword [0x14a]
	vcmpnle_uspd xmm6, oword [0x14a]
	vcmpnle_uspd xmm5, xmm0, xmm0
	vcmpnle_uspd ymm5, ymm7, ymm2
	vcmpnle_uspd ymm5, ymm2
	vcmpnle_uspd ymm6, ymm3, ymm5
	vcmpnle_uspd k7, xmm3, oword [0x78d]
	vcmpnle_uspd k3, xmm1, xmm5
	vcmpnle_uspd k6, ymm4, ymm5
	vcmpnle_uspd k6, ymm0, ymm2

%ifdef ERROR
	vcmpnle_uspd xmm11, xmm13, xmm8
	vcmpnle_uspd ymm8, ymm9, ymm12
	vcmpnle_uspd k4, xmm14, xmm13
	vcmpnle_uspd k6, ymm13, ymm15
%endif
