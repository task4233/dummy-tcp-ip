#pragma once
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include "../src/dip.h"

void interpret_DIP_Data_Test(void) {
    // correct data(tcp)
    unsigned char* correct_test_data_tcp = "\x4\x0\x0\x0\x2\x0\x0\x0\x3\x0\x0\x0\xa\x0\x0\x0\xc\x0\x0\x0\xda\x77\x62\x0e\xbd\xfb\x99\xd6\x65\x4b\x93\xae\x7f\x26\x4f\x67hogefugapiyo\0";
    DIP* got = interpret_DIP_Data(correct_test_data_tcp);
    CU_ASSERT_EQUAL(got->type, 4);
    CU_ASSERT_EQUAL(got->version, 2);
    CU_ASSERT_EQUAL(got->ttl, 3);
    free(got);

    // correct data(udp)
    unsigned char* correct_test_data_udp = "\x11\x0\x0\x0\x2\x0\x0\x0\x3\x0\x0\x0\xa\x0\x0\x0\xc\x0\x0\x0hogefugapiyo\0";
    got = interpret_DIP_Data(correct_test_data_udp);
    CU_ASSERT_EQUAL(got->type, 17);
    CU_ASSERT_EQUAL(got->version, 2);
    CU_ASSERT_EQUAL(got->ttl, 3);
    free(got);

    // invalid data
        unsigned char* invalid_test_data = "\x01\x0\x0\x0\x2\x0\x0\x0\x3\x0\x0\x0\xa\x0\x0\x0\xc\x0\x0\x0hogefugapiyo\0";
    DIP* got = interpret_DIP_Data(invalid_test_data);
    CU_ASSERT_EQUAL(got->type, 1);
    CU_ASSERT_EQUAL(got->version, 2);
    CU_ASSERT_EQUAL(got->ttl, 3);
    free(got);
}