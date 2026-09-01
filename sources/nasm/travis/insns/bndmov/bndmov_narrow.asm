	bndmov bnd1, bnd1
	bndmov bnd2, bnd1
	bndmov bnd0, [0xaa6]
	bndmov bnd1, [0x91f]
	bndmov [0x198], bnd2
	bndmov [0x63d], bnd0
