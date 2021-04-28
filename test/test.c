#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <string.h>
#include <unistd.h>
#include "dudp.h"
#include "dtcp.h"
#include "dip.h"

int main(void)
{
  CU_pSuite suite;
  CU_initialize_registry();

  suite = CU_add_suite("DUDP Test", NULL, NULL);
  CU_add_test(suite, "unwrap_DUDP_Data_Test", unwrap_DUDP_Data_Test);
  CU_add_test(suite, "unwrap_DTCP_Data_Test", unwrap_DTCP_Data_Test);
  CU_add_test(suite, "unwrap_DIP_Data_Test", unwrap_DIP_Data_Test);

  CU_console_run_tests();
  CU_cleanup_registry();
  return 0;
}
