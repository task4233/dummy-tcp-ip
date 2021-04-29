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

// 忘れずにfreeすること
// TODO: 引数で実体を渡すようにする
DIP *unwrap_DIP_Data(unsigned char *data)
{
	puts("=========================DIP============================");
	DIP *dip = (DIP *)calloc(1, sizeof(DIP));

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
		DTCP *dtcp = unwrap_DTCP_Data(&data[12]);
		free(dtcp);
		break;
	}
	case 17:
	{
		// udp
		DUDP *dudp = unwrap_DUDP_Data(&data[12]);
		free(dudp);
		break;
	}
	default:
		// 後々実装されるかもしれないのでエラーのみ出しておく
		fprintf(stderr, "type %d is invalid type\n", dip->type);
		break;
	}

	return dip;
}

void wrap_DIP_Data(DIP *dip, unsigned char *data, unsigned int data_size)
{
	memcpyUint32(&data[0], dip->type);
	memcpyUint32(&data[4], dip->version);
	memcpyUint32(&data[8], dip->ttl);
	memcpy(&data[12], &dip->data[0], data_size);
}