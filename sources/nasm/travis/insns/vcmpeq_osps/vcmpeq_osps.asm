default rel
	vcmpeq_osps xmm1, xmm2, xmm6
	vcmpeq_osps xmm1, xmm6
	vcmpeq_osps xmm4, xmm4, xmm4
	vcmpeq_osps ymm5, ymm4, ymm1
	vcmpeq_osps ymm5, ymm1
	vcmpeq_osps ymm3, ymm2, yword [0xdc7]
	vcmpeq_osps k7, xmm3, xmm5
	vcmpeq_osps k5, xmm7, xmm2
	vcmpeq_osps k7, ymm1, ymm4
	vcmpeq_osps k4, ymm1, ymm2
	vcmpeq_osps xmm12, xmm13, xmm13
	vcmpeq_osps ymm9, ymm8, ymm12
	vcmpeq_osps k1, xmm11, xmm13
	vcmpeq_osps k5, ymm14, ymm14
	vcmpeq_osps k7{k6}, xmm7, oword [0x3da]
	vcmpeq_osps k5{k4}, ymm1, ymm5
	vcmpeq_osps k1{k1}, zmm1, zmm3
	vcmpeq_osps k6, zmm0, zmm5, {sae}
	vcmpeq_osps xmm5, xmm1, oword [eax+1]
	vcmpeq_osps xmm3, xmm2, oword [eax+64]
	vcmpeq_osps ymm1, ymm2, yword [eax+1]
	vcmpeq_osps ymm5, ymm3, yword [eax+64]
	vcmpeq_osps k7, xmm1, oword [eax+1]
	vcmpeq_osps k6, xmm7, oword [eax+64]
	vcmpeq_osps k7, ymm0, yword [eax+1]
	vcmpeq_osps k1, ymm5, yword [eax+64]
	vcmpeq_osps k1, zmm5, zword [eax+1]
	vcmpeq_osps k7, zmm6, zword [eax+64]
	vcmpeq_osps xmm7, xmm2, [0xafa]
	vcmpeq_osps ymm4, ymm4, [0xbe6]
	vcmpeq_osps k5, xmm4, [0x696]
	vcmpeq_osps k7, ymm4, [0x7e0]
	vcmpeq_osps k5, zmm3, [0xa0b]
