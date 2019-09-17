GLOBAL _int80

GLOBAL _sti
GLOBAL _hlt
GLOBAL _cli

section .text

_int80:
    int 80h
    ret

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret

_sti:
	sti
	ret
