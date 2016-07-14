/*
 * common.h
 *
 *  Created on: Aug 9, 2015
 *      Author: mduzinki
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <errno.h>

#define MAX_STR_LEN 300

/** Generic HANDLE definition */
typedef void * HANDLE;

/** UINT8 type definition */
typedef unsigned char UINT8;

/** BOOLEAN definition, using natural variable */
typedef int BOOLEAN;
/** BOOLEAN definition, using natural variable */
typedef int BOOL;
//typedef int bool;

#ifndef TRUE
#define TRUE  1     /**< True value for a BOOLEAN */
#endif
#ifndef FALSE
#define FALSE 0     /**< False value for a BOOLEAN */
#endif
/* When in CPP don't compile these macros since they are natively keywords*/
#ifndef __cplusplus
#define true  1     /**< True value for a BOOLEAN */
#define false 0     /**< False value for a BOOLEAN */
#endif
#define True  1     /**< True value for a BOOLEAN */
#define False 0     /**< False value for a BOOLEAN */

#define IN
#define OUT
#define IO
#define STAT

#define STATUS int

#ifndef FLG_SET
#define FLG_SET 1
#endif

#ifndef FLG_CLRD
#define FLG_CLRD 0
#endif


/*time events*/
//TE_BOT     - Time Event: Begin of time
//TE_10MS    - Time Event: 10ms time tick
//TE_20MS    - Time Event: 20ms time tick
//TE_100MS   - Time Event: 100ms time tick
//TE_500MS   - Time Event: 500ms time tick
//TE_1000MS  - Time Event: 1000ms time tick
//TE_EOT     - Time Event: End of time
#define TE_BOT    (1 << 0)
#define TE_10MS   (1 << 1)
#define TE_20MS   (1 << 2)
#define TE_100MS  (1 << 3)
#define TE_500MS  (1 << 4)
#define TE_1000MS (1 << 5)
#define TE_EOT    (1 << 6)

#define NO_EVENT   0

typedef enum _TIME_EVENT_INDEX
{
   TE_BOT_IDX = 0,
   TE_10MS_IDX,
   TE_20MS_IDX,
   TE_100MS_IDX,
   TE_500MS_IDX,
   TE_1000MS_IDX,
   TE_EOT_IDX,
   TE_LAST_IDX
} TIME_EVENT_INDEX;

typedef unsigned char TIME_EVENT;

#define IDX_TO_MAP(Idx) ((0x1) << (Idx))
#define RMV_IDX_FROM_MAP(Idx) (~(IDX_TO_MAP(Idx)))
#define IS_IDX_SET_IN_MAP(Idx,Map) (0 != ((Map) & IDX_TO_MAP(Idx)))

#define DIV_ROUND_CLOSEST(x, divisor)(          \
{                      						    \
    typeof(x) __x = x;           			    \
    typeof(divisor) __d = divisor;   	        \
    (((typeof(x))-1) > 0 ||             		\
     ((typeof(divisor))-1) > 0 || (__x) > 0) ?  \
        (((__x) + ((__d) / 2)) / (__d)) :  	    \
        (((__x) - ((__d) / 2)) / (__d));   	    \
}                           					\
)


///////////////////////////////////////////////////////////////////////////////
//                        ERROR & STATUS CODE RANGES                         //
///////////////////////////////////////////////////////////////////////////////

/*
* Before add new status code please check if this code doesn't exist at errno.h.
*/

//
// Status codes matching POSIX codes
//

//
//STATUS_SUCCESS
//    The operation completed successfully.
//
#define STATUS_SUCCESS                OK

#define OK 0
//
//STATUS_DELETED
//    The requested object has been deleted.
//
#define STATUS_DELETED                ENOENT

//
//STATUS_PTR_ERROR
//    An invalid pointer was provided.
//
#define STATUS_PTR_ERROR              -2

//
//STATUS_SIZE_ERROR
//    The size of the specified object (memory pool, stack, message queue,
//    etc.) is invalid.
//
#define STATUS_SIZE_ERROR             ERANGE

//
//STATUS_NO_INSTANCE
//    The requested instance was not found (semaphore count is 0x0).
//
#define STATUS_NO_INSTANCE            ENXIO

//
//STATUS_THREAD_ERROR
//    An invalid thread control pointer provided or the thread is already created.
//
#define STATUS_THREAD_ERROR           EINVALIDTID

//
//STATUS_INVALID_FUNCTION
//    The requested function was not found.
//
#define STATUS_INVALID_FUNCTION       ENOSYS

//
//STATUS_NOT_FOUND
//    The requested item could not be found.
//
#define STATUS_NOT_FOUND              ENXIO

//
//STATUS_ACCESS_DENIED
//    Access to the requested object could not be granted.
//
#define STATUS_ACCESS_DENIED          EACCES

//
//STATUS_INVALID_HANDLE
//    The provided handle does not exist.
//
#define STATUS_INVALID_HANDLE         EBADF

//
//STATUS_INVALID_ACCESS
//    An attempt was made to access an inaccessible or unavailable object.
//
#define STATUS_INVALID_ACCESS         EACCES

//
//STATUS_INVALID_PARAMS
//    One or more parameters are invalid.
//
#define STATUS_INVALID_PARAMS         EINVAL

//
//STATUS_WRITE_PROTECTED
//    The media is write protected.  When writing to flash, this means
//    the flash block is write protected.
//
#define STATUS_WRITE_PROTECTED        EROFS

//
//STATUS_NOT_READY
//    The h/w device is not ready.
//
#define STATUS_NOT_READY              EAGAIN

//
//STATUS_NOT_SUPPORTED
//    The function, message or request is not recognized or supported.
//
#define STATUS_NOT_SUPPORTED          ENOSYS

//
//STATUS_INVALID_ADDRESS
//    The address provided in invalid.
//
#define STATUS_INVALID_ADDRESS        EFAULT

//
//STATUS_BUFFER_TOO_SMALL
//    The provided buffer is too small to hold the requested data.
//
#define STATUS_BUFFER_TOO_SMALL       EBUFTOOSML

//
//STATUS_TIMED_OUT
//    The device or object timed out and was unable to complete the request.
//
#define STATUS_TIMED_OUT              ETIMEDOUT

//
//STATUS_RESOURCE_BUSY
//    The requested resource is busy or can not be acquired.
//
#define STATUS_RESOURCE_BUSY          EBUSY

//
//STATUS_FAILURE
//    A unknown error encountered while performing a requested transaction.
//
#define STATUS_FAILURE                EGENERIC

//
//STATUS_OUT_OF_MEMORY
//    This error can be returned for the following reasons:
//       1.Attempt to allocate memory and no memory is available
//       2.Attempt to free a memory block on which a thread is suspended
//    NOTE: This value is the same a as STATUS_START_ERROR.
//
#define STATUS_OUT_OF_MEMORY          ENOMEM

//
//STATUS_GENERAL_ERROR
//    This is a general error code used for errors that do not require a specific
//    code.
#define STATUS_GENERAL_ERROR          EGENERIC

#define EGENERIC -1

//
//STATUS_FILE_OPERATION_ERROR
//    This is a general error code returned by POSIX system calls like open, close, read, write, ioctl
//
#define STATUS_FILE_OPERATION_ERROR   (-1)




#endif /* COMMON_H_ */
