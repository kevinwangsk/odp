/* Copyright (c) 2017, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#include "config.h"
#include <odp_api.h>
#include <odp_cunit_common.h>
#include "event.h"

#define NUM_EVENTS  100
#define EVENT_SIZE  100
#define EVENT_BURST 10

static void event_test_free(void)
{
	odp_pool_t pool;
	odp_pool_param_t pool_param;
	int i;
	odp_buffer_t buf;
	odp_packet_t pkt;
	odp_timeout_t tmo;
	odp_event_subtype_t subtype;
	odp_event_t event[EVENT_BURST];

	/* Buffer events */
	odp_pool_param_init(&pool_param);
	pool_param.buf.num  = NUM_EVENTS;
	pool_param.buf.size = EVENT_SIZE;
	pool_param.type     = ODP_POOL_BUFFER;

	pool = odp_pool_create("event_free", &pool_param);
	CU_ASSERT_FATAL(pool != ODP_POOL_INVALID);

	for (i = 0; i < EVENT_BURST; i++) {
		buf = odp_buffer_alloc(pool);
		CU_ASSERT_FATAL(buf != ODP_BUFFER_INVALID);
		event[i] = odp_buffer_to_event(buf);
		CU_ASSERT(odp_event_type(event[i]) == ODP_EVENT_BUFFER);
		CU_ASSERT(odp_event_subtype(event[i]) == ODP_EVENT_NO_SUBTYPE);
		CU_ASSERT(odp_event_types(event[i], &subtype) ==
			  ODP_EVENT_BUFFER);
		CU_ASSERT(subtype == ODP_EVENT_NO_SUBTYPE);
	}

	for (i = 0; i < EVENT_BURST; i++)
		odp_event_free(event[i]);

	CU_ASSERT(odp_pool_destroy(pool) == 0);

	/* Packet events */
	odp_pool_param_init(&pool_param);
	pool_param.pkt.num = NUM_EVENTS;
	pool_param.pkt.len = EVENT_SIZE;
	pool_param.type    = ODP_POOL_PACKET;

	pool = odp_pool_create("event_free", &pool_param);
	CU_ASSERT_FATAL(pool != ODP_POOL_INVALID);

	for (i = 0; i < EVENT_BURST; i++) {
		pkt = odp_packet_alloc(pool, EVENT_SIZE);
		CU_ASSERT_FATAL(pkt != ODP_PACKET_INVALID);
		event[i] = odp_packet_to_event(pkt);
		CU_ASSERT(odp_event_type(event[i]) == ODP_EVENT_PACKET);
		CU_ASSERT(odp_event_subtype(event[i]) ==
			  ODP_EVENT_PACKET_BASIC);
		CU_ASSERT(odp_event_types(event[i], &subtype) ==
			  ODP_EVENT_PACKET);
		CU_ASSERT(subtype == ODP_EVENT_PACKET_BASIC);
	}

	for (i = 0; i < EVENT_BURST; i++)
		odp_event_free(event[i]);

	CU_ASSERT(odp_pool_destroy(pool) == 0);

	/* Timeout events */
	odp_pool_param_init(&pool_param);
	pool_param.tmo.num = NUM_EVENTS;
	pool_param.type    = ODP_POOL_TIMEOUT;

	pool = odp_pool_create("event_free", &pool_param);
	CU_ASSERT_FATAL(pool != ODP_POOL_INVALID);

	for (i = 0; i < EVENT_BURST; i++) {
		tmo = odp_timeout_alloc(pool);
		CU_ASSERT_FATAL(tmo != ODP_TIMEOUT_INVALID);
		event[i] = odp_timeout_to_event(tmo);
		CU_ASSERT(odp_event_type(event[i]) == ODP_EVENT_TIMEOUT);
		CU_ASSERT(odp_event_subtype(event[i]) == ODP_EVENT_NO_SUBTYPE);
		CU_ASSERT(odp_event_types(event[i], &subtype) ==
			  ODP_EVENT_TIMEOUT);
		CU_ASSERT(subtype == ODP_EVENT_NO_SUBTYPE);
	}

	for (i = 0; i < EVENT_BURST; i++)
		odp_event_free(event[i]);

	CU_ASSERT(odp_pool_destroy(pool) == 0);
}

