	vpcmpgeuw k1, xmm2, oword [0xf6b]
	vpcmpgeuw k5, xmm6, xmm1
	vpcmpgeuw k2, ymm7, yword [0xa78]
	vpcmpgeuw k7, ymm1, ymm0
	vpcmpgeuw k6, zmm4, zmm3
	vpcmpgeuw k3, zmm4, zmm1

%ifdef ERROR
	vpcmpgeuw k7, xmm11, xmm10
	vpcmpgeuw k6, ymm8, ymm10
	vpcmpgeuw k6, zmm11, zmm12
	vpcmpgeuw k6, xmm30, xmm22
	vpcmpgeuw k1, ymm27, ymm31
	vpcmpgeuw k7, zmm23, zmm19
%endif
