/**
 * @file safe_status.h
 * @brief Common status codes for safety-critical applications
 * 
 * This file provides standardized status codes for error reporting
 * in complience with MISRA C
 * 
 * @version 1.0
 */
#ifndef INC_SAFE_STATUS_H
#define INC_SAFE_STATUS_H

typedef enum{
    STATUS_OK               = 0U,
    STATUS_ERROR_GENERIC    = 1U,   // Generic error
    STATUS_ERROR_NULL       = 2U,   // Null pointer provided
    STATUS_ERROR_PARAM      = 3U,   // Invalid parameter
    STATUS_ERROR_RANGE      = 4U,   // Value out of range
    STATUS_ERROR_BUSY       = 5U,   // Resource Busy
    STATUS_ERROR_TIMEOUT    = 6U,   // Operation timed out
    STATUS_ERROR_RESOURCE   = 7U,   // Resource unavailable
    STATUS_ERROR_INIT       = 8U,   // Initialization error
    STATUS_ERROR_STATE      = 9U,   // Invalid state for operation
    STATUS_ERROR_NOT_IMPLEMENTED = 10U, // Feature not implemented
    STATUS_PENDING          = 11U,  // Operation in progress
}Status_t;

#endif /* INC_SAFE_STATUS_H */
