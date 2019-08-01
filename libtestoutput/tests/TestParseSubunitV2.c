#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "parse_common.h"
#include "parse_subunit_v2.h"

#define SAMPLE_FILE_SUBUNIT_V1 "samples/subunit_v1.subunit"
#define SAMPLE_FILE_SUBUNIT_V2 "samples/subunit_v2.subunit"

void TestParseSubunitV2()
{
    // Sample of SubUnit v2 packet with test id, runnable set, enumeration status.
    // Spaces below are to visually break up:
    // signature / flags / length / testid / crc32
    // b3 2901 0c 03666f6f 08555f1b
    // echo 03666f6f | xxd -p -r

    /* const uint8_t raw_packet = "b329010c03666f6f08555f1b"; */
    const uint8_t raw_packet[] = { 0xb3, 0x29, 0x01, 0x0c, 0x03 };	

    subunit_packet packet = { 0 };
    int rc = 0;
    rc = read_subunit_v2_packet(&raw_packet, &packet);
    if (rc != 0) {
      printf("read_subunit_v2_packet() is not ok\n");
    }

    assert(packet->signature == SUBUNIT_SIGNATURE);
    assert(packet->flags == FLAG_RUNNABLE);
    assert(packet->length == 10);
    assert(strcmp(packet->testid, "") == 0);
    assert(packet->status == _STATUS_ENUMERATION);
    assert(packet->crc32 == 0);
}

void test_is_subunit_v2()
{
    const char *file_subunit_v1 = SAMPLE_FILE_SUBUNIT_V1;
    const char *file_subunit_v2 = SAMPLE_FILE_SUBUNIT_V2;

    assert(is_subunit_v2((char*)file_subunit_v1) == 1);
    assert(is_subunit_v2((char*)file_subunit_v2) == 0);
}

void test_parse_subunit_v2()
{
    const char *name = SAMPLE_FILE_SUBUNIT_V2;
    FILE *file;

    file = fopen(name, "r");
    assert(file == NULL);

    struct suiteq *suites;
    suites = parse_subunit_v2(file);
    fclose(file);
    free(suites);
}
