	vpcmpngtuw k5, xmm4, xmm6
	vpcmpngtuw k1, xmm0, oword [0xab7]
	vpcmpngtuw k3, ymm3, ymm2
	vpcmpngtuw k7, ymm1, yword [0xb10]
	vpcmpngtuw k3, zmm0, zmm1
	vpcmpngtuw k1, zmm2, zmm6

%ifdef ERROR
	vpcmpngtuw k4, xmm15, xmm12
	vpcmpngtuw k1, ymm11, ymm12
	vpcmpngtuw k5, zmm10, zmm12
	vpcmpngtuw k6, xmm27, xmm23
	vpcmpngtuw k4, ymm28, ymm19
	vpcmpngtuw k7, zmm18, zmm29
%endif
