default rel
	bndmov bnd1, bnd1
	bndmov bnd2, bnd1
	bndmov bnd0, [0xaa6]
	bndmov bnd1, [0x91f]
	bndmov [0x198], bnd2
	bndmov [0x63d], bnd0
	bndmov bnd1, [eax+1]
	bndmov bnd0, [eax+64]
	bndmov [eax+1], bnd3
	bndmov [eax+64], bnd0
