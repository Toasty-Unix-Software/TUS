default rel
	vcmple_oqps xmm3, xmm0, xmm2
	vcmple_oqps xmm3, xmm2
	vcmple_oqps xmm3, xmm4, oword [0x861]
	vcmple_oqps ymm0, ymm5, ymm6
	vcmple_oqps ymm0, ymm6
	vcmple_oqps ymm6, ymm7, yword [0x8a0]
	vcmple_oqps k3, xmm3, xmm1
	vcmple_oqps k7, xmm7, xmm5
	vcmple_oqps k4, ymm2, ymm5
	vcmple_oqps k6, ymm0, ymm6
	vcmple_oqps xmm8, xmm12, xmm14
	vcmple_oqps ymm9, ymm8, ymm10
	vcmple_oqps k3, xmm8, xmm10
	vcmple_oqps k3, ymm8, ymm15
	vcmple_oqps k2{k6}, xmm7, oword [0x9f9]
	vcmple_oqps k4{k3}, ymm5, ymm3
	vcmple_oqps k5{k5}, zmm7, zword [0x72e]
	vcmple_oqps k2, zmm3, zmm4, {sae}
	vcmple_oqps xmm6, xmm7, oword [eax+1]
	vcmple_oqps xmm3, xmm7, oword [eax+64]
	vcmple_oqps ymm5, ymm3, yword [eax+1]
	vcmple_oqps ymm4, ymm7, yword [eax+64]
	vcmple_oqps k1, xmm7, oword [eax+1]
	vcmple_oqps k1, xmm1, oword [eax+64]
	vcmple_oqps k1, ymm3, yword [eax+1]
	vcmple_oqps k3, ymm5, yword [eax+64]
	vcmple_oqps k3, zmm1, zword [eax+1]
	vcmple_oqps k1, zmm2, zword [eax+64]
	vcmple_oqps xmm3, xmm0, [0x64a]
	vcmple_oqps ymm4, ymm1, [0xbed]
	vcmple_oqps k1, xmm5, [0xc62]
	vcmple_oqps k7, ymm6, [0xa17]
	vcmple_oqps k3, zmm4, [0xeae]
