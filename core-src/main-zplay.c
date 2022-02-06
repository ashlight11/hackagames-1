
#include <czmq.h>
#include <assert.h>

int main (void)
{
    zsock_t *writer = zsock_new_push ("@tcp://127.0.0.1:5560");
    assert (writer);
    assert (zsock_resolve (writer) != writer);
    assert (streq (zsock_type_str (writer), "PUSH"));

    int rc;
    #if (ZMQ_VERSION >= ZMQ_MAKE_VERSION (3, 2, 0))
    // Check unbind
    rc = zsock_unbind (writer, "tcp://127.0.0.1:%d", 5560);
    assert (rc == 0);

    // In some cases and especially when running under Valgrind, doing
    // a bind immediately after an unbind causes an EADDRINUSE error.
    // Even a short sleep allows the OS to release the port for reuse.
    zclock_sleep (100);

    // Bind again
    rc = zsock_bind (writer, "tcp://127.0.0.1:%d", 5560);
    assert (rc == 5560);
    assert (streq (zsock_endpoint (writer), "tcp://127.0.0.1:5560"));
    #endif

    zsock_t *reader = zsock_new_pull (">tcp://127.0.0.1:5560");
    assert (reader);
    assert (zsock_resolve (reader) != reader);
    assert (streq (zsock_type_str (reader), "PULL"));

    // Basic Hello, World
    zstr_send (writer, "Hello, World");
    puts("send> Hello, world");
    zmsg_t *msg = zmsg_recv (reader);
    assert (msg);
    char *string = zmsg_popstr (msg);
    printf("recive> %s (%ld)\n", string, strlen(string));
    assert (streq (string, "Hello, World"));
    free (string);
    zmsg_destroy (&msg);

    // Test resolve libzmq socket
    #if (ZMQ_VERSION >= ZMQ_MAKE_VERSION (3, 2, 0))
    void *zmq_ctx = zmq_ctx_new ();
    #else
    void *zmq_ctx = zmq_ctx_new (1);
    #endif
    assert (zmq_ctx);
    void *zmq_sock = zmq_socket (zmq_ctx, ZMQ_PUB);
    assert (zmq_sock);
    assert (zsock_resolve (zmq_sock) == zmq_sock);
    zmq_close (zmq_sock);
    zmq_ctx_term (zmq_ctx);

    // Test zsock_attach method
    zsock_t *server = zsock_new (ZMQ_DEALER);
    assert (server);
    rc = zsock_attach (server, "@inproc://myendpoint,tcp://127.0.0.1:5556,inproc://others", true);
    assert (rc == 0);
    rc = zsock_attach (server, "", false);
    assert (rc == 0);
    rc = zsock_attach (server, NULL, true);
    assert (rc == 0);
    rc = zsock_attach (server, ">a,@b, c,, ", false);
    assert (rc == -1);
    zsock_destroy (&server);

    // Test zsock_endpoint method
    rc = zsock_bind (writer, "inproc://test.%s", "writer");
    assert (rc == 0);
    assert (streq (zsock_endpoint (writer), "inproc://test.writer"));

    // Test error state when connecting to an invalid socket type
    // ('txp://' instead of 'tcp://', typo intentional)
    rc = zsock_connect (reader, "txp://127.0.0.1:5560");
    assert (rc == -1);

    // Test signal/wait methods
    rc = zsock_signal (writer, 123);
    assert (rc == 0);
    rc = zsock_wait (reader);
    assert (rc == 123);

    // Test zsock_send/recv pictures
    uint8_t number1 = 123;
    uint16_t number2 = 123 * 123;
    uint32_t number4 = 123 * 123;
    number4 *= 123;
    uint32_t number4_MAX = UINT32_MAX;
    uint64_t number8 = 123 * 123;
    number8 *= 123;
    number8 *= 123;
    uint64_t number8_MAX = UINT64_MAX;

    zchunk_t *chunk = zchunk_new ("HELLO", 5);
    assert (chunk);
    zframe_t *frame = zframe_new ("WORLD", 5);
    assert (frame);
    zhashx_t *hash = zhashx_new ();
    assert (hash);
    zuuid_t *uuid = zuuid_new ();
    assert (uuid);
    zhashx_set_destructor (hash, (zhashx_destructor_fn *) zstr_free);
    zhashx_set_duplicator (hash, (zhashx_duplicator_fn *) strdup);
    zhashx_insert (hash, "1", "value A");
    zhashx_insert (hash, "2", "value B");
    char *original = "pointer";

    // Test zsock_recv into each supported type
    zsock_send (writer, "i124488zsbcfUhp",
    -12345, number1, number2, number4, number4_MAX,
    number8, number8_MAX,
    "This is a string", "ABCDE", 5,
    chunk, frame, uuid, hash, original);
    char *uuid_str = strdup (zuuid_str (uuid));
    zchunk_destroy (&chunk);
    zframe_destroy (&frame);
    zuuid_destroy (&uuid);
    zhashx_destroy (&hash);

    int integer;
    byte *data;
    size_t size;
    char *pointer;
    number8_MAX = number8 = number4_MAX = number4 = number2 = number1 = 0ULL;
    rc = zsock_recv (reader, "i124488zsbcfUhp",
    &integer, &number1, &number2, &number4, &number4_MAX,
    &number8, &number8_MAX, &string, &data, &size, &chunk,
    &frame, &uuid, &hash, &pointer);
    assert (rc == 0);
    assert (integer == -12345);
    assert (number1 == 123);
    assert (number2 == 123 * 123);
    assert (number4 == 123 * 123 * 123);
    assert (number4_MAX == UINT32_MAX);
    assert (number8 == 123 * 123 * 123 * 123);
    assert (number8_MAX == UINT64_MAX);
    assert (streq (string, "This is a string"));
    assert (memcmp (data, "ABCDE", 5) == 0);
    assert (size == 5);
    assert (memcmp (zchunk_data (chunk), "HELLO", 5) == 0);
    assert (zchunk_size (chunk) == 5);
    assert (streq (uuid_str, zuuid_str (uuid)));
    assert (memcmp (zframe_data (frame), "WORLD", 5) == 0);
    assert (zframe_size (frame) == 5);
    char *value = (char *) zhashx_lookup (hash, "1");
    assert (streq (value, "value A"));
    value = (char *) zhashx_lookup (hash, "2");
    assert (streq (value, "value B"));
    assert (original == pointer);
    free (string);
    free (data);
    free (uuid_str);
    zframe_destroy (&frame);
    zchunk_destroy (&chunk);
    zhashx_destroy (&hash);
    zuuid_destroy (&uuid);

    // Test zsock_recv of short message; this lets us return a failure
    // with a status code and then nothing else; the receiver will get
    // the status code and NULL/zero for all other values
    zsock_send (writer, "i", -1);
    zsock_recv (reader, "izsbcfp",
    &integer, &string, &data, &size, &chunk, &frame, &pointer);
    assert (integer == -1);
    assert (string == NULL);
    assert (data == NULL);
    assert (size == 0);
    assert (chunk == NULL);
    assert (frame == NULL);
    assert (pointer == NULL);

    msg = zmsg_new ();
    zmsg_addstr (msg, "frame 1");
    zmsg_addstr (msg, "frame 2");
    zsock_send (writer, "szm", "header", msg);
    zmsg_destroy (&msg);

    zsock_recv (reader, "szm", &string, &msg);

    assert (streq ("header", string));
    assert (zmsg_size (msg) == 2);
    assert (zframe_streq (zmsg_first (msg), "frame 1"));
    assert (zframe_streq (zmsg_next (msg), "frame 2"));
    zstr_free (&string);
    zmsg_destroy (&msg);

    // Test zsock_recv with null arguments
    chunk = zchunk_new ("HELLO", 5);
    assert (chunk);
    frame = zframe_new ("WORLD", 5);
    assert (frame);
    zsock_send (writer, "izsbcfp",
    -12345, "This is a string", "ABCDE", 5, chunk, frame, original);
    zframe_destroy (&frame);
    zchunk_destroy (&chunk);
    zsock_recv (reader, "izsbcfp", &integer, NULL, NULL, NULL, &chunk, NULL, NULL);
    assert (integer == -12345);
    assert (memcmp (zchunk_data (chunk), "HELLO", 5) == 0);
    assert (zchunk_size (chunk) == 5);
    zchunk_destroy (&chunk);

    // Test zsock_bsend/brecv pictures with binary encoding
    frame = zframe_new ("Hello", 5);
    chunk = zchunk_new ("World", 5);

    msg = zmsg_new ();
    zmsg_addstr (msg, "Hello");
    zmsg_addstr (msg, "World");

    zsock_bsend (writer, "1248sSpcfm",
    number1, number2, number4, number8,
    "Hello, World",
    "Goodbye cruel World!",
    original,
    chunk, frame, msg);
    zchunk_destroy (&chunk);
    zframe_destroy (&frame);
    zmsg_destroy (&msg);

    number8 = number4 = number2 = number1 = 0;
    char *longstr;
    zsock_brecv (reader, "1248sSpcfm",
    &number1, &number2, &number4, &number8,
    &string, &longstr,
    &pointer,
    &chunk, &frame, &msg);
    assert (number1 == 123);
    assert (number2 == 123 * 123);
    assert (number4 == 123 * 123 * 123);
    assert (number8 == 123 * 123 * 123 * 123);
    assert (streq (string, "Hello, World"));
    assert (streq (longstr, "Goodbye cruel World!"));
    assert (pointer == original);
    zstr_free (&longstr);
    zchunk_destroy (&chunk);
    zframe_destroy (&frame);
    zmsg_destroy (&msg);

    zsock_destroy (&reader); zsock_destroy (&writer);
}