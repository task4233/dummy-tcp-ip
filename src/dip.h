#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "dtcp.h"
#include "utils.h"

typedef struct
{
	uint32_t type;
	uint32_t version;
	uint32_t ttl;
} DIP;

char *interpret_DIP_Data(char *data)
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
	case 4:
		// tcp
		interpret_DTCP_Data(&data[12]);
		break;
	case 17:
		// udp
		// interpret_DUDP_Data(&data[8]);
		break;
	default:
		fprintf(stderr, "type %d is invalid type\n", dip->type);
		break;
	}

	free(dip);

	return data;
}