static void event_test_free_multi(void)
{
	odp_pool_t pool;
	odp_pool_param_t pool_param;
	int i, j;
	odp_buffer_t buf;
	odp_packet_t pkt;
	odp_timeout_t tmo;
	odp_event_t event[EVENT_BURST];

	/* Buffer events */
	odp_pool_param_init(&pool_param);
	pool_param.buf.num  = NUM_EVENTS;
	pool_param.buf.size = EVENT_SIZE;
	pool_param.type     = ODP_POOL_BUFFER;

	pool = odp_pool_create("event_free", &pool_param);
	CU_ASSERT_FATAL(pool != ODP_POOL_INVALID);

	for (j = 0; j < 2; j++) {
		for (i = 0; i < EVENT_BURST; i++) {
			buf = odp_buffer_alloc(pool);
			CU_ASSERT_FATAL(buf != ODP_BUFFER_INVALID);
			event[i] = odp_buffer_to_event(buf);
		}

		if (j == 0)
			odp_event_free_multi(event, EVENT_BURST);
		else
			odp_event_free_sp(event, EVENT_BURST);
	}

	CU_ASSERT(odp_pool_destroy(pool) == 0);

	/* Packet events */
	odp_pool_param_init(&pool_param);
	pool_param.pkt.num = NUM_EVENTS;
	pool_param.pkt.len = EVENT_SIZE;
	pool_param.type    = ODP_POOL_PACKET;

	pool = odp_pool_create("event_free", &pool_param);
	CU_ASSERT_FATAL(pool != ODP_POOL_INVALID);

	for (j = 0; j < 2; j++) {
		for (i = 0; i < EVENT_BURST; i++) {
			pkt = odp_packet_alloc(pool, EVENT_SIZE);
			CU_ASSERT_FATAL(pkt != ODP_PACKET_INVALID);
			event[i] = odp_packet_to_event(pkt);
		}

		if (j == 0)
			odp_event_free_multi(event, EVENT_BURST);
		else
			odp_event_free_sp(event, EVENT_BURST);
	}

	CU_ASSERT(odp_pool_destroy(pool) == 0);

	/* Timeout events */
	odp_pool_param_init(&pool_param);
	pool_param.tmo.num = NUM_EVENTS;
	pool_param.type    = ODP_POOL_TIMEOUT;

	pool = odp_pool_create("event_free", &pool_param);
	CU_ASSERT_FATAL(pool != ODP_POOL_INVALID);

	for (j = 0; j < 2; j++) {
		for (i = 0; i < EVENT_BURST; i++) {
			tmo = odp_timeout_alloc(pool);
			CU_ASSERT_FATAL(tmo != ODP_TIMEOUT_INVALID);
			event[i] = odp_timeout_to_event(tmo);
		}

		if (j == 0)
			odp_event_free_multi(event, EVENT_BURST);
		else
			odp_event_free_sp(event, EVENT_BURST);
	}

	CU_ASSERT(odp_pool_destroy(pool) == 0);
}

static void event_test_free_multi_mixed(void)
{
	odp_pool_t pool1, pool2, pool3;
	odp_pool_param_t pool_param;
	int i, j;
	odp_buffer_t buf;
	odp_packet_t pkt;
	odp_timeout_t tmo;
	odp_event_t event[3 * EVENT_BURST];

	/* Buffer events */
	odp_pool_param_init(&pool_param);
	pool_param.buf.num  = NUM_EVENTS;
	pool_param.buf.size = EVENT_SIZE;
	pool_param.type     = ODP_POOL_BUFFER;

	pool1 = odp_pool_create("event_free1", &pool_param);
	CU_ASSERT_FATAL(pool1 != ODP_POOL_INVALID);

	/* Packet events */
	odp_pool_param_init(&pool_param);
	pool_param.pkt.num = NUM_EVENTS;
	pool_param.pkt.len = EVENT_SIZE;
	pool_param.type    = ODP_POOL_PACKET;

	pool2 = odp_pool_create("event_free2", &pool_param);
	CU_ASSERT_FATAL(pool2 != ODP_POOL_INVALID);

	/* Timeout events */
	odp_pool_param_init(&pool_param);
	pool_param.tmo.num = NUM_EVENTS;
	pool_param.type    = ODP_POOL_TIMEOUT;

	pool3 = odp_pool_create("event_free3", &pool_param);
	CU_ASSERT_FATAL(pool3 != ODP_POOL_INVALID);

	for (j = 0; j < 2; j++) {
		for (i = 0; i < 3 * EVENT_BURST;) {
			buf = odp_buffer_alloc(pool1);
			CU_ASSERT_FATAL(buf != ODP_BUFFER_INVALID);
			event[i] = odp_buffer_to_event(buf);
			i++;
			pkt = odp_packet_alloc(pool2, EVENT_SIZE);
			CU_ASSERT_FATAL(pkt != ODP_PACKET_INVALID);
			event[i] = odp_packet_to_event(pkt);
			i++;
			tmo = odp_timeout_alloc(pool3);
			CU_ASSERT_FATAL(tmo != ODP_TIMEOUT_INVALID);
			event[i] = odp_timeout_to_event(tmo);
			i++;
		}

		if (j == 0)
			odp_event_free_multi(event, 3 * EVENT_BURST);
		else
			odp_event_free_sp(event, 3 * EVENT_BURST);
	}

	CU_ASSERT(odp_pool_destroy(pool1) == 0);
	CU_ASSERT(odp_pool_destroy(pool2) == 0);
	CU_ASSERT(odp_pool_destroy(pool3) == 0);
}

odp_testinfo_t event_suite[] = {
	ODP_TEST_INFO(event_test_free),
	ODP_TEST_INFO(event_test_free_multi),
	ODP_TEST_INFO(event_test_free_multi_mixed),
	ODP_TEST_INFO_NULL,
};

odp_suiteinfo_t event_suites[] = {
	{"Event", NULL, NULL, event_suite},
	ODP_SUITE_INFO_NULL,
};

int event_main(int argc, char *argv[])
{
	int ret;

	/* parse common options: */
	if (odp_cunit_parse_options(argc, argv))
		return -1;

	ret = odp_cunit_register(event_suites);

	if (ret == 0)
		ret = odp_cunit_run();

	return ret;
}
