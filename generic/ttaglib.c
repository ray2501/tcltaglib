#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ttaglib.h"
#include "taglib/tag_c.h"


void Tcltaglib_Thread_Exit(ClientData clientdata)
{
  ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

  if(tsdPtr->taglib_hashtblPtr) {
    Tcl_DeleteHashTable(tsdPtr->taglib_hashtblPtr);
    ckfree(tsdPtr->taglib_hashtblPtr);
  }
}


int TAGLIB_FILE_NEW (ClientData clientData, Tcl_Interp *interp, 
                     int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    Tcl_Obj *returnvalue;
    char handleName[16 + TCL_INTEGER_SPACE];
    char *filename;
    int len = 0;
    int newvalue;
    Tcl_DString translatedFilename;

    TagLib_File *file;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(obj[1], &len);
    if(!filename || len < 1) {
        return TCL_ERROR;
    }

    filename = Tcl_TranslateFileName(interp, filename, &translatedFilename);
    file = taglib_file_new(filename);
    Tcl_DStringFree(&translatedFilename);
    if(file == NULL)
        return TCL_ERROR;

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "taglib%d", tsdPtr->taglib_count++ );
    returnvalue = Tcl_NewStringObj( handleName, -1 );

    hashEntryPtr = Tcl_CreateHashEntry(tsdPtr->taglib_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(hashEntryPtr, file);
    Tcl_MutexUnlock(&myMutex);

    Tcl_SetObjResult(interp, returnvalue);

    return TCL_OK;
}


int TAGLIB_FILE_NEW_TYPE (ClientData clientData, Tcl_Interp *interp, 
                          int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    Tcl_Obj *returnvalue;
    char handleName[16 + TCL_INTEGER_SPACE];
    char *filename;
    int len = 0;
    int newvalue;
    Tcl_DString translatedFilename;

    TagLib_File *file;
    int type;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 3)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "filename type");
        return TCL_ERROR;
    }

    /*
     * typedef enum {
     * TagLib_File_MPEG,
     * TagLib_File_OggVorbis,
     * TagLib_File_FLAC,
     * TagLib_File_MPC,
     * TagLib_File_OggFlac,
     * TagLib_File_WavPack,
     * TagLib_File_Speex,
     * TagLib_File_TrueAudio,
     * TagLib_File_MP4,
     * TagLib_File_ASF
     * } TagLib_File_Type;
     */

    filename = Tcl_GetStringFromObj(obj[1], &len);
    if(!filename || len < 1)
        return TCL_ERROR;

    filename = Tcl_TranslateFileName(interp, filename, &translatedFilename);

    if(Tcl_GetIntFromObj(interp, obj[2], &type) != TCL_OK) {
         return TCL_ERROR;
    }

    file = taglib_file_new_type (filename, type);
    Tcl_DStringFree(&translatedFilename);

    if(file == NULL)
        return TCL_ERROR;

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "taglib%d", tsdPtr->taglib_count++ );
    returnvalue = Tcl_NewStringObj( handleName, -1 );

    hashEntryPtr = Tcl_CreateHashEntry(tsdPtr->taglib_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(hashEntryPtr, file);
    Tcl_MutexUnlock(&myMutex);

    Tcl_SetObjResult(interp, returnvalue);
    return TCL_OK;
}


int TAGLIB_FILE_IS_VALID (ClientData clientData, Tcl_Interp *interp,
                          int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    Tcl_Obj *returnvalue;
    char *handle = NULL;
    int len = 0;

    TagLib_File *file;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "taglibHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    }
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );

    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "invalid taglib handle ", handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    file = Tcl_GetHashValue( hashEntryPtr );
    if(!taglib_file_is_valid(file)) {
        returnvalue = Tcl_NewBooleanObj( 0 );
    } else {
        returnvalue = Tcl_NewBooleanObj( 1 );
    }

    Tcl_SetObjResult(interp, returnvalue);
    return TCL_OK;
}


