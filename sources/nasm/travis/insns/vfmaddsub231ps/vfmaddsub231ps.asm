default rel
	vfmaddsub231ps xmm6, xmm2, xmm5
	vfmaddsub231ps xmm0, xmm3, oword [0x320]
	vfmaddsub231ps ymm5, ymm7, yword [0xf9a]
	vfmaddsub231ps ymm4, ymm5, ymm6
	vfmaddsub231ps xmm7, xmm4, xmm1
	vfmaddsub231ps xmm7, xmm4, xmm3
	vfmaddsub231ps ymm1, ymm4, ymm5
	vfmaddsub231ps ymm0, ymm6, ymm3
	vfmaddsub231ps xmm14, xmm14, xmm8
	vfmaddsub231ps ymm11, ymm10, ymm11
	vfmaddsub231ps xmm13, xmm10, xmm8
	vfmaddsub231ps ymm14, ymm11, ymm9
	vfmaddsub231ps xmm21, xmm22, xmm25
	vfmaddsub231ps ymm17, ymm16, ymm23
	vfmaddsub231ps xmm28, xmm24, xmm23
	vfmaddsub231ps ymm29, ymm25, ymm25
	vfmaddsub231ps xmm5{k2}, xmm4, oword [0xf35]
	vfmaddsub231ps ymm0{k6}, ymm3, yword [0xb99]
	vfmaddsub231ps zmm0{k7}, zmm0, zmm1
	vfmaddsub231ps xmm5{k4}{z}, xmm0, oword [0xa75]
	vfmaddsub231ps ymm0{k1}{z}, ymm6, ymm2
	vfmaddsub231ps zmm5{k3}{z}, zmm0, zmm5
	vfmaddsub231ps zmm2, zmm5, zmm7, {rn-sae}
	vfmaddsub231ps xmm1, xmm0, oword [eax+1]
	vfmaddsub231ps xmm4, xmm2, oword [eax+64]
	vfmaddsub231ps ymm6, ymm6, yword [eax+1]
	vfmaddsub231ps ymm7, ymm5, yword [eax+64]
	vfmaddsub231ps xmm7, xmm6, oword [eax+1]
	vfmaddsub231ps xmm7, xmm2, oword [eax+64]
	vfmaddsub231ps ymm0, ymm3, yword [eax+1]
	vfmaddsub231ps ymm2, ymm4, yword [eax+64]
	vfmaddsub231ps zmm1, zmm4, zword [eax+1]
	vfmaddsub231ps zmm3, zmm5, zword [eax+64]
	vfmaddsub231ps xmm3, xmm0, [0x6a2]
	vfmaddsub231ps ymm5, ymm0, [0xb6b]
	vfmaddsub231ps xmm5, xmm6, [0xabb]
	vfmaddsub231ps ymm2, ymm0, [0x110]
	vfmaddsub231ps zmm7, zmm1, [0x949]
