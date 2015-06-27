#include <xc.inc>

GLOBAL _arg1
GLOBAL _arg2
GLOBAL _res
GLOBAL _MULT16

PSECT my_mult,local,class=CODE,reloc=2
 _MULT16:

    MOVF _arg1+0, W
    MULWF _arg2+0

    MOVFF PRODH, _res+1
    MOVFF PRODL, _res+0

    MOVF _arg1+1, W
    MULWF _arg2+1
    MOVFF PRODH, _res+3
    MOVFF PRODL, _res+2

    MOVF _arg1+0, W
    MULWF _arg2+1
    MOVF PRODL, W
    ADDWF _res+1, F
    MOVF PRODH, W
    ADDWFC _res+2, F
    CLRF WREG ;
    ADDWFC _res+3, F
    MOVF _arg1+1, W
    MULWF _arg2+0
    MOVF PRODL, W ;
    ADDWF _res+1, F
    MOVF PRODH, W
    ADDWFC _res+2, F
    CLRF WREG
    ADDWFC _res+3, F
    RETURN


