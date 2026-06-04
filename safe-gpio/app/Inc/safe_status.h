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
    STATUS_ERROR_GENERIC    = 1U, // Generic error
    STATUS_ERROR_NULL       = 2U, // Null pointer provided
    STATUS_ERROR_PARAM      = 3U, // Invalid parameter
    STATUS_ERROR_RANGE      = 4U, // Value out of range
    STATUS_ERROR_BUSY       = 5U  // Resource Busy
}Status_t;

#endif /* INC_SAFE_STATUS_H */
