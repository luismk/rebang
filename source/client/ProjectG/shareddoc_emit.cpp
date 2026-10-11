// Emit a single out-of-line copy of the SharedDoc inlines that the original
// executable provided from a single object file. Other translation units in
// the project define these macros before including the header when they need
// the inline variants; this unit defines them empty so the linker gets one
// non-inline copy of those helpers (FirstMap/LastMap/PrevMap/NextMap, etc.).

#define LOBBYMAIN_SHAREDDOC_INLINE
#define GOLFRULE_SHAREDDOC_INLINE
#define CLIENTSETTING_SHAREDDOC_INLINE
#define SHAREDDOC_INLINE

#include "shareddoc.h"
