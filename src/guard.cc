/*
 * Copyright 2014 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of the license at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the
 * License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
 */
#include "ltguard/guard.h"
#include "ltguard/utils.h"

#include <ts/remap.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

static TSCont lt_guard_continuation;
static volatile boost::property_tree::ptree* config = NULL;

static void
read_configuration(TSCont contp) {
	const char * path = (const char *)TSContDataGet(contp);
}

static int
lt_guard_global_hook(TSCont contp, TSEvent event, void * edata)
{
    union {
        TSHostLookupResult  dns;
        TSHttpTxn           txn;
        void *              edata;
    } ptr;

    ptr.edata = edata;
    LtGuardLogDebug ("handling event=%d edata=%p", (int)event, edata);

    switch (event) {
		case TS_EVENT_MGMT_UPDATE:
			read_configuration(contp);
    		return TS_EVENT_NONE;

		case TS_EVENT_HTTP_OS_DNS:
			// Ignore internal requests since we generated them.
			if (TSHttpIsInternalRequest(ptr.txn) == TS_SUCCESS) {
				// All our internal requests *must* hit the origin since it is the
				// agent that needs to make the authorization decision. We can't
				// allow that to be cached.
				TSHttpTxnReqCacheableSet(ptr.txn, 0);

				LtGuardLogDebug("re-enabling internal transaction");
				TSHttpTxnReenable(ptr.txn, TS_EVENT_HTTP_CONTINUE);
				return TS_EVENT_NONE;
			}
			return TS_EVENT_NONE;

		default:
			return TS_EVENT_NONE;
    }
}


void
TSPluginInit(int argc, const char *argv[])
{
    TSPluginRegistrationInfo info;

    info.plugin_name = (char *)"LtGuard";
    info.vendor_name = (char *)"LiveTribe";
    info.support_email = (char *)"dev@livetribe.org";

    if (TSPluginRegister(TS_SDK_VERSION_2_0, &info) != TS_SUCCESS) {
        LtGuardLogError ("plugin registration failed");
    }

    if (!check_ts_version()) {
       LtGuardLogError ("Plugin requires Traffic Server 2.0 or later\n");
       return;
    }

	if (pthread_rwlock_init(&rwlock, NULL)) {
		LtGuardLogError ("Unable to initialize RW lock");
		return;
	}

	if (argc != 2) {
		LtGuardLogError ("usage: %s /path/to/guard.ini\n", argv[0]);
		return;
	}

	lt_guard_continuation = TSContCreate(lt_guard_global_hook, NULL);
	TSHttpHookAdd(TS_HTTP_READ_REQUEST_HDR_HOOK, lt_guard_continuation);

	std::string config_path;
    config_path = std::string(argv[1]);
	char * p = (char*)TSmalloc(config_path.size()+1);
	strcpy(p, config_path.c_str());
	TSContDataSet(lt_guard_continuation, (void*)p);
	read_configuration(lt_guard_continuation);

	TSMgmtUpdateRegister(lt_guard_continuation, "lt-guard");
}

