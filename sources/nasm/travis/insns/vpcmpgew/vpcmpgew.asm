default rel
	vpcmpgew k3, xmm5, oword [0x459]
	vpcmpgew k5, xmm3, xmm0
	vpcmpgew k2, ymm4, ymm5
	vpcmpgew k6, ymm6, yword [0x259]
	vpcmpgew k4, zmm0, zmm1
	vpcmpgew k5, zmm1, zmm5
	vpcmpgew k2, xmm13, xmm8
	vpcmpgew k4, ymm9, ymm15
	vpcmpgew k3, zmm13, zmm8
	vpcmpgew k3, xmm26, xmm22
	vpcmpgew k2, ymm24, ymm20
	vpcmpgew k7, zmm28, zmm18
	vpcmpgew k5{k7}, xmm7, oword [0x346]
	vpcmpgew k2{k2}, ymm1, ymm4
	vpcmpgew k4{k7}, zmm7, zmm1
	vpcmpgew k1, xmm2, oword [eax+1]
	vpcmpgew k4, xmm5, oword [eax+64]
	vpcmpgew k4, ymm0, yword [eax+1]
	vpcmpgew k4, ymm0, yword [eax+64]
	vpcmpgew k2, zmm3, zword [eax+1]
	vpcmpgew k3, zmm0, zword [eax+64]
	vpcmpgew k1, xmm6, [0xc88]
	vpcmpgew k4, ymm5, [0xbba]
	vpcmpgew k4, zmm7, [0x645]
