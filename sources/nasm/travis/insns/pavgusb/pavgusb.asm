default rel
	pavgusb mm0, mm5
	pavgusb mm6, mm1
	pavgusb mm7, qword [eax+1]
	pavgusb mm6, qword [eax+64]
	pavgusb mm1, [0x4f3]