int TAGLIB_FILE_SAVE (ClientData clientData, Tcl_Interp *interp, 
                      int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    char *handle = NULL;
    int len = 0;

    TagLib_File *file;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "taglibHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
        return TCL_ERROR;
    }
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );

    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "invalid taglib handle ", handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    file = Tcl_GetHashValue( hashEntryPtr );
    if(!taglib_file_save(file)) {
        return TCL_ERROR;
    }

    return TCL_OK;
}


int TAGLIB_FILE_FREE (ClientData clientData, Tcl_Interp *interp, 
                      int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    char *handle = NULL;
    int len = 0;

    TagLib_File *file;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "taglibHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
        return TCL_ERROR;
    }
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );

    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid taglib handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    file = Tcl_GetHashValue( hashEntryPtr );
    taglib_file_free(file);

    // Remove from hash table
    Tcl_MutexLock(&myMutex);
    Tcl_DeleteHashEntry(hashEntryPtr);
    Tcl_MutexUnlock(&myMutex);    

    return TCL_OK;
}


int TAGLIB_TAG_FREE (ClientData clientData, Tcl_Interp *interp, 
                     int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    char *handle = NULL;
    int len = 0;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
        return TCL_ERROR;
    }
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );

    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    // Remove from hash table
    Tcl_MutexLock(&myMutex);
    Tcl_DeleteHashEntry(hashEntryPtr);
    Tcl_MutexUnlock(&myMutex);

    //Now free tag strings
    taglib_tag_free_strings();

    return TCL_OK;
}


int TAGLIB_AudioProperties (ClientData clientData, Tcl_Interp *interp,
                            int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    Tcl_Obj *listobj = NULL;
    Tcl_Obj *t_length;
    Tcl_Obj *t_bitrate;
    Tcl_Obj *t_samplerate;
    Tcl_Obj *t_channels;
    int length;
    int bitrate;
    int samplerate;
    int channels;
    char *handle = NULL;
    int len = 0;

    TagLib_File *file;
    const TagLib_AudioProperties *properties;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "taglibHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    }
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );

    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid taglib handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }


    file = Tcl_GetHashValue( hashEntryPtr );
    properties = taglib_file_audioproperties(file);
    if(properties == NULL) {
        return TCL_ERROR;
    }

    listobj = Tcl_NewListObj(0, NULL);

    length = taglib_audioproperties_length(properties);
    t_length = Tcl_NewIntObj(length);
    Tcl_ListObjAppendElement(interp, listobj, t_length);

    bitrate = taglib_audioproperties_bitrate(properties);
    t_bitrate = Tcl_NewIntObj(bitrate);
    Tcl_ListObjAppendElement(interp, listobj, t_bitrate);

    samplerate = taglib_audioproperties_samplerate(properties);
    t_samplerate = Tcl_NewIntObj(samplerate);
    Tcl_ListObjAppendElement(interp, listobj, t_samplerate);

    channels = taglib_audioproperties_channels(properties);
    t_channels = Tcl_NewIntObj(channels);
    Tcl_ListObjAppendElement(interp, listobj, t_channels);

    Tcl_SetObjResult(interp, listobj);
    return TCL_OK;
}



int TAGLIB_file_tag (ClientData clientData, Tcl_Interp *interp, 
                     int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    Tcl_Obj *returnvalue;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    char *handle;
    int len;

    TagLib_File *file;
    TagLib_Tag *tag;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "taglibHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
        return TCL_ERROR;
    }

    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid taglib handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    file = Tcl_GetHashValue( hashEntryPtr );
    tag = taglib_file_tag(file);
    if(tag == NULL) {
        return TCL_ERROR;
    }

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "tag%d", tsdPtr->tag_count++ );
    returnvalue = Tcl_NewStringObj( handleName, -1 );

    hashEntryPtr = Tcl_CreateHashEntry(tsdPtr->taglib_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(hashEntryPtr, tag);
    Tcl_MutexUnlock(&myMutex);

    Tcl_SetObjResult(interp, returnvalue);
    return TCL_OK;
}


