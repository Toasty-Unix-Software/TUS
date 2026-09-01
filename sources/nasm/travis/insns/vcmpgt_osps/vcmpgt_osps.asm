default rel
	vcmpgt_osps xmm3, xmm1, xmm0
	vcmpgt_osps xmm3, xmm0
	vcmpgt_osps xmm1, xmm4, xmm7
	vcmpgt_osps ymm6, ymm4, yword [0x70a]
	vcmpgt_osps ymm6, yword [0x70a]
	vcmpgt_osps ymm1, ymm0, ymm3
	vcmpgt_osps k1, xmm2, xmm1
	vcmpgt_osps k5, xmm2, xmm4
	vcmpgt_osps k3, ymm4, yword [0xe00]
	vcmpgt_osps k1, ymm6, ymm1
	vcmpgt_osps xmm8, xmm13, xmm8
	vcmpgt_osps ymm8, ymm8, ymm11
	vcmpgt_osps k6, xmm8, xmm9
	vcmpgt_osps k5, ymm15, ymm15
	vcmpgt_osps k1{k2}, xmm6, xmm5
	vcmpgt_osps k5{k2}, ymm7, ymm1
	vcmpgt_osps k6{k4}, zmm0, zmm7
	vcmpgt_osps k6, zmm2, zmm1, {sae}
	vcmpgt_osps xmm0, xmm0, oword [eax+1]
	vcmpgt_osps xmm6, xmm0, oword [eax+64]
	vcmpgt_osps ymm5, ymm7, yword [eax+1]
	vcmpgt_osps ymm2, ymm3, yword [eax+64]
	vcmpgt_osps k4, xmm3, oword [eax+1]
	vcmpgt_osps k4, xmm2, oword [eax+64]
	vcmpgt_osps k5, ymm6, yword [eax+1]
	vcmpgt_osps k3, ymm7, yword [eax+64]
	vcmpgt_osps k2, zmm5, zword [eax+1]
	vcmpgt_osps k4, zmm2, zword [eax+64]
	vcmpgt_osps xmm1, xmm5, [0x4f3]
	vcmpgt_osps ymm0, ymm4, [0xd13]
	vcmpgt_osps k5, xmm6, [0x22f]
	vcmpgt_osps k3, ymm7, [0x24d]
	vcmpgt_osps k6, zmm5, [0x794]
