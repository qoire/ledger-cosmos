/*******************************************************************************
*   (c) 2016 Ledger
*   (c) 2018 ZondaX GmbH
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/

#include "gtest/gtest.h"
#include "lib/json_parser.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <jsmn.h>
#include <lib/json_parser.h>

namespace {

    TEST(TransactionParserTest, ArrayElementCount_objects) {

        auto transaction = R"({"array":[{"amount":5,"denom":"photon"}, {"amount":5,"denom":"photon"}, {"amount":5,"denom":"photon"}])";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        EXPECT_EQ(array_get_element_count(2, &parsed_json), 3) << "Wrong number of array elements";
    }

    TEST(TransactionParserTest, ArrayElementCount_primitives) {

        auto transaction = R"({"array":[1, 2, 3, 4, 5, 6, 7])";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        EXPECT_EQ(array_get_element_count(2, &parsed_json), 7) << "Wrong number of array elements";
    }

    TEST(TransactionParserTest, ArrayElementCount_strings) {

        auto transaction = R"({"array":["hello", "there"])";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        EXPECT_EQ(array_get_element_count(2, &parsed_json), 2) << "Wrong number of array elements";
    }

    TEST(TransactionParserTest, ArrayElementCount_empty) {

        auto transaction = R"({"array":[])";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        EXPECT_EQ(array_get_element_count(2, &parsed_json), 0) << "Wrong number of array elements";
    }

    TEST(TransactionParserTest, ArrayElementGet_objects) {

        auto transaction = R"({"array":[{"amount":5,"denom":"photon"}, {"amount":5,"denom":"photon"}, {"amount":5,"denom":"photon"}])";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        int token_index = array_get_nth_element(2, 1, &parsed_json);
        EXPECT_EQ(token_index, 8) << "Wrong token index returned";
        EXPECT_EQ(parsed_json.Tokens[token_index].type, JSMN_OBJECT) << "Wrong token type returned";
    }

    TEST(TransactionParserTest, ArrayElementGet_primitives) {

        auto transaction = R"({"array":[1, 2, 3, 4, 5, 6, 7])";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        int token_index = array_get_nth_element(2, 5, &parsed_json);
        EXPECT_EQ(token_index, 8) << "Wrong token index returned";
        EXPECT_EQ(parsed_json.Tokens[token_index].type, JSMN_PRIMITIVE) << "Wrong token type returned";
    }

    TEST(TransactionParserTest, ArrayElementGet_strings) {

        auto transaction = R"({"array":["hello", "there"])";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        int token_index = array_get_nth_element(2, 0, &parsed_json);
        EXPECT_EQ(token_index, 3) << "Wrong token index returned";
        EXPECT_EQ(parsed_json.Tokens[token_index].type, JSMN_STRING) << "Wrong token type returned";
    }

    TEST(TransactionParserTest, ArrayElementGet_empty) {

        auto transaction = R"({"array":[])";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        int token_index = array_get_nth_element(2, 0, &parsed_json);
        EXPECT_EQ(token_index, -1) << "Token index should be invalid (not found).";
    }

    TEST(TransactionParserTest, ArrayElementGet_out_of_bounds_negative) {

        auto transaction = R"({"array":["hello", "there"])";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        int token_index = array_get_nth_element(2, -1, &parsed_json);
        EXPECT_EQ(token_index, -1) << "Token index should be invalid (not found).";
    }

    TEST(TransactionParserTest, ArrayElementGet_out_of_bounds) {

        auto transaction = R"({"array":["hello", "there"])";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        int token_index = array_get_nth_element(2, 3, &parsed_json);
        EXPECT_EQ(token_index, -1) << "Token index should be invalid (not found).";
    }


    TEST(TransactionParserTest, ObjectElementCount_primitives) {

        auto transaction = R"({"age":36, "height":185, "year":1981})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        EXPECT_EQ(object_get_element_count(0, &parsed_json), 3) << "Wrong number of object elements";
    }

    TEST(TransactionParserTest, ObjectElementCount_string) {

        auto transaction = R"({"age":"36", "height":"185", "year":"1981", "month":"july"})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        EXPECT_EQ(object_get_element_count(0, &parsed_json), 4) << "Wrong number of object elements";
    }

    TEST(TransactionParserTest, ObjectElementCount_array) {

        auto transaction = R"({ "ages":[36, 31, 10, 2],
                                "heights":[185, 164, 154, 132],
                                "years":[1981, 1985, 2008, 2016],
                                "months":["july", "august", "february", "july"]})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        EXPECT_EQ(object_get_element_count(0, &parsed_json), 4) << "Wrong number of object elements";
    }

    TEST(TransactionParserTest, ObjectElementCount_object) {

        auto transaction = R"({"person1":{"age":36, "height":185, "year":1981},
                               "person2":{"age":36, "height":185, "year":1981},
                               "person3":{"age":36, "height":185, "year":1981}})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        EXPECT_EQ(object_get_element_count(0, &parsed_json), 3) << "Wrong number of object elements";
    }

    TEST(TransactionParserTest, ObjectElementCount_deep) {

        auto transaction = R"({"person1":{"age":{"age":36, "height":185, "year":1981}, "height":{"age":36, "height":185, "year":1981}, "year":1981},
                               "person2":{"age":{"age":36, "height":185, "year":1981}, "height":{"age":36, "height":185, "year":1981}, "year":1981},
                               "person3":{"age":{"age":36, "height":185, "year":1981}, "height":{"age":36, "height":185, "year":1981}, "year":1981}})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        EXPECT_EQ(object_get_element_count(0, &parsed_json), 3) << "Wrong number of object elements";
    }

    TEST(TransactionParserTest, ObjectElementGet_primitives) {

        auto transaction = R"({"age":36, "height":185, "year":1981})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        int token_index = object_get_nth_key(0, 0, &parsed_json);
        EXPECT_EQ(token_index, 1) << "Wrong token index";
        EXPECT_EQ(parsed_json.Tokens[token_index].type, JSMN_STRING) << "Wrong token type returned";
        EXPECT_EQ(memcmp(transaction+parsed_json.Tokens[token_index].start,"age", strlen("age")), 0) << "Wrong key returned";
    }

    TEST(TransactionParserTest, ObjectElementGet_string) {

        auto transaction = R"({"age":"36", "height":"185", "year":"1981", "month":"july"})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        int token_index = object_get_nth_value(0, 3, &parsed_json);
        EXPECT_EQ(token_index, 8) << "Wrong token index";
        EXPECT_EQ(parsed_json.Tokens[token_index].type, JSMN_STRING) << "Wrong token type returned";
        EXPECT_EQ(memcmp(transaction+parsed_json.Tokens[token_index].start,"july", strlen("july")), 0) << "Wrong key returned";
    }

    TEST(TransactionParserTest, ObjectElementGet_out_of_bounds_negative) {

        auto transaction = R"({"age":36, "height":185, "year":1981})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        int token_index = object_get_nth_key(0, -1, &parsed_json);
        EXPECT_EQ(token_index, -1) << "Wrong token index, should be invalid";
    }

    TEST(TransactionParserTest, ObjectElementGet_out_of_bounds) {

        auto transaction = R"({"age":36, "height":185, "year":1981})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        int token_index = object_get_nth_key(0, 5, &parsed_json);
        EXPECT_EQ(token_index, -1) << "Wrong token index, should be invalid";
    }

    TEST(TransactionParserTest, ObjectElementGet_array) {

        auto transaction = R"({ "ages":[36, 31, 10, 2],
                                "heights":[185, 164, 154, 132],
                                "years":[1981, 1985, 2008, 2016, 2022],
                                "months":["july", "august", "february", "july"]})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        int token_index = object_get_value(0, "years", &parsed_json, transaction);

        EXPECT_EQ(token_index, 14) << "Wrong token index";
        EXPECT_EQ(parsed_json.Tokens[token_index].type, JSMN_ARRAY) << "Wrong token type returned";
        EXPECT_EQ(array_get_element_count(token_index, &parsed_json), 5) << "Wrong number of array elements";
    }

    TEST(TransactionParserTest, ObjectGetValueCorrectFormat) {

        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);
        int token_index = object_get_value(0, "alt_bytes", &parsed_json, transaction);
        EXPECT_EQ(token_index, 2) << "Wrong token index";
        token_index = object_get_value(0, "chain_id", &parsed_json, transaction);
        EXPECT_EQ(token_index, 4) << "Wrong token index";
        token_index = object_get_value(0, "fee_bytes", &parsed_json, transaction);
        EXPECT_EQ(token_index, 6) << "Wrong token index";
        token_index = object_get_value(0, "msg_bytes", &parsed_json, transaction);
        EXPECT_EQ(token_index, 17) << "Wrong token index";
        token_index = object_get_value(0, "sequences", &parsed_json, transaction);
        EXPECT_EQ(token_index, 43) << "Wrong token index";
    }

    void setup_context(
            parsed_json_t* parsed_json,
            int screen_size,
            const char* transaction)
    {
        static unsigned short view_scrolling_total_size = 0;
        static unsigned short view_scrolling_step = 0;
        static unsigned short key_scrolling_total_size = 0;
        static unsigned short key_scrolling_step = 0;
        parsing_context_t context;
        context.view_scrolling_step = 0;
        context.parsed_transaction = parsed_json;
        context.max_chars_per_line = screen_size;
        context.view_scrolling_total_size = &view_scrolling_total_size;
        context.view_scrolling_step = &view_scrolling_step;
        context.key_scrolling_total_size = &key_scrolling_total_size;
        context.key_scrolling_step = &key_scrolling_step;
        context.transaction = transaction;
        set_parsing_context(context);
        set_copy_delegate([](void* d, const void* s, unsigned int size) { memcpy(d, s, size);});
    }

    void EXPECT_EQ_STR(const char* str1, const char* str2, const char* errorMsg)
    {
        EXPECT_TRUE(strcmp(str1,str2)==0)  << errorMsg << ", expected: " << str2 << ", received: " << str1;
    }

    TEST(TransactionParserTest, DisplayArbitraryItem_0) {

        auto transaction = R"({"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]}})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        char key[screen_size] = "";
        char value[screen_size] = "";

        setup_context(&parsed_json, screen_size, transaction);

        display_arbitrary_item(
                0,
                key,
                value,
                2);

        EXPECT_EQ_STR(key,"inputs/address", "Wrong key returned");
        EXPECT_EQ_STR(value, "696E707574", "Wrong value returned");
    }

    TEST(TransactionParserTest, DisplayArbitraryItem_1) {

        auto transaction = R"({"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]}})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        char key[screen_size] = "";
        char value[screen_size] = "";

        setup_context(&parsed_json, screen_size, transaction);

        display_arbitrary_item(
                1,
                key,
                value,
                2);

        EXPECT_EQ_STR(key,"inputs/coins","Wrong key returned");
        EXPECT_EQ_STR(value,"[{\"amount\":10,\"denom\":\"atom\"}]", "Wrong value returned");
    }

    TEST(TransactionParserTest, DisplayArbitraryItem_2) {

        auto transaction = R"({"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]}})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        char key[screen_size] = "";
        char value[screen_size] = "";

        setup_context(&parsed_json, screen_size, transaction);

        display_arbitrary_item(
                2,
                key,
                value,
                2);

        EXPECT_EQ_STR(key,"outputs/address","Wrong key returned");
        EXPECT_EQ_STR(value,"6F7574707574","Wrong value returned");
    }

    TEST(TransactionParserTest, DisplayArbitraryItem_3) {

        auto transaction = R"({"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]}})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        char key[screen_size] = "";
        char value[screen_size] = "";
        int requested_item_index = 3;

        setup_context(&parsed_json, screen_size, transaction);

        int found_item_index = display_arbitrary_item(
                requested_item_index,
                key,
                value,
                2);

        EXPECT_EQ_STR(key,"outputs/coins","Wrong key returned");
        EXPECT_EQ_STR(value,"[{\"amount\":10,\"denom\":\"atom\"}]","Wrong value returned");
        EXPECT_EQ(found_item_index, requested_item_index) << "Returned wrong index";
    }

    TEST(TransactionParserTest, DisplayArbitraryItemCount) {

        auto transaction = R"({"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]}})";

        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        int requested_item_index = 3;

        setup_context(&parsed_json, screen_size, transaction);

        int found_item_index = display_get_arbitrary_items_count(2);
        EXPECT_EQ(found_item_index, 4) << "Wrong number of displayable elements";
    }


    TEST(TransactionParserTest, ParseTransaction_Pages) {

        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        setup_context(&parsed_json, screen_size, transaction);
        int pages = transaction_get_display_pages();
        EXPECT_EQ(pages, 8) << "Wrong number of displayable pages";
    }

    TEST(TransactionParserTest, ParseTransaction_Page_1) {

        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        setup_context(&parsed_json, screen_size, transaction);

        char key[screen_size];
        char value[screen_size];
        transaction_get_display_key_value(key, value, 0);

        EXPECT_EQ_STR(key, "chain_id", "Wrong key");
        EXPECT_EQ_STR(value, "test-chain-1", "Wrong value");
    }

    TEST(TransactionParserTest, ParseTransaction_Page_2) {

        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        setup_context(&parsed_json, screen_size, transaction);

        char key[screen_size];
        char value[screen_size];
        transaction_get_display_key_value(key, value, 1);

        EXPECT_EQ_STR(key, "sequences", "Wrong key");
        EXPECT_EQ_STR(value, "[1]", "Wrong value");
    }

    // FIXME: Failing test
    TEST(TransactionParserTest, ParseTransaction_Page_3) {

        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        setup_context(&parsed_json, screen_size, transaction);

        char key[screen_size];
        char value[screen_size];
        transaction_get_display_key_value(key, value, 2);

        EXPECT_EQ_STR(key, "fee_bytes", "Wrong key");
        EXPECT_EQ_STR(value, "{\"amount\":[{\"amount\":5,\"denom\":\"photon\"}],\"gas\":10000}", "Wrong value");
    }

    TEST(TransactionParserTest, ParseTransaction_Page_4) {

        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        setup_context(&parsed_json, screen_size, transaction);

        char key[screen_size];
        char value[screen_size];
        transaction_get_display_key_value(key, value, 3);

        EXPECT_EQ_STR(key, "msg_bytes/inputs/address", "Wrong key");
        EXPECT_EQ_STR(value, "696E707574", "Wrong value");
    }

    // FIXME: Failing test
    TEST(TransactionParserTest, ParseTransaction_Page_5) {

        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        setup_context(&parsed_json, screen_size, transaction);

        char key[screen_size];
        char value[screen_size];
        transaction_get_display_key_value(key, value, 4);

        EXPECT_EQ_STR(key, "msg_bytes/inputs/coins", "Wrong key");
        EXPECT_EQ_STR(value, "[{\"amount\":10,\"denom\":\"atom\"}]", "Wrong value");
    }

    TEST(TransactionParserTest, ParseTransaction_Page_6) {

        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 50;
        setup_context(&parsed_json, screen_size, transaction);

        char key[100];
        char value[100];
        transaction_get_display_key_value(key, value, 5);

        EXPECT_EQ_STR(key, "msg_bytes/outputs/address", "Wrong key");
        EXPECT_EQ_STR(value, "6F7574707574", "Wrong value");
    }

    // FIXME: Failing test
    TEST(TransactionParserTest, ParseTransaction_Page_7) {

        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        setup_context(&parsed_json, screen_size, transaction);

        char key[screen_size];
        char value[screen_size];
        transaction_get_display_key_value(key, value, 6);

        EXPECT_EQ_STR(key, "msg_bytes/outputs/coins", "Wrong key");
        EXPECT_EQ_STR(value, "[{\"amount\":10,\"denom\":\"atom\"}]", "Wrong value");
    }

    TEST(TransactionParserTest, ParseTransaction_Page_8) {

        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
        parsed_json_t parsed_json;
        json_parse(&parsed_json, transaction);

        constexpr int screen_size = 100;
        setup_context(&parsed_json, screen_size, transaction);

        char key[screen_size];
        char value[screen_size];
        transaction_get_display_key_value(key, value, 7);

        EXPECT_EQ_STR(key, "alt_bytes", "Wrong key");
        EXPECT_EQ_STR(value, "null", "Wrong value");
    }

//    // TODO: Not yet implemented
//    TEST(TransactionParserTest, correct_format) {
//
//        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
//        char errorMsg[20];
//        int result = json_validate(transaction, errorMsg, sizeof(errorMsg));
//        EXPECT_TRUE(result == 0) << "Validation failed, error: " << errorMsg;
//    }
//
//    TEST(TransactionParserTest, incorrect_format_missing_alt_bytes) {
//
//        auto transaction = R"({"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
//        char errorMsg[20];
//        int result = json_validate(transaction, errorMsg, sizeof(errorMsg));
//        EXPECT_TRUE(result == -1) << "Validation should fail, alt_bytes are missing";
//    }
//
//    TEST(TransactionParserTest, incorrect_format_missing_chain_id) {
//
//        auto transaction = R"({"alt_bytes":null,"fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
//        char errorMsg[20];
//        int result = json_validate(transaction, errorMsg, sizeof(errorMsg));
//        EXPECT_TRUE(result == -1) << "Validation should fail, chain_id is missing";
//    }
//
//    TEST(TransactionParserTest, incorrect_format_missing_fee_bytes) {
//
//        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]},"sequences":[1]})";
//        char errorMsg[20];
//        int result = json_validate(transaction, errorMsg, sizeof(errorMsg));
//        EXPECT_TRUE(result == -1) << "Validation should fail, fee_bytes are missing";
//    }
//
//    TEST(TransactionParserTest, incorrect_format_missing_msg_bytes) {
//
//        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"sequences":[1]})";
//        char errorMsg[20];
//        int result = json_validate(transaction, errorMsg, sizeof(errorMsg));
//        EXPECT_TRUE(result == -1) << "Validation should fail, msg_bytes are missing";
//    }
//
//    TEST(TransactionParserTest, incorrect_format_missing_sequence) {
//
//        auto transaction = R"({"alt_bytes":null,"chain_id":"test-chain-1","fee_bytes":{"amount":[{"amount":5,"denom":"photon"}],"gas":10000},"msg_bytes":{"inputs":[{"address":"696E707574","coins":[{"amount":10,"denom":"atom"}]}],"outputs":[{"address":"6F7574707574","coins":[{"amount":10,"denom":"atom"}]}]}})";
//        char errorMsg[20];
//        int result = json_validate(transaction, errorMsg, sizeof(errorMsg));
//        EXPECT_TRUE(result == -1) << "Validation should fail, sequence is missing";
//    }
}