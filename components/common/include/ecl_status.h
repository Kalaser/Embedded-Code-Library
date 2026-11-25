#ifndef ECL_COMMON_ECL_STATUS_H
#define ECL_COMMON_ECL_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ecl_status {
    ECL_STATUS_OK = 0,
    ECL_STATUS_INVALID_ARGUMENT = -1,
    ECL_STATUS_INVALID_STATE = -2,
    ECL_STATUS_NO_MEMORY = -3,
    ECL_STATUS_TIMEOUT = -4,
    ECL_STATUS_NOT_SUPPORTED = -5,
    ECL_STATUS_IO_ERROR = -6
} ecl_status_t;

#ifdef __cplusplus
}
#endif

#endif /* ECL_COMMON_ECL_STATUS_H */
