#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "dtcp.h"
#include "dudp.h"
#include "utils.h"

typedef struct
{
	uint32_t type;
	uint32_t version;
	uint32_t ttl;
	unsigned char *data; // the pointer to data in next layer
} DIP;

// if error occurs, return 1
// if not, return 0
int unwrap_DIP_Data(unsigned char *data, DIP* dip)
{
	puts("=========================DIP============================");
	memcpy(&dip->type, data, 4);
	printf("type   : %0d\n", dip->type);

	memcpy(&dip->version, data + 4, 4);
	printf("version: %0d\n", dip->version);

	memcpy(&dip->ttl, data + 8, 4);
	printf("ttl    : %0d\n", dip->ttl);

	puts("========================================================");
	switch (dip->type)
	{
	case 6:
	{
		// tcp
		DTCP *dtcp = (DTCP *)malloc(sizeof(DTCP));
		if (unwrap_DTCP_Data(&data[12], &dtcp[0])) {
			free(dtcp);
			return 1;
		}
		free(dtcp);
		break;
	}
	case 17:
	{
		// udp
		DUDP *dudp = (DUDP *)malloc(sizeof(DUDP));
		if (unwrap_DUDP_Data(&data[12], &dudp[0])) {
			free(dudp);
			return 1;
		}
		free(dudp);
		break;
	}
	default:
		// 後々実装されるかもしれないのでエラーのみ出しておく
		fprintf(stderr, "type %d is invalid type\n", dip->type);
		return 1;
	}
	return 0;
}

int wrap_DIP_Data(DIP *dip, unsigned char *data, unsigned int data_size)
{
	memcpyUint32(&data[0], dip->type);
	memcpyUint32(&data[4], dip->version);
	memcpyUint32(&data[8], dip->ttl);
	memcpy(&data[12], &dip->data[0], data_size);
	return 0;
}