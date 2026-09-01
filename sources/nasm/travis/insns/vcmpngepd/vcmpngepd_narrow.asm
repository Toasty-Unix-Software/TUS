	vcmpngepd xmm6, xmm7, oword [0x707]
	vcmpngepd xmm6, oword [0x707]
	vcmpngepd xmm5, xmm3, xmm6
	vcmpngepd ymm2, ymm3, ymm5
	vcmpngepd ymm2, ymm5
	vcmpngepd ymm1, ymm2, ymm2
	vcmpngepd k3, xmm1, oword [0x1ab]
	vcmpngepd k1, xmm2, xmm0
	vcmpngepd k2, ymm1, ymm4
	vcmpngepd k3, ymm4, ymm2

%ifdef ERROR
	vcmpngepd xmm12, xmm12, xmm11
	vcmpngepd ymm12, ymm10, ymm12
	vcmpngepd k6, xmm14, xmm15
	vcmpngepd k1, ymm10, ymm8
%endif
