#pragma once
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include "../src/dtcp.h"

void unwrap_DTCP_Data_Test(void) {
  // correct data
  unsigned char *correct_test_data = "\xa\x0\x0\x0\xc\x0\x0\x0\xda\x77\x62\x0e\xbd\xfb\x99\xd6\x65\x4b\x93\xae\x7f\x26\x4f\x67hogefugapiyo\0";
  DTCP* got = unwrap_DTCP_Data(correct_test_data);
  CU_ASSERT_EQUAL(got->type, 10);
  CU_ASSERT_EQUAL(got->len, 12);
  CU_ASSERT_NSTRING_EQUAL(got->digest, "\xda\x77\x62\x0e\xbd\xfb\x99\xd6\x65\x4b\x93\xae\x7f\x26\x4f\x67", 16);
  free(got);

  // invalid digest
  unsigned char *invalid_digest_test_data = "\xa\x0\x0\x0\xc\x0\x0\x0\xda\x77\x62\x0e\xbd\xfb\x99\xd6\x65\x4b\x93\xae\x7f\x27\x4f\x67hogefugapiyo\0";
  got = unwrap_DTCP_Data(invalid_digest_test_data);
  CU_ASSERT_PTR_NULL(got);
}
