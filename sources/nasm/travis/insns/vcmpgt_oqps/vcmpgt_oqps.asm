default rel
	vcmpgt_oqps xmm4, xmm1, oword [0x71c]
	vcmpgt_oqps xmm4, oword [0x71c]
	vcmpgt_oqps xmm4, xmm5, xmm4
	vcmpgt_oqps ymm1, ymm5, ymm5
	vcmpgt_oqps ymm1, ymm5
	vcmpgt_oqps ymm2, ymm0, yword [0x588]
	vcmpgt_oqps k2, xmm2, xmm7
	vcmpgt_oqps k2, xmm7, oword [0x40d]
	vcmpgt_oqps k4, ymm6, ymm2
	vcmpgt_oqps k1, ymm3, yword [0x231]
	vcmpgt_oqps xmm12, xmm9, xmm9
	vcmpgt_oqps ymm12, ymm8, ymm8
	vcmpgt_oqps k5, xmm9, xmm10
	vcmpgt_oqps k2, ymm12, ymm14
	vcmpgt_oqps k3{k7}, xmm2, xmm2
	vcmpgt_oqps k6{k7}, ymm2, ymm0
	vcmpgt_oqps k3{k1}, zmm4, zmm2
	vcmpgt_oqps k1, zmm7, zmm5, {sae}
	vcmpgt_oqps xmm5, xmm3, oword [eax+1]
	vcmpgt_oqps xmm5, xmm6, oword [eax+64]
	vcmpgt_oqps ymm0, ymm2, yword [eax+1]
	vcmpgt_oqps ymm4, ymm0, yword [eax+64]
	vcmpgt_oqps k7, xmm6, oword [eax+1]
	vcmpgt_oqps k3, xmm3, oword [eax+64]
	vcmpgt_oqps k5, ymm3, yword [eax+1]
	vcmpgt_oqps k4, ymm3, yword [eax+64]
	vcmpgt_oqps k1, zmm3, zword [eax+1]
	vcmpgt_oqps k1, zmm1, zword [eax+64]
	vcmpgt_oqps xmm0, xmm1, [0x514]
	vcmpgt_oqps ymm6, ymm2, [0xbd7]
	vcmpgt_oqps k1, xmm0, [0xc65]
	vcmpgt_oqps k6, ymm1, [0x5cc]
	vcmpgt_oqps k5, zmm1, [0xd57]
