	umwait esi
	umwait esi
	umwait edi, edx, eax
	umwait ecx, edx, eax

%ifdef ERROR
	umwait r15d
	umwait r11d, edx, eax
	umwait r28d
	umwait r16d, edx, eax
%endif
