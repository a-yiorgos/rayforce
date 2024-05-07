/*
 *   Copyright (c) 2023 Anton Kundenko <singaraiona@gmail.com>
 *   All rights reserved.

 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:

 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.

 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "parse.h"
#include "hash.h"
#include "format.h"
#include "util.h"
#include "string.h"
#include "poll.h"
#include "heap.h"
#include "runtime.h"
#include "sys.h"

#define __ABOUT "\
  %sRayforceDB: %d.%d %s\n\
  WASM target\n\
  Documentation: https://rayforcedb.com/\n\
  Github: https://github.com/singaraiona/rayforce%s\n"

// Declare rayforce_ready callback on js side
EM_JS(nil_t, js_rayforce_ready, (str_p text), {
    Module.rayforce_ready(UTF8ToString(text));
});

poll_p poll_init(i64_t port)
{
    poll_p poll = (poll_p)heap_alloc(sizeof(struct poll_t));
    poll->code = NULL_I64;

    return poll;
}

nil_t poll_destroy(poll_p poll)
{
    heap_free(poll);
}

i64_t poll_run(poll_p poll)
{
    unused(poll);
    return 0;
}

i64_t poll_register(poll_p poll, i64_t fd, u8_t version)
{
    unused(poll);
    unused(fd);
    unused(version);
    return 0;
}

nil_t poll_deregister(poll_p poll, i64_t id)
{
    unused(poll);
    unused(id);
}

obj_p ipc_send_sync(poll_p poll, i64_t id, obj_p msg)
{
    unused(poll);
    unused(id);
    unused(msg);
    return NULL_OBJ;
}

obj_p ipc_send_async(poll_p poll, i64_t id, obj_p msg)
{
    unused(poll);
    unused(id);
    unused(msg);
    return NULL_OBJ;
}

EMSCRIPTEN_KEEPALIVE str_p strof_obj(obj_p obj)
{
    return as_string(obj);
}

EMSCRIPTEN_KEEPALIVE i32_t main(i32_t argc, str_p argv[])
{
    i32_t code;
    obj_p fmt;
    sys_info_t info = sys_info(1);

    fmt = str_fmt(-1, __ABOUT, BOLD, info.major_version, info.minor_version, info.build_date, RESET);

    atexit(runtime_destroy);
    runtime_init(0, NULL);
    code = runtime_run();
    js_rayforce_ready(as_string(fmt));
    drop_obj(fmt);

    return code;
}
