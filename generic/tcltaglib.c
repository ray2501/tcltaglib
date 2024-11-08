#include <tcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ttaglib.h"

/*
 *----------------------------------------------------------------------
 *
 * Tcltaglib_Init
 *
 * Results:
 *	A standard Tcl result
 *
 * Side effects:
 *	The Tcltaglib package is created.
 *
 *----------------------------------------------------------------------
 */

int
Tcltaglib_Init(Tcl_Interp *interp)
{
    ThreadSpecificData *tsdPtr;
    
    if (Tcl_InitStubs(interp, "8.4", 0) == NULL) {
	return TCL_ERROR;
    }
    if (Tcl_PkgRequire(interp, "Tcl", "8.4", 0) == NULL) {
	return TCL_ERROR;
    }
    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
	return TCL_ERROR;
    }

    //Tcltaglib_InitHashTable();
    
    /* 
     *   Tcl_GetThreadData handles the auto-initialization of all data in 
     *  the ThreadSpecificData to NULL at first time.
     */
    Tcl_MutexLock(&myMutex);    
    tsdPtr = (ThreadSpecificData *)
        Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
        tsdPtr->initialized = 1;
        tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
        Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
          
        tsdPtr->taglib_count = 0;
        tsdPtr->tag_count = 0;
    }
    Tcl_MutexUnlock(&myMutex);    
    
    /* Add a thread exit handler to delete hash table */
    Tcl_CreateThreadExitHandler(Tcltaglib_Thread_Exit, (ClientData)NULL);     
    
    Tcl_CreateObjCommand(interp, "::taglib::file_new", TAGLIB_FILE_NEW, (ClientData) NULL, NULL);
    
    Tcl_CreateObjCommand(interp, "::taglib::file_new_type", TAGLIB_FILE_NEW_TYPE, (ClientData) NULL, NULL);
    
    Tcl_CreateObjCommand(interp, "::taglib::file_is_valid", TAGLIB_FILE_IS_VALID, (ClientData) NULL, NULL);
    
    Tcl_CreateObjCommand(interp, "::taglib::file_tag", TAGLIB_file_tag, (ClientData) NULL, NULL);
    
    Tcl_CreateObjCommand(interp, "::taglib::file_save", TAGLIB_FILE_SAVE, (ClientData) NULL, NULL);
    
    Tcl_CreateObjCommand(interp, "::taglib::file_free", TAGLIB_FILE_FREE, (ClientData) NULL, NULL);
    
    Tcl_CreateObjCommand(interp, "::taglib::audioproperties", TAGLIB_AudioProperties, (ClientData) NULL, NULL);
    
    Tcl_CreateObjCommand(interp, "::taglib::tag_free", TAGLIB_TAG_FREE, (ClientData) NULL, NULL);
    
    
    /* get tat info */    
    Tcl_CreateObjCommand(interp, "::taglib::tag_title", TAGLIB_tag_title, (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "::taglib::tag_artist", TAGLIB_tag_artist, (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "::taglib::tag_album", TAGLIB_tag_album, (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "::taglib::tag_comment", TAGLIB_tag_comment, (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "::taglib::tag_genre", TAGLIB_tag_genre, (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "::taglib::tag_year", TAGLIB_tag_year, (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "::taglib::tag_track", TAGLIB_tag_track, (ClientData) NULL, NULL);
    
    /* set tag info */
    Tcl_CreateObjCommand(interp, "::taglib::set_title", TAGLIB_set_title, (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "::taglib::set_artist", TAGLIB_set_artist, (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "::taglib::set_album", TAGLIB_set_album, (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "::taglib::set_comment", TAGLIB_set_comment, (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "::taglib::set_genre", TAGLIB_set_genre, (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "::taglib::set_year", TAGLIB_set_year, (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "::taglib::set_track", TAGLIB_set_track, (ClientData) NULL, NULL);
    
    return TCL_OK;
}
