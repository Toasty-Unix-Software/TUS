	vpcmpequd k4, xmm3, xmm1
	vpcmpequd k6, xmm1, xmm5
	vpcmpequd k1, ymm7, ymm3
	vpcmpequd k4, ymm7, ymm7
	vpcmpequd k6, zmm4, zword [0xd84]
	vpcmpequd k7, zmm0, zword [0xf4b]

%ifdef ERROR
	vpcmpequd k6, xmm11, xmm8
	vpcmpequd k6, ymm12, ymm8
	vpcmpequd k3, zmm10, zmm8
	vpcmpequd k2, xmm19, xmm21
	vpcmpequd k2, ymm22, ymm30
	vpcmpequd k7, zmm28, zmm25
%endif
