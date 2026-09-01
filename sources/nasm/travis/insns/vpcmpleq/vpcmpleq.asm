default rel
	vpcmpleq k1, xmm0, xmm5
	vpcmpleq k6, xmm1, xmm7
	vpcmpleq k5, ymm2, yword [0xa41]
	vpcmpleq k2, ymm4, yword [0xe4c]
	vpcmpleq k4, zmm2, zmm7
	vpcmpleq k1, zmm4, zmm2
	vpcmpleq k4, xmm12, xmm14
	vpcmpleq k1, ymm10, ymm14
	vpcmpleq k1, zmm12, zmm12
	vpcmpleq k6, xmm31, xmm24
	vpcmpleq k6, ymm17, ymm19
	vpcmpleq k7, zmm21, zmm29
	vpcmpleq k1{k1}, xmm0, xmm3
	vpcmpleq k7{k5}, ymm5, yword [0x54d]
	vpcmpleq k6{k7}, zmm7, zword [0x137]
	vpcmpleq k3, xmm3, oword [eax+1]
	vpcmpleq k4, xmm1, oword [eax+64]
	vpcmpleq k6, ymm6, yword [eax+1]
	vpcmpleq k5, ymm2, yword [eax+64]
	vpcmpleq k7, zmm7, zword [eax+1]
	vpcmpleq k4, zmm2, zword [eax+64]
	vpcmpleq k6, xmm1, [0x902]
	vpcmpleq k6, ymm5, [0xfdb]
	vpcmpleq k2, zmm6, [0xe21]
