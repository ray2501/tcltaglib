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

int TAGLIB_FILE_NEW (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_FILE_NEW_TYPE (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_FILE_IS_VALID (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_file_tag (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_FILE_SAVE (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_FILE_FREE (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);

int TAGLIB_AudioProperties (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);

int TAGLIB_TAG_FREE (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);

int TAGLIB_tag_title (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_tag_artist (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_tag_album (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_tag_comment (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_tag_genre (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_tag_year (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_tag_track (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);

int TAGLIB_set_title (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_set_artist (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_set_album (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_set_comment (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_set_genre (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_set_year (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);
int TAGLIB_set_track (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const obj[]);

//void Tcltaglib_InitHashTable ();
void Tcltaglib_Thread_Exit(ClientData clientdata);

/*
 * end block for C++
 */

#ifdef __cplusplus
}
#endif

#endif /* _TAGLIB */