int TAGLIB_tag_title (ClientData clientData, Tcl_Interp *interp, 
                      int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    Tcl_Obj *str_obj;
    char *handle;
    int len;

    TagLib_Tag *tag;
    char *string;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    }
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "invalid tag handle ", handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    tag = Tcl_GetHashValue( hashEntryPtr );

    string = taglib_tag_title(tag);
    str_obj = Tcl_NewStringObj( string, -1 );

    Tcl_SetObjResult(interp, str_obj);
    return TCL_OK;
}



int TAGLIB_tag_artist (ClientData clientData, Tcl_Interp *interp, 
                       int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    Tcl_Obj *str_obj;
    char *handle;
    int len;

    TagLib_Tag *tag;
    char *string;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    }
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );

    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "invalid tag handle ", handle, (char *)NULL );
        }

        return TCL_ERROR;
    }


    tag = Tcl_GetHashValue( hashEntryPtr );

    string = taglib_tag_artist(tag);
    str_obj = Tcl_NewStringObj( string, -1 );

    Tcl_SetObjResult(interp, str_obj);
    return TCL_OK;
}



int TAGLIB_tag_album (ClientData clientData, Tcl_Interp *interp, 
                      int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    Tcl_Obj *str_obj;
    char *handle;
    int len;

    TagLib_Tag *tag;
    char *string;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    } 
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    tag = Tcl_GetHashValue( hashEntryPtr );
    string = taglib_tag_album(tag);
    str_obj = Tcl_NewStringObj( string, -1 );

    Tcl_SetObjResult(interp, str_obj);
    return TCL_OK;
}


int TAGLIB_tag_comment (ClientData clientData, Tcl_Interp *interp, 
                        int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    Tcl_Obj *str_obj;
    char *handle;
    int len;

    TagLib_Tag *tag;
    char *string;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    } 
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );

    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    tag = Tcl_GetHashValue( hashEntryPtr );
    string = taglib_tag_comment(tag);
    str_obj = Tcl_NewStringObj( string, -1 );

    Tcl_SetObjResult(interp, str_obj);
    return TCL_OK;
}


int TAGLIB_tag_genre (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    Tcl_Obj *str_obj;
    char *handle;
    int len;

    TagLib_Tag *tag;
    char *string;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    } 
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );

    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }


    tag = Tcl_GetHashValue( hashEntryPtr );
    string = taglib_tag_genre(tag);
    str_obj = Tcl_NewStringObj( string, -1 );

    Tcl_SetObjResult(interp, str_obj);
    return TCL_OK;
}


int TAGLIB_tag_year (ClientData clientData, Tcl_Interp *interp, 
                     int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    Tcl_Obj *int_obj;
    char *handle;
    int len;

    TagLib_Tag *tag;
    int year;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    } 
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    tag = Tcl_GetHashValue( hashEntryPtr );
    year = taglib_tag_year(tag);
    int_obj = Tcl_NewIntObj(year);

    Tcl_SetObjResult(interp, int_obj);
    return TCL_OK;
}


int TAGLIB_tag_track (ClientData clientData, Tcl_Interp *interp, 
                      int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    Tcl_Obj *int_obj;
    char *handle; 
    int len;

    TagLib_Tag *tag;
    int track;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    } 
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );

    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    tag = Tcl_GetHashValue( hashEntryPtr );
    track = taglib_tag_track(tag);
    int_obj = Tcl_NewIntObj(track);

    Tcl_SetObjResult(interp, int_obj);
    return TCL_OK;
}


int TAGLIB_set_title (ClientData clientData, Tcl_Interp *interp, 
                      int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    int len;
    char *title;

    TagLib_Tag *tag;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 3)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle title");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    }
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );

    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    tag = Tcl_GetHashValue( hashEntryPtr );
    title = Tcl_GetStringFromObj(obj[2], 0);
    if(!title) {
	return TCL_ERROR;
    }
    taglib_tag_set_title(tag, title);

    return TCL_OK;
}


