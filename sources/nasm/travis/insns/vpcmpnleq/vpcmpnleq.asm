default rel
	vpcmpnleq k4, xmm4, oword [0xddf]
	vpcmpnleq k4, xmm3, oword [0xb94]
	vpcmpnleq k1, ymm4, ymm0
	vpcmpnleq k3, ymm5, ymm6
	vpcmpnleq k2, zmm6, zword [0x525]
	vpcmpnleq k4, zmm2, zword [0x16c]
	vpcmpnleq k6, xmm12, xmm10
	vpcmpnleq k2, ymm9, ymm15
	vpcmpnleq k7, zmm14, zmm15
	vpcmpnleq k1, xmm27, xmm30
	vpcmpnleq k6, ymm18, ymm27
	vpcmpnleq k3, zmm19, zmm16
	vpcmpnleq k7{k7}, xmm5, xmm3
	vpcmpnleq k5{k4}, ymm6, ymm4
	vpcmpnleq k4{k7}, zmm4, zmm6
	vpcmpnleq k5, xmm0, oword [eax+1]
	vpcmpnleq k3, xmm7, oword [eax+64]
	vpcmpnleq k7, ymm1, yword [eax+1]
	vpcmpnleq k5, ymm5, yword [eax+64]
	vpcmpnleq k2, zmm2, zword [eax+1]
	vpcmpnleq k6, zmm6, zword [eax+64]
	vpcmpnleq k1, xmm3, [0x640]
	vpcmpnleq k4, ymm2, [0xb47]
	vpcmpnleq k6, zmm6, [0xe77]
