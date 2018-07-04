#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "output.h"

#define STR_SIZE 100

struct output_test {
	double value;
	const char result[STR_SIZE];
	char siPrefix;
	int precision;
};


void TEST_output(void)
{
	struct output_test tests[] = {
		{1e-22, "100y", '\0', -1},
		{1e0, "1", '\0', -1},
		{1e22, "10Z", '\0', -1},
		{1.123456089, "1.12346", '\0', -1},
		{1.123456089, "1", '\0', 0},
		{1.123456089, "1.1", '\0', 2},
		{1.123456089, "1.12", '\0', 3},
		{1.123456089, "1.123", '\0', 4},
		{1.123456089, "1.1235", '\0', 5},
		{1.123456089, "1.12346", '\0', 6},
		{1.123456089, "1.123456", '\0', 7},
		{1.123456089, "1.1234561", '\0', 8},
		{1.123456089, "1.12345609", '\0', 9},
		{1.123456089, "1.123456089", '\0', 10},
		{1.123456089, "1.123456089", '\0', 11},
		{0.001, "1m", 'm', -1},
		{-8.7e-4, "-870u", 'u', -1},
		{8.7e-10, "0.87n", 'n', -1},
		{3.141e24, "3.141Y", 'Y', -1},
		{3.141e18, "3.141E", 'E', -1},
		{3.141e15, "3.141P", 'P', -1},
		{3.141e12, "3.141T", 'T', -1},
		{3.141e9, "3.141G", 'G', -1},
		{3.141e6, "3.141M", 'M', -1},
		{3.141e3, "3.141k", 'k', -1},
		{3.141e-21, "3.141z", 'z', -1},
		{3.141e-18, "3.141a", 'a', -1},
		{3.141e-15, "3.141f", 'f', -1},
		{3.141e-12, "3.141p", 'p', -1},
		{3.141e-9, "3.141n", 'n', -1}
	};
	int test_count = sizeof(tests)/sizeof(struct output_test);
	int i;

	char str[STR_SIZE];

	for(i=0; i<test_count; i++){
		doubleToString(tests[i].value, str, STR_SIZE, tests[i].siPrefix, tests[i].precision);
		CU_ASSERT_STRING_EQUAL(str, tests[i].result);
		if(strcmp(str, tests[i].result)){
			printf("FAIL: %s %s\n", tests[i].result, str);
		}
	}
}


int init_tests(void)
{
	CU_pSuite test_suite = NULL;

	test_suite = CU_add_suite("tests", NULL, NULL);
	if(!test_suite){
		printf("Error adding CUnit test suite.\n");
		return 1;
	}

	if(!CU_add_test(test_suite, "output", TEST_output)){
		printf("Error adding CUnit tests.\n");
		return 1;
	}
	return 0;
}


int main(int argc, char *argv[])
{
	if(CU_initialize_registry() != CUE_SUCCESS){
		printf("Error initializing CUnit registry.\n");
		return 1;
	}

	if(init_tests()){
		CU_cleanup_registry();
		return 1;
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return 0;
}
