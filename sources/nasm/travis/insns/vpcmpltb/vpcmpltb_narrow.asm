	vpcmpltb k4, xmm4, xmm2
	vpcmpltb k7, xmm3, xmm4
	vpcmpltb k3, ymm6, ymm5
	vpcmpltb k2, ymm4, ymm0
	vpcmpltb k3, zmm0, zmm3
	vpcmpltb k5, zmm2, zword [0xa52]

%ifdef ERROR
	vpcmpltb k1, xmm14, xmm8
	vpcmpltb k6, ymm13, ymm10
	vpcmpltb k4, zmm13, zmm10
	vpcmpltb k2, xmm27, xmm21
	vpcmpltb k4, ymm29, ymm25
	vpcmpltb k3, zmm22, zmm31
%endif
