default rel
	vpcmpngtuw k5, xmm4, xmm6
	vpcmpngtuw k1, xmm0, oword [0xab7]
	vpcmpngtuw k3, ymm3, ymm2
	vpcmpngtuw k7, ymm1, yword [0xb10]
	vpcmpngtuw k3, zmm0, zmm1
	vpcmpngtuw k1, zmm2, zmm6
	vpcmpngtuw k4, xmm15, xmm12
	vpcmpngtuw k1, ymm11, ymm12
	vpcmpngtuw k5, zmm10, zmm12
	vpcmpngtuw k6, xmm27, xmm23
	vpcmpngtuw k4, ymm28, ymm19
	vpcmpngtuw k7, zmm18, zmm29
	vpcmpngtuw k3{k6}, xmm0, oword [0x134]
	vpcmpngtuw k5{k2}, ymm4, ymm5
	vpcmpngtuw k5{k1}, zmm1, zmm4
	vpcmpngtuw k6, xmm7, oword [eax+1]
	vpcmpngtuw k6, xmm1, oword [eax+64]
	vpcmpngtuw k1, ymm2, yword [eax+1]
	vpcmpngtuw k7, ymm0, yword [eax+64]
	vpcmpngtuw k1, zmm0, zword [eax+1]
	vpcmpngtuw k1, zmm6, zword [eax+64]
	vpcmpngtuw k3, xmm4, [0x2b1]
	vpcmpngtuw k5, ymm7, [0x7c2]
	vpcmpngtuw k7, zmm1, [0xb77]
