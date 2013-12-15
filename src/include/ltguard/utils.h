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

/**
 * @file utils.h
 * @brief Contains utility functions
 */

#pragma once
#ifndef LTGUARD_UTILS_H_
#define LTGUARD_UTILS_H_

#include <ts/ts.h>
#include <openssl/rsa.h>

#define LtGuardLogDebug(fmt, ...) TSDebug("lt-guard", "%s: " fmt, __func__, ##__VA_ARGS__)
#define LtGuardLogError(fmt, ...) TSError(fmt, ##__VA_ARGS__)

int check_ts_version();
RSA* getRsaFp(const char* rsaprivKeyPath);

#endif /* LTGUARD_UTILS_H_ */
