default rel
	vcmple_osps xmm7, xmm2, oword [0x792]
	vcmple_osps xmm7, oword [0x792]
	vcmple_osps xmm5, xmm5, xmm6
	vcmple_osps ymm4, ymm4, yword [0x2d6]
	vcmple_osps ymm4, yword [0x2d6]
	vcmple_osps ymm6, ymm0, ymm4
	vcmple_osps k3, xmm4, xmm7
	vcmple_osps k7, xmm7, xmm4
	vcmple_osps k2, ymm7, ymm2
	vcmple_osps k5, ymm2, ymm5
	vcmple_osps xmm13, xmm15, xmm9
	vcmple_osps ymm14, ymm12, ymm12
	vcmple_osps k7, xmm8, xmm14
	vcmple_osps k1, ymm13, ymm14
	vcmple_osps k3{k7}, xmm1, xmm7
	vcmple_osps k2{k7}, ymm0, ymm2
	vcmple_osps k4{k6}, zmm5, zmm0
	vcmple_osps k4, zmm0, zmm4, {sae}
	vcmple_osps xmm1, xmm2, oword [eax+1]
	vcmple_osps xmm0, xmm7, oword [eax+64]
	vcmple_osps ymm5, ymm7, yword [eax+1]
	vcmple_osps ymm2, ymm4, yword [eax+64]
	vcmple_osps k6, xmm0, oword [eax+1]
	vcmple_osps k4, xmm2, oword [eax+64]
	vcmple_osps k7, ymm4, yword [eax+1]
	vcmple_osps k5, ymm1, yword [eax+64]
	vcmple_osps k6, zmm3, zword [eax+1]
	vcmple_osps k4, zmm6, zword [eax+64]
	vcmple_osps xmm7, xmm5, [0x52d]
	vcmple_osps ymm4, ymm6, [0xcfe]
	vcmple_osps k6, xmm7, [0x632]
	vcmple_osps k3, ymm7, [0xe60]
	vcmple_osps k5, zmm3, [0x169]
