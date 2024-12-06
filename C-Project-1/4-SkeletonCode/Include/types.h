/*************************************************************************************************************************************
** Name                 : Rajesh jonnalgadda.
** Date                 : //2024.
** 
** @File	            : types.h
** Description 			: This file consist of enum values for whole process.
** 
**
***********************************************************************************************************************************/

#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn return type */

typedef enum
{
	d_success,
	d_failure
}d_Status;


typedef enum
{
    e_success,
    e_failure
} Status;


typedef enum
{
    e_encode,
    e_decode,
    e_unsupported

} OperationType;

#endif
