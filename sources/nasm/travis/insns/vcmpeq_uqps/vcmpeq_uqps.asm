default rel
	vcmpeq_uqps xmm1, xmm0, oword [0x36e]
	vcmpeq_uqps xmm1, oword [0x36e]
	vcmpeq_uqps xmm0, xmm7, xmm0
	vcmpeq_uqps ymm2, ymm3, ymm3
	vcmpeq_uqps ymm2, ymm3
	vcmpeq_uqps ymm0, ymm0, ymm6
	vcmpeq_uqps k1, xmm3, oword [0x655]
	vcmpeq_uqps k4, xmm6, oword [0xfae]
	vcmpeq_uqps k1, ymm0, ymm6
	vcmpeq_uqps k1, ymm0, yword [0xb80]
	vcmpeq_uqps xmm12, xmm14, xmm14
	vcmpeq_uqps ymm12, ymm9, ymm12
	vcmpeq_uqps k3, xmm14, xmm8
	vcmpeq_uqps k7, ymm14, ymm12
	vcmpeq_uqps k6{k3}, xmm4, oword [0x3b4]
	vcmpeq_uqps k4{k3}, ymm2, ymm7
	vcmpeq_uqps k4{k1}, zmm2, zword [0xf33]
	vcmpeq_uqps k3, zmm0, zmm3, {sae}
	vcmpeq_uqps xmm6, xmm3, oword [eax+1]
	vcmpeq_uqps xmm2, xmm5, oword [eax+64]
	vcmpeq_uqps ymm3, ymm1, yword [eax+1]
	vcmpeq_uqps ymm5, ymm2, yword [eax+64]
	vcmpeq_uqps k1, xmm5, oword [eax+1]
	vcmpeq_uqps k5, xmm3, oword [eax+64]
	vcmpeq_uqps k3, ymm5, yword [eax+1]
	vcmpeq_uqps k4, ymm3, yword [eax+64]
	vcmpeq_uqps k2, zmm7, zword [eax+1]
	vcmpeq_uqps k6, zmm7, zword [eax+64]
	vcmpeq_uqps xmm7, xmm1, [0xd42]
	vcmpeq_uqps ymm4, ymm1, [0x24b]
	vcmpeq_uqps k3, xmm2, [0x263]
	vcmpeq_uqps k6, ymm1, [0x24f]
	vcmpeq_uqps k2, zmm5, [0xf70]
