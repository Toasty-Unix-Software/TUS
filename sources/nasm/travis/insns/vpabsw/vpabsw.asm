default rel
	vpabsw xmm1, oword [0xe00]
	vpabsw xmm3, xmm3
	vpabsw ymm7, ymm5
	vpabsw ymm0, yword [0xddf]
	vpabsw xmm7, xmm4
	vpabsw xmm5, oword [0x45f]
	vpabsw ymm3, yword [0xa3c]
	vpabsw ymm7, ymm5
	vpabsw xmm9, xmm8
	vpabsw ymm12, ymm10
	vpabsw xmm12, xmm8
	vpabsw ymm8, ymm15
	vpabsw xmm20, xmm28
	vpabsw ymm24, ymm24
	vpabsw xmm24, xmm28
	vpabsw ymm31, ymm29
	vpabsw xmm0{k3}, xmm6
	vpabsw ymm4{k6}, ymm7
	vpabsw zmm7{k3}, zword [0x848]
	vpabsw xmm1{k5}{z}, xmm0
	vpabsw ymm7{k6}{z}, ymm6
	vpabsw zmm3{k5}{z}, zmm4
	vpabsw xmm2, oword [eax+1]
	vpabsw xmm6, oword [eax+64]
	vpabsw ymm5, yword [eax+1]
	vpabsw ymm1, yword [eax+64]
	vpabsw xmm2, oword [eax+1]
	vpabsw xmm1, oword [eax+64]
	vpabsw ymm1, yword [eax+1]
	vpabsw ymm3, yword [eax+64]
	vpabsw zmm2, zword [eax+1]
	vpabsw zmm4, zword [eax+64]
	vpabsw xmm4, [0x470]
	vpabsw ymm0, [0x13a]
	vpabsw xmm5, [0xa6e]
	vpabsw ymm6, [0x4ac]
	vpabsw zmm3, [0x230]
