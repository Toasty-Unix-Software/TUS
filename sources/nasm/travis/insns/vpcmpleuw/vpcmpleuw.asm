default rel
	vpcmpleuw k4, xmm7, oword [0x150]
	vpcmpleuw k5, xmm2, xmm0
	vpcmpleuw k6, ymm3, yword [0x1ef]
	vpcmpleuw k1, ymm1, yword [0x588]
	vpcmpleuw k6, zmm2, zmm4
	vpcmpleuw k2, zmm0, zword [0xc64]
	vpcmpleuw k2, xmm13, xmm12
	vpcmpleuw k2, ymm11, ymm15
	vpcmpleuw k7, zmm15, zmm12
	vpcmpleuw k4, xmm28, xmm23
	vpcmpleuw k4, ymm22, ymm24
	vpcmpleuw k2, zmm22, zmm29
	vpcmpleuw k3{k6}, xmm0, xmm3
	vpcmpleuw k5{k1}, ymm5, yword [0xdc8]
	vpcmpleuw k7{k5}, zmm4, zmm5
	vpcmpleuw k7, xmm5, oword [eax+1]
	vpcmpleuw k2, xmm7, oword [eax+64]
	vpcmpleuw k4, ymm3, yword [eax+1]
	vpcmpleuw k7, ymm2, yword [eax+64]
	vpcmpleuw k7, zmm1, zword [eax+1]
	vpcmpleuw k7, zmm7, zword [eax+64]
	vpcmpleuw k1, xmm3, [0x8db]
	vpcmpleuw k4, ymm3, [0x8a3]
	vpcmpleuw k2, zmm0, [0x5db]
