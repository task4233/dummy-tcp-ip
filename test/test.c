#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <string.h>
#include <unistd.h>
#include "dudp.h"

int main(void)
{
  CU_pSuite suite;
  CU_initialize_registry();

  suite = CU_add_suite("DUDP Test", NULL, NULL);
  CU_add_test(suite, "interpret_DUDP_Data_Test", interpret_DUDP_Data_Test);

  CU_console_run_tests();
  CU_cleanup_registry();
  return 0;
}
