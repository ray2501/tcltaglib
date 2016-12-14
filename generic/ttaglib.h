#ifndef _TAGLIB
#define _TAGLIB

#include <tcl.h>

/*
 * Windows needs to know which symbols to export.
 */

#ifdef BUILD_tcltaglib
#undef TCL_STORAGE_CLASS
#define TCL_STORAGE_CLASS DLLEXPORT
#endif /* BUILD_tcltaglib */


/*
 * For C++ compilers, use extern "C"
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ThreadSpecificData {
  int initialized;                /* initialization flag */
  Tcl_HashTable *taglib_hashtblPtr; /* per thread hash table. */
  int taglib_count;
  int tag_count;
} ThreadSpecificData;

static Tcl_ThreadDataKey dataKey;    

TCL_DECLARE_MUTEX(myMutex);    
    

/*
 * Only the _Init function is exported.
 */

EXTERN int	Tcltaglib_Init(Tcl_Interp * interp);

int TAGLIB_FILE_NEW _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_FILE_NEW_TYPE _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_FILE_IS_VALID _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_file_tag _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_FILE_SAVE _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_FILE_FREE _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));

int TAGLIB_AudioProperties _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));

int TAGLIB_TAG_FREE _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));

int TAGLIB_tag_title _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_tag_artist _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_tag_album _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_tag_comment _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_tag_genre _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_tag_year _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_tag_track _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));

int TAGLIB_set_title _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_set_artist _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_set_album _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_set_comment _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_set_genre _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_set_year _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));
int TAGLIB_set_track _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[]));

//void Tcltaglib_InitHashTable _ANSI_ARGS_(());
void Tcltaglib_Thread_Exit(ClientData clientdata);

/*
 * end block for C++
 */

#ifdef __cplusplus
}
#endif

#endif /* _TAGLIB */
