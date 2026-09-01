default rel
	vcmplt_oqps xmm1, xmm2, oword [0x103]
	vcmplt_oqps xmm1, oword [0x103]
	vcmplt_oqps xmm4, xmm6, oword [0xe41]
	vcmplt_oqps ymm6, ymm5, ymm2
	vcmplt_oqps ymm6, ymm2
	vcmplt_oqps ymm4, ymm2, ymm7
	vcmplt_oqps k2, xmm6, oword [0x8f7]
	vcmplt_oqps k2, xmm3, xmm7
	vcmplt_oqps k5, ymm4, ymm3
	vcmplt_oqps k4, ymm4, yword [0xd19]
	vcmplt_oqps xmm12, xmm9, xmm14
	vcmplt_oqps ymm14, ymm13, ymm14
	vcmplt_oqps k5, xmm14, xmm12
	vcmplt_oqps k1, ymm10, ymm11
	vcmplt_oqps k1{k7}, xmm6, xmm7
	vcmplt_oqps k1{k7}, ymm1, ymm2
	vcmplt_oqps k1{k7}, zmm6, zword [0x4c2]
	vcmplt_oqps k4, zmm7, zmm7, {sae}
	vcmplt_oqps xmm6, xmm2, oword [eax+1]
	vcmplt_oqps xmm5, xmm3, oword [eax+64]
	vcmplt_oqps ymm5, ymm3, yword [eax+1]
	vcmplt_oqps ymm7, ymm2, yword [eax+64]
	vcmplt_oqps k7, xmm0, oword [eax+1]
	vcmplt_oqps k7, xmm7, oword [eax+64]
	vcmplt_oqps k6, ymm0, yword [eax+1]
	vcmplt_oqps k5, ymm4, yword [eax+64]
	vcmplt_oqps k1, zmm3, zword [eax+1]
	vcmplt_oqps k4, zmm5, zword [eax+64]
	vcmplt_oqps xmm7, xmm2, [0xc1f]
	vcmplt_oqps ymm2, ymm2, [0x44e]
	vcmplt_oqps k3, xmm5, [0x649]
	vcmplt_oqps k1, ymm0, [0x76f]
	vcmplt_oqps k7, zmm2, [0x3ee]
