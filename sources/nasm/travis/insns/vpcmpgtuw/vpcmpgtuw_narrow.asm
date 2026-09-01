	vpcmpgtuw k6, xmm0, oword [0xe75]
	vpcmpgtuw k3, xmm0, xmm3
	vpcmpgtuw k5, ymm4, yword [0xe9f]
	vpcmpgtuw k5, ymm6, ymm0
	vpcmpgtuw k3, zmm4, zmm4
	vpcmpgtuw k3, zmm6, zmm1

%ifdef ERROR
	vpcmpgtuw k7, xmm8, xmm12
	vpcmpgtuw k2, ymm15, ymm11
	vpcmpgtuw k7, zmm15, zmm10
	vpcmpgtuw k4, xmm31, xmm25
	vpcmpgtuw k3, ymm30, ymm21
	vpcmpgtuw k7, zmm25, zmm30
%endif
