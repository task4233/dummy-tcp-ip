#pragma once
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <stdlib.h>
#include "../src/dudp.h"

void unwrap_DUDP_Data_Test(void) {
  int res;
  DUDP* got = (DUDP *)malloc(sizeof(DUDP));

  // correct data
  unsigned char *correct_test_data = "\xa\x0\x0\x0\xc\x0\x0\x0\xda\x77\x62\x0e\xbd\xfb\x99\xd6\x65\x4b\x93\xae\x7f\x26\x4f\x67hogefugapiyo\0";
  res = unwrap_DUDP_Data(correct_test_data, &got[0]);
  CU_ASSERT_EQUAL(got->type, 10);
  CU_ASSERT_EQUAL(got->len, 12);
  CU_ASSERT_EQUAL(res, 0);

  free(got);
}