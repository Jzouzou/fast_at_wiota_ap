/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */
/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <string.h> /* for memset */
#include "mount.h"

/* This file is copied from RFC1813
 * Copyright 1995 Sun Micrososystems (I assume)
 */

typedef char* caddr_t;

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

enum clnt_stat 
mountproc3_null_3(void *clnt_res, CLIENT *clnt)
{
	 return (clnt_call(clnt, MOUNTPROC3_NULL,
		(xdrproc_t) xdr_void, (caddr_t) NULL,
		(xdrproc_t) xdr_void, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mountproc3_mnt_3(dirpath arg1, mountres3 *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC3_MNT,
		(xdrproc_t) xdr_dirpath, (caddr_t) &arg1,
		(xdrproc_t) xdr_mountres3, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mountproc3_dump_3(mountlist *clnt_res, CLIENT *clnt)
{
	 return (clnt_call(clnt, MOUNTPROC3_DUMP,
		(xdrproc_t) xdr_void, (caddr_t) NULL,
		(xdrproc_t) xdr_mountlist, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mountproc3_umnt_3(dirpath arg1, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC3_UMNT,
		(xdrproc_t) xdr_dirpath, (caddr_t) &arg1,
		(xdrproc_t) xdr_void, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mountproc3_umntall_3(void *clnt_res, CLIENT *clnt)
{
	 return (clnt_call(clnt, MOUNTPROC3_UMNTALL,
		(xdrproc_t) xdr_void, (caddr_t) NULL,
		(xdrproc_t) xdr_void, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mountproc3_export_3(exports *clnt_res, CLIENT *clnt)
{
	 return (clnt_call(clnt, MOUNTPROC3_EXPORT,
		(xdrproc_t) xdr_void, (caddr_t) NULL,
		(xdrproc_t) xdr_exports, (caddr_t) clnt_res,
		TIMEOUT));
}
