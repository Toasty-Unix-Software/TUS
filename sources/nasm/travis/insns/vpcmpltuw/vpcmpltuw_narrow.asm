	vpcmpltuw k4, xmm7, xmm3
	vpcmpltuw k1, xmm0, oword [0xcfd]
	vpcmpltuw k1, ymm7, yword [0x45e]
	vpcmpltuw k3, ymm2, ymm2
	vpcmpltuw k7, zmm5, zmm5
	vpcmpltuw k4, zmm5, zmm2

%ifdef ERROR
	vpcmpltuw k5, xmm15, xmm11
	vpcmpltuw k4, ymm8, ymm11
	vpcmpltuw k3, zmm9, zmm10
	vpcmpltuw k1, xmm25, xmm23
	vpcmpltuw k6, ymm31, ymm16
	vpcmpltuw k4, zmm30, zmm22
%endif
