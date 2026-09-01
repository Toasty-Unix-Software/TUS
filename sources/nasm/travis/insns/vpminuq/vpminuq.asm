default rel
	vpminuq xmm2, xmm2, oword [0x19a]
	vpminuq xmm2, oword [0x19a]
	vpminuq xmm6, xmm5, xmm7
	vpminuq ymm0, ymm5, ymm3
	vpminuq ymm0, ymm3
	vpminuq ymm1, ymm0, ymm1
	vpminuq zmm5, zmm1, zmm5
	vpminuq zmm5, zmm5
	vpminuq zmm7, zmm5, zmm1
	vpminuq xmm10, xmm13, xmm8
	vpminuq ymm9, ymm10, ymm11
	vpminuq zmm9, zmm14, zmm8
	vpminuq xmm21, xmm28, xmm29
	vpminuq ymm22, ymm31, ymm23
	vpminuq zmm17, zmm29, zmm25
	vpminuq xmm0{k2}, xmm5, xmm6
	vpminuq ymm3{k7}, ymm3, ymm3
	vpminuq zmm6{k4}, zmm0, zword [0xab1]
	vpminuq xmm3{k4}{z}, xmm1, oword [0xaf1]
	vpminuq ymm6{k3}{z}, ymm1, ymm5
	vpminuq zmm7{k6}{z}, zmm1, zmm0
	vpminuq xmm0, xmm7, oword [eax+1]
	vpminuq xmm3, xmm1, oword [eax+64]
	vpminuq ymm4, ymm2, yword [eax+1]
	vpminuq ymm5, ymm0, yword [eax+64]
	vpminuq zmm0, zmm0, zword [eax+1]
	vpminuq zmm4, zmm1, zword [eax+64]
	vpminuq xmm7, xmm7, [0x942]
	vpminuq ymm5, ymm3, [0x693]
	vpminuq zmm5, zmm0, [0x615]