int TAGLIB_set_artist (ClientData clientData, Tcl_Interp *interp, 
                       int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    int len;
    char *artist;

    TagLib_Tag *tag;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 3)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle artist");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    }
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );

    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    tag = Tcl_GetHashValue( hashEntryPtr );
    artist = Tcl_GetStringFromObj(obj[2], 0);
    if(!artist) {
	return TCL_ERROR;
    }
    taglib_tag_set_artist(tag, artist);

    return TCL_OK;
}

int TAGLIB_set_album (ClientData clientData, Tcl_Interp *interp, 
                      int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    int len;
    char *album;
    TagLib_Tag *tag;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 3)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle album");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    } 
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    tag = Tcl_GetHashValue( hashEntryPtr );
    album = Tcl_GetStringFromObj(obj[2], 0);
    if(!album) {
        return TCL_ERROR;
    }
    taglib_tag_set_album(tag, album);

    return TCL_OK;
}

int TAGLIB_set_comment (ClientData clientData, Tcl_Interp *interp, 
                        int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    int len;
    char *comment;

    TagLib_Tag *tag;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 3)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle comment");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    }
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );

    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    tag = Tcl_GetHashValue( hashEntryPtr );
    comment = Tcl_GetStringFromObj(obj[2], 0);
    if(!comment) {
        return TCL_ERROR;
    }
    taglib_tag_set_comment(tag, comment);

    return TCL_OK;
}

int TAGLIB_set_genre (ClientData clientData, Tcl_Interp *interp, 
                      int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    int len;
    char *genre;

    TagLib_Tag *tag;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 3)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle genre");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    } 
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    tag = Tcl_GetHashValue( hashEntryPtr );
    genre = Tcl_GetStringFromObj(obj[2], 0);
    if(!genre) {
        return TCL_ERROR;
    }
    taglib_tag_set_genre(tag, genre);

    return TCL_OK;
}

int TAGLIB_set_year (ClientData clientData, Tcl_Interp *interp, 
                     int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    int len;
    int year;

    TagLib_Tag *tag;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 3)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle year");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    } 
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    tag = Tcl_GetHashValue( hashEntryPtr );
    if(Tcl_GetIntFromObj(interp, obj[2], &year) != TCL_OK) {
        return TCL_ERROR;
    }
    taglib_tag_set_year(tag, year);

    return TCL_OK;
}

int TAGLIB_set_track (ClientData clientData, Tcl_Interp *interp, 
                      int objc, Tcl_Obj *CONST obj[])
{
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    int len;
    int track;
    TagLib_Tag *tag;
    
    ThreadSpecificData *tsdPtr = (ThreadSpecificData *)
      Tcl_GetThreadData(&dataKey, sizeof(ThreadSpecificData));

    if (tsdPtr->initialized == 0) {
      tsdPtr->initialized = 1;
      tsdPtr->taglib_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
      Tcl_InitHashTable(tsdPtr->taglib_hashtblPtr, TCL_STRING_KEYS);
    }

    if(objc != 3)
    {
        Tcl_WrongNumArgs(interp, 1, obj, "tagHandle track");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(obj[1], &len);
    if(!handle || len < 1) {
	return TCL_ERROR;
    }
    hashEntryPtr = Tcl_FindHashEntry( tsdPtr->taglib_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj(resultObj, "invalid tag handle ", handle, (char *)NULL);
        }

        return TCL_ERROR;
    }

    tag = Tcl_GetHashValue( hashEntryPtr );
    if(Tcl_GetIntFromObj(interp, obj[2], &track) != TCL_OK) {
        return TCL_ERROR;
    }
    taglib_tag_set_track(tag, track);

    return TCL_OK;
}
