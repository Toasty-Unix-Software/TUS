default rel
	vcmpngt_uqps xmm4, xmm1, xmm1
	vcmpngt_uqps xmm4, xmm1
	vcmpngt_uqps xmm0, xmm7, oword [0x8be]
	vcmpngt_uqps ymm0, ymm7, yword [0xb40]
	vcmpngt_uqps ymm0, yword [0xb40]
	vcmpngt_uqps ymm6, ymm2, ymm2
	vcmpngt_uqps k2, xmm7, xmm1
	vcmpngt_uqps k2, xmm4, xmm0
	vcmpngt_uqps k6, ymm0, ymm7
	vcmpngt_uqps k2, ymm7, ymm3
	vcmpngt_uqps xmm14, xmm12, xmm12
	vcmpngt_uqps ymm12, ymm11, ymm10
	vcmpngt_uqps k7, xmm12, xmm10
	vcmpngt_uqps k7, ymm14, ymm15
	vcmpngt_uqps k3{k3}, xmm0, oword [0x7d0]
	vcmpngt_uqps k7{k7}, ymm6, ymm4
	vcmpngt_uqps k6{k3}, zmm7, zword [0xf49]
	vcmpngt_uqps k4, zmm6, zmm3, {sae}
	vcmpngt_uqps xmm2, xmm2, oword [eax+1]
	vcmpngt_uqps xmm6, xmm4, oword [eax+64]
	vcmpngt_uqps ymm7, ymm7, yword [eax+1]
	vcmpngt_uqps ymm3, ymm3, yword [eax+64]
	vcmpngt_uqps k2, xmm1, oword [eax+1]
	vcmpngt_uqps k6, xmm4, oword [eax+64]
	vcmpngt_uqps k7, ymm4, yword [eax+1]
	vcmpngt_uqps k5, ymm0, yword [eax+64]
	vcmpngt_uqps k3, zmm7, zword [eax+1]
	vcmpngt_uqps k2, zmm3, zword [eax+64]
	vcmpngt_uqps xmm3, xmm0, [0x827]
	vcmpngt_uqps ymm7, ymm1, [0xc80]
	vcmpngt_uqps k2, xmm0, [0x554]
	vcmpngt_uqps k1, ymm7, [0x4f3]
	vcmpngt_uqps k4, zmm6, [0x17f]
