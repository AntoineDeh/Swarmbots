/**
 * \file  main_test.c
 * \version  0.1
 * \author Joshua MONTREUIL
 * \date Apr 30, 2023
 * \brief main test file.
 *
 * \section License
 *
 * The MIT License
 *
 * Copyright (c) 2023, Prose A2 2023
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * \copyright Prose A2 2023
 * 
 */
/* ----------------------  INCLUDES  ---------------------------------------- */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * \def TESTS_SUITE_NB
 * Number of tests suite to be executed.
 * */
#define TESTS_SUITE_NB 4
/**
 * \see /controller/controller_core_test.c
 */
extern int CONTROLLER_CORE_TEST_run_tests(void);
/**
 * \see /controller/controller_ringer_test.c
 */
extern int CONTROLLER_RINGER_TEST_run_tests(void);
/**
 * \see /controller/pilot_test.c
 */
extern int PILOT_TEST_run_tests(void);
/**
 * \see /controller/state_indicator_test.c
 */
extern int STATE_INDICATOR_TEST_run_tests(void);
/**
 * \see /com/dispatcher_test.c
 */
extern int DISPATCHER_run_tests(void);
/**
 * \see /com/logs_manager_proxy_test.c
 */
extern int LOGS_MANAGER_PROXY_TEST_run_tests(void);
/**
 * \see /com/gui_secretary_proxy_test.c
 */
extern int GUI_SECRETARY_PROXY_TEST_run_tests(void);
/**
 * \see /com/gui_ringer_proxy_test.c
 */
extern int GUI_RINGER_PROXY_TEST_run_tests(void);
/**
 * \see /com/gui_proxy_test.c
 */
extern int GUI_PROXY_TEST_run_tests(void);

/** List of tests suite to run */
int (* suite_tests[])(void) = {
	CONTROLLER_CORE_TEST_run_tests,
	CONTROLLER_RINGER_TEST_run_tests,
	PILOT_TEST_run_tests,
	STATE_INDICATOR_TEST_run_tests,
    //DISPATCHER_run_tests,   /* Not working */
    //LOGS_MANAGER_PROXY_TEST_run_tests,    /* Not working */
    //GUI_SECRETARY_PROXY_TEST_run_tests,   /* Not working */
	//GUI_RINGER_PROXY_TEST_run_tests,  /* Not working */
    //GUI_PROXY_TEST_run_tests, /* Not working */
};


/**
 * \fn static void test_runner()
 * \brief Generic code for the test launch suite of CMOCKA.
 */
static void test_runner();
/**
 * \fn static void affiche_aide(char * nom)
 * \brief Help ihm.
 */
static void affiche_aide(char * nom);

/**
 * \fn int main(int argc, char * argv[])
 * \brief Main function for the test launch suite.
 */
int main(int argc, char * argv[]) {
	if ( (argc > 1) )
	{
		if (! strcmp(argv[1], "-text")) {
			cmocka_set_message_output(CM_OUTPUT_STDOUT);
		} else
		if (! strcmp(argv[1], "-subunit")) {
			cmocka_set_message_output(CM_OUTPUT_SUBUNIT);
		} else
		if (! strcmp(argv[1], "-tap")) {
			cmocka_set_message_output(CM_OUTPUT_TAP);
		} else
		if (! strcmp(argv[1], "-xml")) {
			cmocka_set_message_output(CM_OUTPUT_XML);
		} else {
			affiche_aide(argv[0]);
		}
	}

	test_runner();

	return 0;
}


static void test_runner() {
	int i;
	for (i = 0; i < TESTS_SUITE_NB; i++)
	{
		suite_tests[i]();
	}
}

static void affiche_aide(char * nom) {
	printf("Suite de tests pour le programme à tester\n");
	printf("Usage : %s [-text] [-subunit] [-tap] [-xml]\n", nom);

	exit(0);
}

